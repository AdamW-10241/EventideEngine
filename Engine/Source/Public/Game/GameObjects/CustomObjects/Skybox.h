#pragma once
#include "Game/GameObjects/EWorldObject.h"

class Skybox : public EWorldObject {
public:
	Skybox() = default;

protected:
	virtual void OnStart() override;
};