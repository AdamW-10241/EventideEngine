#include "Game/GameObjects/CustomObjects/GUIHUD.h"

void GUIHUD::OnTick(float deltaTime)
{
    // Get window
    auto window = EGameEngine::GetGameEngine()->GetWindow().lock();
    if (!window) {
        EDebug::Log("Window could not be locked.", LT_ERROR);
        return;
    }
    glm::vec2 windowSize = window->GetCurrentSize();

    // Iterate screen objects
    for (auto obj : m_screenObjects) {
        if (auto objRef = obj.lock()) {
            // Reposition sprites
            for (auto sprite : objRef->GetSprites()) {
                if (auto spriteRef = sprite.lock()) {
                    spriteRef->UpdateTransform(windowSize);
                }
            }
        }
    }
}
