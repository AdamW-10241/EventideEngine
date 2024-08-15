#pragma once
#include "Game/GameObjects/EWorldObject.h"

class Player : public EWorldObject {
public:
	Player() = default;

protected:
	virtual void OnStart() override;

	virtual void OnTick(float deltaTime) override;
};