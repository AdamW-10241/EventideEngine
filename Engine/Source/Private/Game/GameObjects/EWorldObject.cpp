#include "Game/GameObjects/EWorldObject.h"
#include "Graphics/EGraphicsEngine.h"

#include "Game/GameObjects/CustomObjects/Floor.h"

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
    // Create the collisionm, also set the position and size
    const TShared<ESCollision>& newCol = TMakeShared<ESCollision>(box);

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

void EWorldObject::Rotate(float deltaTime, glm::vec3 rotation, glm::vec3 scale)
{
    GetTransform().rotation += rotation * scale * deltaTime;

    if (m_transform.rotation.x < -89.0f)
        m_transform.rotation.x = -89.0f;

    if (m_transform.rotation.x > 89.0f)
        m_transform.rotation.x = 89.0f;
}

void EWorldObject::TranslateLocal(float deltaTime, glm::vec3 translation, glm::vec3 scale)
{
    // Move the input direction forward if required
    glm::vec3 moveDir = m_transform.Forward() * translation.z;
    moveDir += m_transform.Right() * translation.x;
    moveDir.y += translation.y;

    if (glm::length(moveDir) != 0.0f)
        moveDir = glm::normalize(moveDir);

    m_transform.position += moveDir * scale * deltaTime;
}

void EWorldObject::PlaceOnFloorRandomly(TShared<Floor> floor, float placementScale)
{
    // Set the position to a random vertex position on the mesh
    GetTransform().position = floor->GetTransform().position +
        (floor->GetModel(0).lock()->GetMesh(0)->GetRandomVertexPosition() * placementScale);
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
