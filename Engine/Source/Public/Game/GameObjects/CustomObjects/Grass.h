#pragma once
#include "Game/GameObjects/EWorldObject.h"

class Grass : public EWorldObject {
public:
	Grass(TWeak<EModel>& modelToPlaceOn);

protected:
	virtual void OnStart() override;

private:
	void PositionRandomlyOnMesh(TWeak<EModel>& modelToPlaceOn);

};