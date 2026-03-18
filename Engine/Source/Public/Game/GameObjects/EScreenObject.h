#pragma once
#include "Game/GameObjects/EObject.h"
#include "Graphics/ESprite.h"

class EScreenObject : public EObject {
public:
    EScreenObject() { m_renderOrder = 0; }
    EScreenObject(EUi32 renderOrder) { m_renderOrder = renderOrder; }
	
    // Creates and stores a sprite, returns a weak ref if you need to modify it later
    TWeak<ESprite> AddSprite(ESAddSpriteConfig config);

    // Render sprites
    void Render(const TShared<EShaderProgram>& shader);

    // Adds text OnTicked binding to set text and size using passed functions
    void AddTextBindingTick(std::function<EString()> text, std::function<float()> fontSizeMulti, const int spriteIndex = 0);

    void AddTextBindingTick(std::function<EString()> text, const float fontSizeMulti = 1.2f, const int spriteIndex = 0);

    // Apply multi to aspect ratio if non-square for use with text binding tick
    float GetAspectRatioMulti(const float multi = 1.0f);

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