#pragma once
#include "Game/GameObjects/EMovingObject.h"
#include "Graphics/EModel.h"
#include "Graphics/ETexture.h"
#include "Graphics/ESMaterial.h"

class EModelObject : public EMovingObject {
public:
	EModelObject() { m_modelTransformOffset.scale = glm::vec3(0.0f); }
	~EModelObject() = default;

	// Add a model to the model object
	void AddModel(const EString& path);

	// Get the model
	TWeak<EModel>& GetModel() { return m_model; }

protected:
	// Run every frame, passes in deltaTime
	virtual void OnTick(float deltaTime) override;

protected:
	// Offset for the model to differentiate from the transform
	ESTransform m_modelTransformOffset;

	// Store model
	TWeak<EModel> m_model;
};