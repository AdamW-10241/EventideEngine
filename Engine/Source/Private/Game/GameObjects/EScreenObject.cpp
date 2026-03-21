#include "Game/GameObjects/EScreenObject.h"
#include "Graphics/EText.h"

// External Libs
#include <algorithm>

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

void EScreenObject::AddTextBindingTick(std::function<EString()> text, const int spriteIndex)
{
    // Create binding
    auto binding = [text, spriteIndex](TShared<EObject> obj, float deltaTime) {
        if (auto screenRef = TCast<EScreenObject>(obj)) {
            if (auto textRef = TCast<EText>(screenRef->GetSprite(spriteIndex).lock())) {
                // Set new text
                EString string = text();
                if (!string.empty()) textRef->SetText(string);
            }
            else EDebug::Log("Could not create tick text binding.\n", LT_ERROR);
        }
    };

    // Bind event
    BIND_EVENT_SELF(GetSharedRef<EObject>(), OnTicked, binding);
}

TWeak<ESprite> EScreenObject::GetSprite(const EString& texturePath)
{
    // Return matching texture
    for (TShared<ESprite>& sprite : m_sprites) {
        if (sprite->GetPath() == texturePath) { return sprite; }
    }

    return {};
}
