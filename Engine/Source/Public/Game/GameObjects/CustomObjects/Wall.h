#pragma once
#include "Game/GameObjects/EWorldObject.h"

class Wall : public EWorldObject {
public:
	Wall() = default;

protected:
	virtual void OnStart() override;
};