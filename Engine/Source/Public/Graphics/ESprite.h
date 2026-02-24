#pragma once
#include "Graphics/ETexture.h"
#include "Graphics/EMesh.h"
#include "Graphics/EShaderProgram.h"
#include "Math/ESTransform.h"

class ESprite : public EMesh, public ETexture {
public:
	ESprite() { m_transform = ESTransform2D(); }
	ESprite(const EString& texturePath) { CreateSprite(texturePath); m_transform = ESTransform2D(); }
	ESprite(const EString& texturePath, const ESTransform2D transform) { CreateSprite(texturePath); SetTransform(transform); }

	virtual ~ESprite() = default;

	// Create sprite
	bool CreateSprite(const EString& texturePath);

	// Set transform
	void SetTransform(const ESTransform2D transform) { m_transform = transform; }

	// Set scale to match texture
	void SetScaleToTextureSize() { m_transform.scale = glm::vec2(static_cast<float>(m_width), static_cast<float>(m_height)); }

	// Get transform
	ESTransform2D& GetTransform() { return m_transform; }

	// Render with set transform
	void Render(const TShared<EShaderProgram>& shader, ESTransform2D& transform);

	// Render with base transform
	void Render(const TShared<EShaderProgram>& shader) { Render(shader, m_transform); }

protected:
	// Store transform
	ESTransform2D m_transform;
};