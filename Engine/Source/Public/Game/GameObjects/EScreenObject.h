#pragma once
#include "Game/GameObjects/EObject.h"
#include "Graphics/ESprite.h"

class EScreenObject : public EObject {
public:
    EScreenObject() { m_renderOrder = 0; }
    EScreenObject(EUi32 renderOrder) { m_renderOrder = renderOrder; }
	
    // Creates and stores a sprite, returns a weak ref if you need to modify it later
    TWeak<ESprite> AddSprite(const EString& texturePath, const ESTransform2D& transform, const EUi32 renderOrder, 
        const glm::vec4 renderColor = glm::vec4(1.0f));

    TWeak<ESprite> AddSprite(const ESTransform2D& transform, const EUi32 renderOrder,
        const glm::vec4 renderColor = glm::vec4(1.0f));

    // Render sprites
    void Render(const TShared<EShaderProgram>& shader);

    // Set render order
    void SetRenderOrder(const EUi32 renderOrder) { m_renderOrder = renderOrder; }

    // Get render order
    EUi32& GetRenderOrder() { return m_renderOrder; }

    // Get sprites
    TArray<TShared<ESprite>>& GetSprites() { return m_sprites; }

    TWeak<ESprite> GetSprite(const EString& texturePath);

    TWeak<ESprite> GetSprite(const int index) { return m_sprites.at(index); }

protected:
    TArray<TShared<ESprite>> m_sprites;

    // Store render order
    EUi32 m_renderOrder;
};