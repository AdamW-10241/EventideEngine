#include "Game/GameObjects/CustomObjects/GUIHUD.h"
#include "Game/GameObjects/CustomObjects/GUIButton.h"

void GUIHUD::OnStart()
{
	// Points text
	if (auto pointsObj = AddScreenObject<EScreenObject>(1).lock()) {
		// Add text
		ESAddTextConfig config;
		config.path = FONT_PRESS_START;
		config.renderColor = { 1.0f, 1.0f, 0.0f, 1.0f };
		config.anchor = { 0.08f, 0.15f };
		config.alignment = { 0.0f, 0.0f };
		pointsObj->AddSprite(config);

		// Add text binding for tick
		pointsObj->AddTextBindingTick([] {
			// Set text
			return EString("Points - " + toEString(EGameEngine::GetGameEngine()->GetPoints()));
			});
	}

	// FPS text
	if (auto fpsObj = AddScreenObject<EScreenObject>(1).lock()) {
		// Add text
		ESAddTextConfig config;
		config.path = FONT_PRESS_START;
		config.renderColor = { 1.0f, 1.0f, 1.0f, 1.0f };
		config.anchor = { 0.08f, 0.9f };
		config.alignment = { 0.0f, 0.0f };
		fpsObj->AddSprite(config);
		fpsObj->AttachDraggableButton(EESpriteButtonSide::BOTTOM);

		// Add text binding for tick
		fpsObj->AddTextBindingTick([] {
			return EString("FPS - " + toEString((int)floor(1 / EGameEngine::GetGameEngine()->DeltaTime())));
		});
	}

	// Quit button
	if (auto buttonObj = AddScreenObject<GUIButton>(1).lock()) {
		glm::vec2 anchor{ 0.8f, 0.1f };
		// Add base sprite
		ESAddSpriteConfig spriteConfig;
		spriteConfig.path = "Sprites/Button/QuitButton.png";
		spriteConfig.anchor = anchor;
		spriteConfig.sizeInUnits = glm::vec2(230.0f, 0.0f);
		buttonObj->AddSprite(spriteConfig);

		// Add bindings
		buttonObj->SetSpritePressedColor(0, glm::vec4(0.4f, 0.4f, 0.4f, 1.0f));
		buttonObj->AddPressAndReleaseScaling();
		BIND_EVENT_RAW(buttonObj, OnReleased, []() {
			EGameEngine::QuitGame();
		});
	}

	// Points text
	if (auto pausedObj = AddScreenObject<EScreenObject>(1).lock()) {
		// Add text
		ESAddTextConfig config;
		config.text = "PAUSED";
		config.fontSize = 50;
		config.anchor = { 0.5f, 0.4f };
		pausedObj->AddSprite(config);
		BIND_EVENT_SELF(pausedObj, OnUpdateTransform, [](const TShared<EScreenObject>& obj) {
			float alpha = (EGameEngine::GetGameEngine()->GetGameState() == EEGameState::PAUSED) ? 1.0f : 0.0f;
			obj->SetSpritesRenderColors({1.0f, 1.0f, 1.0f, alpha });
		});
	}

	// TEST - Scalable sprite
	if (auto scalableObj = AddScreenObject<GUIButton>(2).lock()) {
		glm::vec2 anchor{ 0.8f, 0.2f };
		// Add base sprite
		ESAddSpriteConfig baseConfig;
		baseConfig.path = "Sprites/Button/PlayButton.png";
		baseConfig.anchor = anchor;
		baseConfig.sizeInUnits = glm::vec2(230.0f, 0.0f);
		auto baseSpr = scalableObj->AddSprite(baseConfig).lock();
		if (!baseSpr) return;
		scalableObj->SetSpritePressedColor(0, glm::vec4(0.4f, 0.4f, 0.4f, 1.0f));
		scalableObj->AddPressAndReleaseScaling();
		scalableObj->AttachScalingCornerButtons();
		scalableObj->AttachDraggableButton(EESpriteButtonSide::LEFT);
	}
}

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
			if (objRef->OnUpdateTransform) objRef->OnUpdateTransform();
        }
    }
}
