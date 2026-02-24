#include "Game/GameObjects/EWorldObject.h"
#include "Graphics/EGraphicsEngine.h"

#include "Game/GameObjects/CustomObjects/Floor.h"

#define Super EObject

TWeak<EModel> EWorldObject::ImportModel(const EString& modelPath, const TArray<ESMaterialSlot>& materials)
{
    // Import model
    auto modelRef = EGameEngine::GetGameEngine()->GetGraphicsEngine()->ImportModel(modelPath);
    if (!modelRef->HasMaterials()) return modelRef;

    // Assign materials once per model
    bool isDefaultTexture = modelRef->GetMaterials().at(0)->m_baseColourMap->GetImportPath() == "Textures/T_DefaultGrid.png";
    if (isDefaultTexture) {
        // Get static cache
        static std::unordered_map<EString, TShared<ETexture>> textureCache;
        static std::unordered_map<EString, TShared<ESMaterial>> materialCache;

        // Apply materials
        for (const auto& slot : materials) {
            const ETexturePaths& paths = slot.desc.paths;

            // Create texture if not in cache
            if (!textureCache.count(paths.base)) {
                auto texBase = TMakeShared<ETexture>();
                texBase->LoadTexture(modelPath, paths.base);
                textureCache[paths.base] = texBase;

                if (!paths.normal.empty()) {
                    auto texNormal = TMakeShared<ETexture>();
                    texNormal->LoadTexture(modelPath, paths.normal);
                    textureCache[paths.normal] = texNormal;
                }

                if (!paths.specular.empty()) {
                    auto texSpecular = TMakeShared<ETexture>();
                    texSpecular->LoadTexture(modelPath, paths.specular);
                    textureCache[paths.specular] = texSpecular;
                }
            }

            // Create material if not in cache
            if (!materialCache.count(paths.base)) {
                auto mat = EGameEngine::GetGameEngine()->CreateMaterial();
                mat->m_baseColourMap = textureCache[paths.base];
                if (!paths.normal.empty()) mat->m_normalMap = textureCache[paths.normal];
                if (!paths.specular.empty()) mat->m_specularMap = textureCache[paths.specular];
                mat->m_brightness = slot.desc.m_brightness;
                mat->m_shininess = slot.desc.m_shininess;
                mat->m_specularStrength = slot.desc.m_specularStrength;
                mat->m_textureDepth = slot.desc.m_textureDepth;
                materialCache[paths.base] = mat;
            }

            // Add material to the model at each slot
            for (int index : slot.slotIndices) {
                modelRef->SetMaterialBySlot(index, materialCache[paths.base]);
            }
        }
    }

    // Cache and return
    m_objectModels.push_back(modelRef);
    return modelRef;
}

TWeak<EModel> EWorldObject::LoadModel(const EString& modelPath, const TArray<ESMaterialSlot>& materials)
{
    auto& models = EGameEngine::GetGameEngine()->GetGraphicsEngine()->GetModels();

    // Find cached model
    for (const auto& modelRef : models) {
        if (modelRef->GetPath().compare(modelPath) == 0) {
            m_objectModels.push_back(modelRef);
            return modelRef;
        }
    }

    // Else import if not cached
    return ImportModel(modelPath, materials);
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
    if (auto model = floor->GetModel(0).lock()) {
        GetTransform().position = floor->GetTransform().position + (model->GetMesh(0)->GetRandomVertexPosition() * placementScale);
    }
}

void EWorldObject::OnPostTick(float deltaTime)
{
    Super::OnPostTick(deltaTime);

    // All collisions will follow the world object
    for (const auto& colRef : m_objectCollisions) {
        colRef->box.position = GetTransform().position;
    }
}
