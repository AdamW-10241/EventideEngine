#pragma once
#include "Game/GameObjects/EWorldObject.h"

class Floor;

class Grass : public EWorldObject {
public:
	Grass() = default;

protected:
	virtual void OnStart() override;

};