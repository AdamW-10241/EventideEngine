#include "Game/GameObjects/EScreenObject.h"
#include "Graphics/EText.h"

// External Libs
#include <algorithm>

TWeak<ESprite> EScreenObject::AddSprite(ESAddSpriteConfig config)
{
    // Create sprite
    auto sprite = config.isText ? TMakeShared<EText>(config) : TMakeShared<ESprite>(config);

    // Set sprite scale to texture size if not set
    if (config.transform.scale == glm::vec2(0.0f)) {
        sprite->SetScaleToTextureSize();
    }

    m_sprites.push_back(sprite);
    return sprite;
}

void EScreenObject::Render(const TShared<EShaderProgram>& shader)
{
    std::sort(m_sprites.begin(), m_sprites.end(),
        [](const TShared<ESprite>& a, const TShared<ESprite>& b) {
            return a->GetRenderOrder() < b->GetRenderOrder();
    });

    for (const auto& sprite : m_sprites) {
        if (const auto& text = TCast<EText>(sprite)) {
            text->Render(shader, true);
            continue;
        }
        sprite->Render(shader);
    }
}

void EScreenObject::AddTextBindingTick(std::function<EString()> text, std::function<float()> fontSizeMulti, const int spriteIndex)
{
    // Create binding
    auto binding = [text, fontSizeMulti, spriteIndex](TShared<EObject> obj, float deltaTime) {
        if (auto screenRef = TCast<EScreenObject>(obj)) {
            if (auto textRef = TCast<EText>(screenRef->GetSprite(spriteIndex).lock())) {
                // Set new text
                EString string = text();
                if (!string.empty()) textRef->SetText(string);

                // Update font size
                float sizeMulti = fontSizeMulti();
                if (sizeMulti >= 0.0f) textRef->SetFontSizeMulti(sizeMulti);
            }
            else EDebug::Log("Could not create tick text binding.\n", LT_ERROR);
        }
    };

    // Bind event
    BIND_EVENT_SELF(GetSharedRef<EObject>(), OnTicked, binding);
}

void EScreenObject::AddTextBindingTick(std::function<EString()> text, const float fontSizeMulti, const int spriteIndex)
{
    auto selfWeak = GetWeakRef<EScreenObject>();
    AddTextBindingTick(text, [selfWeak, fontSizeMulti] {
        if (auto self = selfWeak.lock())
            return self->GetAspectRatioMulti(fontSizeMulti);
        return -1.0f;
    }, spriteIndex);
}

float EScreenObject::GetAspectRatioMulti(float multi)
{
    // Multiply non-square aspect ratios so full screen objects are scaled
    if (auto window = EGameEngine::GetGameEngine()->GetWindow().lock()) {
        float aspectRatio = window->GetAspectRatio();
        return (aspectRatio == 1.0f) ? aspectRatio : aspectRatio * multi;
    }
    return -1.0f;
}

TWeak<ESprite> EScreenObject::GetSprite(const EString& texturePath)
{
    // Return matching texture
    for (TShared<ESprite>& sprite : m_sprites) {
        if (sprite->GetImportPath() == texturePath) { return sprite; }
    }

    return {};
}
