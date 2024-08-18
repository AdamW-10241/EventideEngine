#pragma once
#include "Game/GameObjects/CustomObjects/Character.h"

class Enemy : public Character {
public:
	Enemy();

protected:
	virtual void OnStart() override;

	virtual void OnTick(float deltaTime) override;

	virtual void OnPostTick(float deltaTime) override;

	virtual void OnTakeDamage(float damage) override;
};