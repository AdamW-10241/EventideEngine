#pragma once
#include "Game/GameObjects/EObject.h"
#include "Graphics/ESprite.h"
#include "Graphics/EText.h"

#define DEFAULT_FONT_SIZE_MULTI 1.2f

template<typename T>
concept IsSpriteConfig = requires(T t) {
    static_cast<ESAddSpriteConfigBase<T>&>(t);
};

class EScreenObject : public EObject {
public:
    EScreenObject() { m_renderOrder = 0; }
    EScreenObject(EUi32 renderOrder) { m_renderOrder = renderOrder; }

    template<typename T>
        requires IsSpriteConfig<T>
    TWeak<ESprite> AddSprite(const T& config);

    // Render sprites
    void Render(const TShared<EShaderProgram>& shader);

    // Adds text OnTicked binding to set text and size using passed functions
    void AddTextBindingTick(std::function<EString()> text, const int spriteIndex = 0);

    void AddTextBindingTick(const int spriteIndex) {
        AddTextBindingTick([] { return EString(""); }, spriteIndex);
    }

    // Set render order
    void SetRenderOrder(const EUi32 renderOrder) { m_renderOrder = renderOrder; }

    // Get render order
    EUi32& GetRenderOrder() { return m_renderOrder; }

    // Get sprites
    TArray<TWeak<ESprite>> GetSprites() { 
        TArray<TWeak<ESprite>> weakSprites;
        for (auto sprite : m_sprites) {
            weakSprites.push_back(sprite);
        }
        return weakSprites;
    }

    TWeak<ESprite> GetSprite(const EString& texturePath);

    TWeak<ESprite> GetSprite(const int index) {
        if ((int)m_sprites.size() > index) {
            return m_sprites.at(index);
        }
        return {};
    }

    // Manip all sprites in array
    void SetSpritesTransforms(const ESTransform2D transform) {
        for (auto sprite : m_sprites)
            sprite->GetTransform() = transform;
    }

    void SetSpritesRenderColors(const glm::vec4 renderColor) { 
        for (auto sprite : m_sprites)
            sprite->SetRenderColor(renderColor);
    }

    void SetSpritesRenderScales(const glm::vec2 renderScale) {
        for (auto sprite : m_sprites)
            sprite->SetRenderScale(renderScale);
    }
    void SetSpritesRenderScales(const float renderScale) { 
        SetSpritesRenderScales(glm::vec2(renderScale));
    }

protected:
    TArray<TShared<ESprite>> m_sprites;

    // Store render order
    EUi32 m_renderOrder;
};

template<typename T>
    requires IsSpriteConfig<T>
inline TWeak<ESprite> EScreenObject::AddSprite(const T& config)
{
    // Create sprite
    TShared<ESprite> sprite;
    if constexpr (std::is_base_of_v<ESAddTextConfig, T>) {
        sprite = TMakeShared<EText>(static_cast<const ESAddTextConfig&>(config));
    }
    else {
        sprite = TMakeShared<ESprite>(config);
    }

    m_sprites.push_back(sprite);
    return sprite;
}
