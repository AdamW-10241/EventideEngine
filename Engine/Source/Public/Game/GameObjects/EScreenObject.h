#pragma once
#include "Game/GameObjects/EObject.h"
#include "Graphics/ESprite.h"

class EScreenObject : public EObject {
public:
	EScreenObject() = default;
	virtual ~EScreenObject() = default;
	
    // Creates and stores a sprite, returns a weak ref if you need to modify it later
    TWeak<ESprite> AddSprite(const EString& texturePath, const ESTransform2D& transform);

    // Render sprites
    void Render(const TShared<EShaderProgram>& shader);

    // Get sprites
    TArray<TShared<ESprite>>& GetSprites() { return m_sprites; }

    TWeak<ESprite> GetSprite(const EString& texturePath);

    TWeak<ESprite> GetSprite(const int index) { return m_sprites.at(index); }

protected:
    TArray<TShared<ESprite>> m_sprites;
};