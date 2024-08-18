#include "Game/GameObjects/ELightObject.h"
#include "Game/EGameEngine.h"
#include "Graphics/EGraphicsEngine.h"

#define Super EWorldObject

void ELightObject::AddPointLight(glm::vec3 colour, float intensity, glm::vec3 position, float linear, float quadratic)
{
	// Create point 
	m_pointLight = EGameEngine::GetGameEngine()->GetGraphicsEngine()->CreatePointLight();
	GetTransform().position = position;

	// Copy point light values
	if (const auto& lightRef = m_pointLight.lock()) {
		lightRef->colour = colour;
		lightRef->intensity = intensity;
		lightRef->position = position;
		lightRef->linear = linear;
		lightRef->quadratic = quadratic;
	}
}

void ELightObject::OnTick(float deltaTime)
{
	// Move light if light exists
	if (m_pointLight.lock()) {
		m_pointLight.lock()->position = GetTransform().position + m_lightTransformOffset.position;
	}

	Super::OnTick(deltaTime);
}

// DEBUG Rotate light
//static float lightTimer = 0.0f;
//const float timeToRotate = 1.0f;
//lightTimer += EGameEngine::GetGameEngine()->DeltaTimeF();
//
//if (lightTimer > timeToRotate)
//lightTimer = 0.0f;
//
//if (const auto& lightRef = std::dynamic_pointer_cast<ESPointLight>(m_lights[1])) {
//	lightRef->position.x = sin(lightTimer / timeToRotate * 2 * PI) * 2.0f;
//	lightRef->position.z = cos(lightTimer / timeToRotate * 2 * PI) * 2.0f;
//}
