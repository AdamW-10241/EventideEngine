#pragma once
#include "Game/GameObjects/EWorldObject.h"

class EMesh;

class Grass : public EWorldObject {
public:
	Grass() = default;

	// Place the grass on a random vertex of the mesh
	void PlaceOnMeshRandomly(TUnique<EMesh>& meshToPlaceOn);

protected:
	virtual void OnStart() override;

};