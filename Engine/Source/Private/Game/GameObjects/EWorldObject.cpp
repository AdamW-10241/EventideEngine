#include "Game/GameObjects/EWorldObject.h"
#include "Graphics/EGraphicsEngine.h"
#include "Graphics/EModel.h"

#define Super EObject

TWeak<EModel> EWorldObject::ImportModel(const EString& path)
{
    if (const auto& modelRef = EGameEngine::GetGameEngine()->GetGraphicsEngine()->ImportModel(path)) {
        m_objectModels.push_back(modelRef);
        return modelRef;
    }
    
    return {};
}

void EWorldObject::OnPostTick(float deltaTime)
{
    Super::OnPostTick(deltaTime);

    for (const auto& modelRef : m_objectModels) {
        modelRef->GetTransform() = GetTransform();
    }
}
