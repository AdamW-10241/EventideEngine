#pragma once
#include "Graphics/ETexture.h"
#include "Graphics/EMesh.h"
#include "Graphics/EShaderProgram.h"
#include "Math/ESTransform.h"

class ESprite : public EMesh, public ETexture {
public:
	ESprite() { 
		m_transform = ESTransform2D(); 
		m_renderOrder = 0; 
		m_renderColor = glm::vec4(1.0f); }

	ESprite(const ESTransform2D transform, const EUi32 renderOrder, const glm::vec4 renderColor = glm::vec4(1.0f)) {
		CreateSprite();
		SetTransform(transform);
		m_renderOrder = renderOrder;
		m_renderColor = renderColor;
	}

	ESprite(const EString& texturePath, const ESTransform2D transform, const EUi32 renderOrder, const glm::vec4 renderColor = glm::vec4(1.0f)) {
		CreateSprite(texturePath); 
		SetTransform(transform);
		m_renderOrder = renderOrder;
		m_renderColor = renderColor;
	}

	virtual ~ESprite() = default;

	// Create sprite
	bool CreateSprite(const EString& texturePath);

	bool CreateSprite();

	// Set transform
	void SetTransform(const ESTransform2D transform) { m_transform = transform; }

	// Get transform
	ESTransform2D& GetTransform() { return m_transform; }

	// Set render order
	void SetRenderOrder(const EUi32 renderOrder) { m_renderOrder = renderOrder; }

	// Get render order
	EUi32& GetRenderOrder() { return m_renderOrder; }

	// Set scale to match texture
	void SetScaleToTextureSize() { m_transform.scale = glm::vec2(static_cast<float>(m_width), static_cast<float>(m_height)); }

	// Render with set transform
	void Render(const TShared<EShaderProgram>& shader, ESTransform2D& transform);

	// Render with base transform
	void Render(const TShared<EShaderProgram>& shader) { Render(shader, m_transform); }

protected:
	// Store transform
	ESTransform2D m_transform;

	// Store render order
	EUi32 m_renderOrder;

	// Store render color
	glm::vec4 m_renderColor;
};