#pragma once
#include "Game/GameObjects/EObject.h"
#include "Graphics/EModel.h"
#include "Graphics/ETexture.h"
#include "Graphics/ESMaterial.h"

class EMovingObject : public EObject {
public:
	EMovingObject() = default;
	~EMovingObject() = default;

	// Get the model objects transform
	ESTransform& GetTransform() { return m_transform; }

	// Rotate based on the passed rotation
	void Rotate(glm::vec3 rotation, glm::vec3 scale = glm::vec3(1.0f));

	// Translate locally based on the passed translation
	void TranslateLocal(glm::vec3 translation, glm::vec3 scale = glm::vec3(1.0f));

protected:
	// Transform for the modelObject in 3D space
	ESTransform m_transform;
};