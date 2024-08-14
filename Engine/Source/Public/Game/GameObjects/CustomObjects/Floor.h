#pragma once
#include "Game/GameObjects/EWorldObject.h"

class Floor : public EWorldObject {
public:
	Floor();

protected:
	virtual void OnStart() override;
};
