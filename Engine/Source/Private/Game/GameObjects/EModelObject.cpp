#include "Game/GameObjects/EModelObject.h"
#include "Graphics/EGraphicsEngine.h"

#define Super EMovingObject

void EModelObject::AddModel(const EString& path)
{
	m_model = EGameEngine::GetGameEngine()->ImportModel(path);
}

void EModelObject::OnTick(float deltaTime)
{
	m_model.lock()->GetTransform() = m_transform + m_modelTransformOffset;

	Super::OnTick(deltaTime);
}
