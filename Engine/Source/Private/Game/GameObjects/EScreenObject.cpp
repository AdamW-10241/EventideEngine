#include "Game/GameObjects/EScreenObject.h"
#include "Graphics/EText.h"
#include "Game/GameObjects/CustomObjects/GUIButton.h"
#include "Game/GameObjects/CustomObjects/GUIHUD.h"
#include "Graphics/ESprite.h"

// External Libs
#include <algorithm>

void EScreenObject::Render(const TShared<EShaderProgram>& shader)
{
    std::sort(m_sprites.begin(), m_sprites.end(),
        [](const TShared<ESprite>& a, const TShared<ESprite>& b) {
            return a->GetRenderOrder() < b->GetRenderOrder();
    });

    for (const auto& sprite : m_sprites) {
        if (const auto& text = TCast<EText>(sprite)) {
            text->Render(shader, true);
            continue;
        }
        sprite->Render(shader);
    }
}

void EScreenObject::AddTextBindingTick(std::function<EString()> text, const int spriteIndex)
{
    // Create binding
    auto binding = [text, spriteIndex](TShared<EObject> obj, float deltaTime) {
        if (auto screenRef = TCast<EScreenObject>(obj)) {
            if (auto textRef = TCast<EText>(screenRef->GetSprite(spriteIndex).lock())) {
                // Set new text
                EString string = text();
                if (!string.empty()) textRef->SetText(string);
            }
            else EDebug::Log(LT_WARNING, "Could not create tick text binding.");
        }
    };

    // Bind event
    BIND_EVENT_SELF(GetSharedRef<EObject>(), OnTicked, binding);
}

TWeak<ESprite> EScreenObject::GetSprite(const EString& texturePath)
{
    // Return matching texture
    for (TShared<ESprite>& sprite : m_sprites) {
        if (sprite->GetPath() == texturePath) { return sprite; }
    }

    return {};
}

void EScreenObject::AttachScalingCornerButtons(const int spriteIndex)
{
	// Attach draggable corner button to each corner
	for (int i = 0; i < (int)EESpriteButtonCorner::CORNER_MAX; ++i) {
		AttachScalingCornerButton((EESpriteButtonCorner)i, spriteIndex);
	}
}

void EScreenObject::AttachScalingCornerButton(const EESpriteButtonCorner corner, const int spriteIndex)
{
	auto hud = EGameEngine::GetGameEngine()->GetGameHUD().lock();
	if (!hud) return;
	auto spr = GetSprite(spriteIndex).lock();
	if (!spr) return;

	// Create draggable button
	if (auto dragObj = hud->AddScreenObject<GUIButton>(0).lock()) {
		// Add base sprite
		ESAddSpriteConfig dragConfig;
		dragConfig.path = "Sprites/Button/Drag.png";
		dragConfig.anchor = spr->GetSpriteCornerAnchors().at((int)corner);
		dragObj->AddSprite(dragConfig);
		dragObj->SetHeldUntilReleased();

		// Add press bindings
		dragObj->SetSpritePressedColor(0, glm::vec4(0.4f, 0.4f, 0.4f, 1.0f));
		dragObj->AddPressAndReleaseScaling();

		// Move button when held and adjust button size
		BIND_EVENT_EXT(dragObj, OnHeld, GetSharedRef<EScreenObject>(), [corner, spriteIndex](const TShared<EScreenObject>& drag, const TShared<EScreenObject>& base, float deltaTime, float timeHeld) {
			auto baseSpr = base->GetSprite(spriteIndex).lock();
			if (!baseSpr) return;
			auto dragBtn = TCast<GUIButton>(drag);
			if (!dragBtn) return;
			auto dragSpr = dragBtn->GetSprite().lock();
			if (!dragSpr) return;
			auto window = EGameEngine::GetGameEngine()->GetWindow().lock();
			if (!window) return;

			dragBtn->SetSpriteAnchorMouse(); // Move to mouse

			glm::vec2 dragCenter = dragSpr->GetVisualPosition() + dragSpr->GetTransform().scale * 0.5f;
			glm::vec2 basePos = baseSpr->GetVisualPosition();
			glm::vec2 baseScale = baseSpr->GetTransform().scale;
			float slateUnit = window->GetSlateUnitY();

			// Get opposite corner to fix sprite size after dragging
			glm::vec2 oppositeCorner;
			switch ((int)corner) {
				case 0: oppositeCorner = basePos + baseScale;                     break; // Top Left		-> fix Bottom Right
				case 1: oppositeCorner = { basePos.x, basePos.y + baseScale.y };  break; // Top Right		-> fix Bottom Left
				case 2: oppositeCorner = { basePos.x + baseScale.x, basePos.y };  break; // Bottom Left		-> fix Top Right
				case 3: oppositeCorner = basePos;                                 break; // Bottom Right	-> fix Top Left
				default: oppositeCorner = basePos + baseScale;                    break; // Should be unusable
			}
			glm::vec2 newTopLeft = glm::min(dragCenter, oppositeCorner);

			// Get new size
			glm::vec2 newSize = glm::abs(dragCenter - oppositeCorner);
			glm::vec2 newSizeInUnits = newSize / slateUnit;
			const float minDragScale = 5.0f;
			newSizeInUnits.x = glm::max(newSizeInUnits.x, minDragScale);
			newSizeInUnits.y = glm::max(newSizeInUnits.y, minDragScale);
			
			// Scale sprite and adjust anchor
			baseSpr->SetSizeInUnits(newSizeInUnits);
			baseSpr->SetAnchorScreenPosition(newTopLeft);
		});

		// Adjust button to match expected screen object corner position
		BIND_EVENT_EXT(dragObj, OnUpdateTransform, GetSharedRef<EScreenObject>(), [corner, spriteIndex](const TShared<EScreenObject>& drag, const TShared<EScreenObject>& base) {
			auto baseSpr = base->GetSprite(spriteIndex).lock();
			if (!baseSpr) return;
			auto dragSpr = drag->GetSprite().lock();
			if (!dragSpr) return;
			auto window = EGameEngine::GetGameEngine()->GetWindow().lock();
			if (!window) return;

			glm::vec2 basePos = baseSpr->GetVisualPosition();
			glm::vec2 baseScale = baseSpr->GetTransform().scale;
			glm::vec2 windowSize = window->GetCurrentSize();

			// Set anchor to corner
			glm::vec2 cornerScreen;
			switch ((int)corner) {
				case 0: cornerScreen = basePos;									break; // Top Left
				case 1: cornerScreen = { basePos.x + baseScale.x, basePos.y };	break; // Top Right
				case 2: cornerScreen = { basePos.x, basePos.y + baseScale.y };	break; // Bottom Left
				case 3: cornerScreen = basePos + baseScale;						break; // Bottom Right
				default: cornerScreen = basePos;								break; // Should be unusable
			}
			dragSpr->SetAnchor(cornerScreen / windowSize);
		});
	}
}

