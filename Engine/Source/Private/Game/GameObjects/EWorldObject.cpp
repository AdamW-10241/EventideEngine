#include "Game/GameObjects/EWorldObject.h"
#include "Graphics/EGraphicsEngine.h"

#define Super EObject

TWeak<EModel> EWorldObject::ImportModel(const EString& path)
{
    if (const auto& modelRef = EGameEngine::GetGameEngine()->GetGraphicsEngine()->ImportModel(path)) {
        m_objectModels.push_back(modelRef);
        modelRef->GetTransform() = m_transform;
        return modelRef;
    }
    
    return {};
}

TWeak<ESCollision> EWorldObject::AddCollision(const ESBox& box, const bool& debug)
{
    // Create the collision
    const TShared<ESCollision>& newCol = TMakeShared<ESCollision>();
    // Set the position and size
    newCol->box = box;

    // Add a debug collision mesh if debugging
    if (debug)
        EGameEngine::GetGameEngine()->GetGraphicsEngine()->CreateCollisionMesh(newCol);
    
    // Add the collision the the array
    m_objectCollisions.push_back(newCol);
    // Return a weak version
    return newCol;
}

void EWorldObject::TestCollision(const TShared<EWorldObject>& other)
{
    // Looping through this objects collisions
    for (const auto& col : m_objectCollisions) {
        // Loop through the other objects collisions for each of this objects collisions
        for (const auto& otherCol : other->m_objectCollisions) {
            // Check for overlap between collisions
            if (ESCollision::IsOverlapping(*col, *otherCol)) {
                OnOverlap(other, col, otherCol);
            }
        }
    }
}

void EWorldObject::OnPostTick(float deltaTime)
{
    Super::OnPostTick(deltaTime);

    // All models will follow the world object
    for (const auto& modelRef : m_objectModels) {
        modelRef->GetTransform() = GetTransform();
    }

    // All collisions will follow the world object
    for (const auto& colRef : m_objectCollisions) {
        colRef->box.position = GetTransform().position;
    }
}
