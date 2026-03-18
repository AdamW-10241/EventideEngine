#pragma once
#include "Graphics/ETexture.h"
#include "Graphics/EMesh.h"
#include "Graphics/EShaderProgram.h"
#include "Math/ESTransform.h"

#define FONT_PRESS_START "Fonts/Press_Start_2P/PressStart2P-Regular.ttf"

struct ESAddSpriteConfig {
	EString texturePath = "";
	glm::vec2 screenPositionRatio = glm::vec2(0.5f);
	glm::vec2 m_positionOffset = glm::vec2(0.0f);
	ESTransform2D transform;

	EUi32 renderOrder = 0;
	bool isText = false;
	glm::vec4 renderColor = glm::vec4(1.0f);

	ESAddSpriteConfig SetRenderColor(glm::vec4 _renderColor) { renderColor = _renderColor; return *this; }
	ESAddSpriteConfig SetIsText(bool _isText) { isText = _isText; return *this; }
};

class ESprite : public EMesh, public ETexture {
public:
	ESprite(const ESAddSpriteConfig config) {
		if (!config.isText) {
			// ESprite — load texture as usual
			if (!config.texturePath.empty()) CreateSprite(config.texturePath);
			else CreateSprite();
		}
		else {
			// EText — load only mesh
			CreateSprite();
		}
		m_renderOrder = config.renderOrder;
		m_renderColor = config.renderColor;
		m_screenPositionRatio = config.screenPositionRatio;
		m_positionOffset = config.m_positionOffset;

		ESTransform2D offsetTransform = config.transform;
		offsetTransform.position += config.m_positionOffset;
		SetTransform(offsetTransform);
	}

	// Create sprite
	bool CreateSprite(const EString& texturePath);

	bool CreateSprite();

	// Render with set transform
	virtual void Render(const TShared<EShaderProgram>& shader, ESTransform2D& transform, bool flipVertically = false);

	// Render with base transform
	virtual void Render(const TShared<EShaderProgram>& shader, bool flipVertically = false) { Render(shader, m_transform, flipVertically); }

	// Set transform
	void SetTransform(const ESTransform2D transform) { m_transform = transform; }

	// Update transform to match screen position
	void UpdateTransformScreenPosition(glm::vec2 windowSize);
	void UpdateTransformScreenPosition();
	// Virtual updating transform to match screen position
	virtual void OnUpdateTransformScreenPosition() { 
		GetTransform().CenterOnPosition(); 
	}

	// Get transform
	ESTransform2D& GetTransform() { return m_transform; }

	// Set scale to match texture
	void SetScaleToTextureSize() { m_transform.scale = glm::vec2((float)m_width, (float)m_height); }

	// Set render order
	void SetRenderOrder(const EUi32 renderOrder) { m_renderOrder = renderOrder; }
	// Get render order
	EUi32& GetRenderOrder() { return m_renderOrder; }

	// Set render color
	void SetRenderColor(const glm::vec4 renderColor) { m_renderColor = renderColor; }
	void SetRenderColorAlpha(const float renderAlpha) { m_renderColor.a = renderAlpha; }
	// Get render color
	glm::vec4& GetRenderColor() { return m_renderColor; }
	float& GetRenderAlpha() { return m_renderColor.a; }

	// Set render scale
	void SetRenderScale(const glm::vec2 renderScale) { m_renderScale = renderScale; }
	void SetRenderScale(const float renderScale) { m_renderScale = glm::vec2(renderScale); }
	
	// Get screen position ratio
	glm::vec2& GetScreenPositionRatio() { return m_screenPositionRatio; }
	// Set screen position ratio
	void GetScreenPositionRatio(glm::vec2 ratio) { m_screenPositionRatio = ratio; }

	// Get position offset
	glm::vec2& GetPositionOffset() { return m_positionOffset; }
	// Set position offset
	void GetPositionOffset(glm::vec2 offset) { m_positionOffset = offset; }

	// Get render scale
	glm::vec2& GetRenderScale() { return m_renderScale; }

protected:
	ESTransform2D m_transform;

	glm::vec2 m_positionOffset;

	// Screen position ratio for drawing
	// 0..1 ( 0.5 is center)
	glm::vec2 m_screenPositionRatio;

	EUi32 m_renderOrder;

	glm::vec4 m_renderColor;

	glm::vec2 m_renderScale = glm::vec2(1.0f);
};