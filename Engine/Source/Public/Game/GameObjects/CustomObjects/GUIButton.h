#pragma once
#include "Game/GameObjects/EScreenObject.h"

class GUIButton : public EScreenObject {
public:
	using EScreenObject::EScreenObject;

protected:
	virtual void OnRegisterInputs(const TShared<EInput>& m_input) override;

	virtual void OnButtonPressed() {}

private:
	const bool IsMouseOnButton(const TShared<EInput>& m_input);

	//if (const auto& window = EGameEngine::GetGameEngine()->GetWindow().lock()) {
	//	const auto& sprite = AddSprite("Sprites/Test/Tree.png", window->GetWindowCenter(), 0);
	//	if (const auto& spriteRef = sprite.lock()) {
	//		spriteRef->GetTransform().scale *= 2.0f;
	//		spriteRef->GetTransform().CenterOnPosition();
	//	}
	//}
};