#include "Game/GameObjects/ELightObject.h"
#include "Game/EGameEngine.h"
#include "Graphics/EGraphicsEngine.h"

#define Super EMovingObject

void ELightObject::AddPointLight(glm::vec3 colour, float intensity, glm::vec3 position, float linear, float quadratic)
{
	// Create point 
	m_pointLight = EGameEngine::GetGameEngine()->GetGraphicsEngine().lock()->CreatePointLight();

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
	m_pointLight.lock()->position = m_transform.position + m_lightTransformOffset.position;

	Super::OnTick(deltaTime);
}
