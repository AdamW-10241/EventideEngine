#pragma once
#include "Game/GameObjects/EWorldObject.h"

class Helmet : public EWorldObject {
public:
	Helmet() = default;

protected:
	virtual void OnStart() override;

	virtual void OnTick(float deltaTime) override;

	virtual void OnOverlap(const TShared<EWorldObject>& other, const TShared<ESCollision>& col, 
		const TShared<ESCollision>& otherCol) override;
};