#pragma once
#include "Game/GameObjects/EWorldObject.h"

class Coin : public EWorldObject {
public:
	Coin() = default;

protected:
	virtual void OnStart();

	virtual void OnTick(float deltaTime);

	virtual void OnOverlap(const TShared<EWorldObject>& other, const TShared<ESCollision>& col,
		const TShared<ESCollision>& otherCol) override;

private:
	// Store the coins point value
	float m_points = 1.0f;
};