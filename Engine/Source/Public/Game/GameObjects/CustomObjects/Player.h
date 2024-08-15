#pragma once
#include "Game/GameObjects/EWorldObject.h"

class Player : public EWorldObject {
public:
	Player() = default;

	void SetDefaultCamPosition(glm::vec3 position);

protected:
	virtual void OnStart() override;

	virtual void OnTick(float deltaTime) override;

	virtual void OnOverlap(const TShared<EWorldObject>& other, const TShared<ESCollision>& col,
		const TShared<ESCollision>& otherCol) override;
};