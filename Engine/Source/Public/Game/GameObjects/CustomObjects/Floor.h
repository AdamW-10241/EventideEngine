#pragma once
#include "Game/GameObjects/EWorldObject.h"

class Floor : public EWorldObject {
public:
	Floor(EUi32 grassToSpawn = 0) { m_grassToSpawn = grassToSpawn; }

protected:
	virtual void OnStart() override;

private:
	// Spawn grass objects on the floor
	void SpawnGrass();

private:
	// Store the amount of grass to spawn
	EUi32 m_grassToSpawn;
};