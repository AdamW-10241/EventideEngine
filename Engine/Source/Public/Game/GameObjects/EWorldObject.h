#pragma once
#include "Game/GameObjects/EObject.h"
#include "Math/ESTransform.h"
#include "Math/ESCollision.h"

#include "Graphics/EModel.h"
#include "Graphics/ETexture.h"
#include "Graphics/ESMaterial.h"

class EWorldObject : public EObject {
public:
	EWorldObject() = default;
	virtual ~EWorldObject() = default;

	// Import a model and return a weak reference
	TWeak<EModel> ImportModel(const EString& path);

	// Get the world objects model at an index
	TWeak<EModel> GetModel(const int& index) const { return m_objectModels.at(index); }

	// Add a collision to the object
	TWeak<ESCollision> AddCollision(const ESBox& box, const bool& debug = false);

	// Get the objects transform
	ESTransform& GetTransform() { return m_transform; }

	// Run a test to see if another object is overlapping
	void TestCollision(const TShared<EWorldObject>& other);

	// Does the object have collisions
	bool HasCollisions() const { return m_objectCollisions.size() > 0; }

protected:
	virtual void OnPostTick(float deltaTime) override;

	// On collision overlap
	// Detect every frame a collision is overlapped
	// Can add a flag for enter / exit
	virtual void OnOverlap(const TShared<EWorldObject>& other, const TShared<ESCollision>& col, 
		const TShared<ESCollision>& otherCol) {}

private:
	// Transform in world space
	ESTransform m_transform;

	// Store any models attached to this object
	TArray<TShared<EModel>> m_objectModels;

	// Store the collisions for the model
	TArray<TShared<ESCollision>> m_objectCollisions;
};