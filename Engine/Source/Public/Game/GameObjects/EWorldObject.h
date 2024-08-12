#pragma once
#include "Game/GameObjects/EObject.h"
#include "Math/ESTransform.h"

class EModel;

class EWorldObject : public EObject {
public:
	EWorldObject() = default;
	virtual ~EWorldObject() = default;

	// Import a model and return a weak reference
	TWeak<EModel> ImportModel(const EString& path);

	// Get the objects transform
	ESTransform& GetTransform() { return m_transform; }

protected:
	virtual void OnPostTick(float deltaTime) override;

private:
	// Transform in world space
	ESTransform m_transform;

	// Store any models attached to this object
	TArray<TShared<EModel>> m_objectModels;
};