#pragma once
#include "Game/GameObjects/EWorldObject.h"

class InvisibleWalls : public EWorldObject {
public:
	InvisibleWalls() = default;

protected:
	virtual void OnStart() override;

	// Skip moving the walls each frame
	virtual void OnPostTick(float deltaTime) override {}
};