#include "Game/GameObjects/EPteraObject.h"

#define Super EModelObject

EPteraObject::EPteraObject()
{
	m_modelScale = 1.0f; 
	m_minModelScale = 2.5f;
	m_maxModelScale = 15.0f;

	m_moveSpeed = 1.0f;
	m_maxMoveSpeed = 10.0f;

	m_changeSize = false; 
	m_moveMulti = 1.0f;
}

void EPteraObject::OnStart()
{
	// --------- PTERA
	AddModel("Models/Ptera/scene.gltf");

	m_modelScale = EGameEngine::GetGameEngine()->GetRandomFloatRange(m_minModelScale, m_maxModelScale / 2.0f);
	m_transform.scale = glm::vec3(m_modelScale);
	m_transform.position.x = -5.0f;
	m_transform.position.z = -5.0f;
	m_transform.position.y = 5.0f;

	// Ptera base colour
	TShared<ETexture> pteraTex1 = TMakeShared<ETexture>();
	pteraTex1->LoadTexture("Ptera Base Colour", "Models/Ptera/textures/material0_baseColor.png");

	// Ptera normal
	TShared<ETexture> pteraNormal1 = TMakeShared<ETexture>();
	pteraNormal1->LoadTexture("Ptera Normal", "Models/Ptera/textures/material0_normal.png");

	// Creating materials
	TShared<ESMaterial> pteraMat1 = EGameEngine::GetGameEngine()->CreateMaterial();

	// Assigning the texture to the base colour map for the material
	pteraMat1->m_baseColourMap = pteraTex1;
	pteraMat1->m_normalMap = pteraNormal1;

	// Setting the material to slots in the model
	m_model.lock()->SetMaterialBySlot(0, pteraMat1);

	// Get random move speed
	m_moveSpeed = EGameEngine::GetGameEngine()->GetRandomFloatRange(-m_maxMoveSpeed, m_maxMoveSpeed);

	// Rotate offset to make model allign with movement
	// Flip forward or backward based on positive or negative movement speed
	m_modelTransformOffset.rotation.y = (m_moveSpeed <= 0.0f) ? 90.0f : -90.0f;

	// 1/4 chance to change size
	m_changeSize = EGameEngine::GetGameEngine()->GetRandomIntRange(0, 1) == 0;

	Super::OnStart();
}

void EPteraObject::OnTick(float deltaTime)
{
	// Oscillate size if flag set
	if (m_changeSize) {
		// Flip multi if outside range
		if (m_modelScale < m_minModelScale) {
			m_modelScale = m_minModelScale;
			m_moveMulti *= -1;
		}
		if (m_modelScale > m_maxModelScale) {
			m_modelScale = m_maxModelScale;
			m_moveMulti *= -1;
		}

		// Oscillate scale with time and multi
		m_modelScale -= deltaTime * m_moveMulti;

		// Apply scale the the transform
		m_transform.scale = glm::vec3(m_modelScale);
		EDebug::Log(m_modelScale);
	}
	
	// Rotate to the left
	Rotate(glm::vec3(0.0f, deltaTime, 0.0f), glm::vec3(m_moveSpeed * 10.0f));
	
	// Move forward
	TranslateLocal(glm::vec3(deltaTime, 0.0f, 0.0f), glm::vec3(m_moveSpeed * 2.0f));

	Super::OnTick(deltaTime);
}
