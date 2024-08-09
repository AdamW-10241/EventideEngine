#include "Game/GameObjects/EModelObject.h"
#include "Graphics/EGraphicsEngine.h"

#define Super EObject

void EModelObject::Rotate(glm::vec3 rotation, glm::vec3 scale)
{
	if (glm::length(rotation) != 0.0f)
		rotation = glm::normalize(rotation);

	m_transform.rotation += rotation * scale * EGameEngine::GetGameEngine()->DeltaTimeF();

	if (m_transform.rotation.x < -89.0f)
		m_transform.rotation.x = -89.0f;

	if (m_transform.rotation.x > 89.0f)
		m_transform.rotation.x = 89.0f;
}

void EModelObject::TranslateLocal(glm::vec3 translation, glm::vec3 scale)
{
	// Move the input direction forward if required
	glm::vec3 moveDir = m_transform.Forward() * translation.z;
	moveDir += m_transform.Right() * translation.x;
	moveDir.y += translation.y;

	if (glm::length(moveDir) != 0.0f)
		moveDir = glm::normalize(moveDir);

	m_transform.position += moveDir * scale * EGameEngine::GetGameEngine()->DeltaTimeF();
}

void EModelObject::AddModel(const EString& path)
{
	m_model = EGameEngine::GetGameEngine()->ImportModel(path);
}

void EModelObject::OnTick(float deltaTime)
{
	m_model.lock()->GetTransform() = m_transform + m_modelTransformOffset;

	Super::OnTick(deltaTime);
}