void EScreenObject::AttachDraggableButton(const EESpriteButtonSide movementButtonSide, const int spriteIndex)
{
	auto hud = EGameEngine::GetGameEngine()->GetGameHUD().lock();
	if (!hud) return;
	auto spr = GetSprite(spriteIndex).lock();
	if (!spr) return;

	// Create draggable button
	if (auto dragObj = hud->AddScreenObject<GUIButton>(0).lock()) {
		// Add base sprite
		ESAddSpriteConfig dragConfig;
		dragConfig.path = "Sprites/Button/Drag.png";
		dragConfig.anchor = spr->GetSpriteEdgeAnchors().at((int)movementButtonSide);
		dragObj->AddSprite(dragConfig);
		dragObj->SetHeldUntilReleased();

		// Add press bindings
		dragObj->SetSpritePressedColor(0, glm::vec4(0.4f, 0.4f, 0.4f, 1.0f));
		dragObj->AddPressAndReleaseScaling();

		// Move button when held and adjust button size
		BIND_EVENT_EXT(dragObj, OnHeld, GetSharedRef<EScreenObject>(), [spriteIndex](const TShared<EScreenObject>& drag, const TShared<EScreenObject>& base, float deltaTime, float timeHeld) {
			auto baseSpr = base->GetSprite(spriteIndex).lock();
			if (!baseSpr) return;
			auto dragBtn = TCast<GUIButton>(drag);
			if (!dragBtn) return;
			auto dragSpr = dragBtn->GetSprite().lock();
			if (!dragSpr) return;
			auto window = EGameEngine::GetGameEngine()->GetWindow().lock();
			if (!window) return;

			glm::vec2 dragPos = dragSpr->GetAnchor() * window->GetCurrentSize();
			dragBtn->SetSpriteAnchorMouse(); // Move to mouse
			glm::vec2 newDragPos = dragSpr->GetAnchor() * window->GetCurrentSize();
			glm::vec2 newBasePos = baseSpr->GetVisualPosition() + (newDragPos - dragPos);
			baseSpr->SetAnchorScreenPosition(newBasePos); // Move anchor
		});

		// Adjust button to match expected screen object corner position
		BIND_EVENT_EXT(dragObj, OnUpdateTransform, GetSharedRef<EScreenObject>(), [spriteIndex, movementButtonSide](const TShared<EScreenObject>& drag, const TShared<EScreenObject>& base) {
			auto baseSpr = base->GetSprite(spriteIndex).lock();
			if (!baseSpr) return;
			auto dragSpr = drag->GetSprite().lock();
			if (!dragSpr) return;
			dragSpr->SetAnchor(baseSpr->GetSpriteEdgeAnchors().at((int)movementButtonSide));
		});
	}
}

void EScreenObject::AttachAllMovementButtons(const EESpriteButtonSide movementButtonSide, const int spriteIndex)
{
	AttachDraggableButton(movementButtonSide, spriteIndex);
	AttachScalingCornerButtons(spriteIndex);
}

void EScreenObject::AddLifetimeFadeTick(const float lifetime, const int spriteIndex)
{
	SetLifeTime(lifetime);

	// Add movement binding
	BIND_EVENT_SELF(GetSharedRef<EObject>(), OnTicked, [spriteIndex](const TShared<EObject>& obj, float deltaTime) {
		auto screenObj = TCast<EScreenObject>(obj);
		if (!screenObj) return;
		auto spr = screenObj->GetSprite(spriteIndex).lock();
		if (!spr) return;
		auto window = EGameEngine::GetGameEngine()->GetWindow().lock();
		if (!window) return;

		constexpr float FLOAT_SPEED = 15.0f;
		spr->GetPositionOffset().y += (SLATE_UNIT_SCALAR / FLOAT_SPEED) * window->GetSlateUnitY() * deltaTime;
		spr->SetRenderColorAlpha(screenObj->GetLifeTimeRatio());
	});
}
