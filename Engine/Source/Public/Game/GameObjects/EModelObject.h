#pragma once
#include "Game/GameObjects/EObject.h"
#include "Graphics/EModel.h"
#include "Graphics/ETexture.h"
#include "Graphics/ESMaterial.h"

class EModelObject : public EObject {
public:
	EModelObject() { m_modelTransformOffset.scale = glm::vec3(0.0f); }
	~EModelObject() = default;

	// Get the model objects transform
	ESTransform& GetTransform() { return m_transform; }

	// Rotate based on the passed rotation
	void Rotate(glm::vec3 rotation, glm::vec3 scale = glm::vec3(1.0f));

	// Translate locally based on the passed translation
	void TranslateLocal(glm::vec3 translation, glm::vec3 scale = glm::vec3(1.0f));

	// Add a model to the model object
	void AddModel(const EString& path);

	// Get the model
	TWeak<EModel>& GetModel() { return m_model; }

protected:
	// Run every frame, passes in deltaTime
	virtual void OnTick(float deltaTime) override;

protected:
	// Transform for the modelObject in 3D space
	ESTransform m_transform;

	// Offset for the model to differentiate from the transform
	ESTransform m_modelTransformOffset;

	// Store model
	TWeak<EModel> m_model;
};