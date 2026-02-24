#include "Game/GameObjects/EScreenObject.h"

TWeak<ESprite> EScreenObject::AddSprite(const EString& texturePath, const ESTransform2D& transform)
{
    TShared<ESprite> sprite = TMakeShared<ESprite>(texturePath, transform);

    // Set sprite scale to texture size if not set
    if (transform.scale == glm::vec2(0.0f)) {
        sprite->SetScaleToTextureSize();
    }

    m_sprites.push_back(sprite);
    return sprite;
}

void EScreenObject::Render(const TShared<EShaderProgram>& shader)
{
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
