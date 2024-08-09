#pragma once
#include "Game/GameObjects/EMovingObject.h"
#include "Graphics/ESLight.h"

class ELightObject : public EMovingObject {
public:
	ELightObject() = default;
	~ELightObject() = default;

	// Add a light to the light object
	void AddPointLight(glm::vec3 colour = glm::vec3(0.0f), float intensity = 1.0f, glm::vec3 position = glm::vec3(0.0f),
		float linear = 0.045f, float quadratic = 0.0075f);

	// Get the light
	TWeak<ESPointLight>& GetPointLight() { return m_pointLight; }

protected:
	// Run every frame, passes in deltaTime
	virtual void OnTick(float deltaTime);

protected:
	// Offset for the light to differentiate from the transform
	ESTransform m_lightTransformOffset;

	// Store light
	TWeak<ESPointLight> m_pointLight;
};