#include "Game/GameObjects/EScreenObject.h"
#include "Graphics/EText.h"

// External Libs
#include <algorithm>

TWeak<ESprite> EScreenObject::AddSprite(ESAddSpriteConfig config)
{
    // Create sprite
    auto sprite = config.isText ?
        TMakeShared<EText>(config.texturePath, config.transform, config.renderOrder, config.renderColor) :
        TMakeShared<ESprite>(config.texturePath, config.transform, config.renderOrder, config.renderColor);

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

TWeak<ESprite> EScreenObject::GetSprite(const EString& texturePath)
{
    // Return matching texture
    for (TShared<ESprite>& sprite : m_sprites) {
        if (sprite->GetImportPath() == texturePath) { return sprite; }
    }

    return {};
}
