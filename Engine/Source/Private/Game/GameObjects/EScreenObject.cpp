#include "Game/GameObjects/EScreenObject.h"

// External Libs
#include <algorithm>

TWeak<ESprite> EScreenObject::AddSprite(const EString& texturePath, const ESTransform2D& transform, const EUi32 renderOrder, const glm::vec4 renderColor)
{
    TShared<ESprite> sprite = TMakeShared<ESprite>(texturePath, transform, renderOrder, renderColor);

    // Set sprite scale to texture size if not set
    if (transform.scale == glm::vec2(0.0f)) {
        sprite->SetScaleToTextureSize();
    }

    m_sprites.push_back(sprite);
    return sprite;
}

TWeak<ESprite> EScreenObject::AddSprite(const ESTransform2D& transform, const EUi32 renderOrder, const glm::vec4 renderColor)
{
    TShared<ESprite> sprite = TMakeShared<ESprite>(transform, renderOrder, renderColor);

    // Set sprite scale to texture size if not set
    if (transform.scale == glm::vec2(0.0f)) {
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
