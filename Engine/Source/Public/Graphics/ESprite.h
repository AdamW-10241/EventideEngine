#pragma once
#include "Graphics/ETexture.h"
#include "Graphics/EMesh.h"
#include "Graphics/EShaderProgram.h"
#include "Math/ESTransform.h"

template<typename T>
struct ESAddSpriteConfigBase {
	EString path = "";
	EUi32 renderOrder = 0;
	glm::vec4 renderColor = glm::vec4(1.0f);
	glm::vec2 anchor = glm::vec2(0.5f);      // normalised screen position
	glm::vec2 alignment = glm::vec2(0.5f);   // 0 = top left, 0.5 = center, 1 = bottom right
	glm::vec2 sizeInUnits = glm::vec2(0.0f); // 0 = use natural size
	bool doScale = true;
	ESTransform2D transform;

	T& SetPath(EString& _path) { path = _path; return static_cast<T&>(*this); }
	T& SetRenderOrder(EUi32& _renderOrder) { renderOrder = _renderOrder; return static_cast<T&>(*this); }
	T& SetRenderColor(glm::vec4& _renderColor) { renderColor = _renderColor; return static_cast<T&>(*this); }
	T& SetAnchor(glm::vec2& _anchor) { anchor = _anchor; return static_cast<T&>(*this); }
	T& SetAlignment(glm::vec2& _alignment) { anchor = _alignment; return static_cast<T&>(*this); }
	T& SetSizeInUnits(glm::vec2& _sizeInUnits) { sizeInUnits = _sizeInUnits; return static_cast<T&>(*this); }
	T& SetDoScale(bool& _doScale) { doScale = _doScale; return static_cast<T&>(*this); }
	T& SetTransform(ESTransform2D& _transform) { transform = _transform; return static_cast<T&>(*this); }
};

struct ESAddSpriteConfig : public ESAddSpriteConfigBase<ESAddSpriteConfig> {};

class ESprite : public EMesh, public ETexture {
public:
	template<typename T>
	ESprite(const ESAddSpriteConfigBase<T>& config) {
		if (!config.path.empty()) CreateSprite(config.path);
		else CreateSprite();
		m_renderOrder = config.renderOrder;
		m_renderColor = config.renderColor;
		m_anchor = config.anchor;
		m_alignment = config.alignment;
		m_sizeInUnits = config.sizeInUnits;
		m_doScale = config.doScale;
		m_positionOffset = glm::vec2(0.0f);
		SetTransform(config.transform);
	}

	// Create sprite
	bool CreateSprite(const EString& texturePath);

	bool CreateSprite();

	// Render with set transform
	virtual void Render(const TShared<EShaderProgram>& shader, ESTransform2D& transform, bool flipVertically = false);

	// Render with base transform
	virtual void Render(const TShared<EShaderProgram>& shader, bool flipVertically = false) { Render(shader, m_transform, flipVertically); }

	// Set transform
	void SetTransform(const ESTransform2D& transform) { m_transform = transform; }

	// Update transform to match screen position
	void UpdateTransform();
	virtual void UpdateTransform(glm::vec2 windowSize);

	glm::vec2 CalcPixelSize(float slateUnit) const;

	// Get transform
	ESTransform2D& GetTransform() { return m_transform; }

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

	// Get position offset
	glm::vec2& GetPositionOffset() { return m_positionOffset; }
	// Set position offset
	void SetPositionOffset(glm::vec2 offset) { m_positionOffset = offset; }

	// Get render scale
	glm::vec2& GetRenderScale() { return m_renderScale; }

protected:
	ESTransform2D m_transform;

	glm::vec2 m_positionOffset;

	glm::vec2 m_anchor = glm::vec2(0.5f);
	glm::vec2 m_alignment = glm::vec2(0.5f);
	glm::vec2 m_sizeInUnits = glm::vec2(0.0f);

	EUi32 m_renderOrder;

	glm::vec4 m_renderColor;

	glm::vec2 m_renderScale = glm::vec2(1.0f);

	bool m_doScale;
};