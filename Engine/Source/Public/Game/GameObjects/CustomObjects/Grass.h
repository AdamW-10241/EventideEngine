#pragma once
#include "Game/GameObjects/EWorldObject.h"

class Floor;

class Grass : public EWorldObject {
public:
	Grass() = default;

	// Place the grass on a random vertex of the mesh
	void PlaceOnFloorRandomly(TShared<Floor> floor, float placementScale);

protected:
	virtual void OnStart() override;

};