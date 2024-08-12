#pragma once
#include "Math/ESBox.h"
#include "EngineTypes.h"

enum class EECollisionType {
	ALL,
	COLLECTABLE,
	PLAYER,
	ENEMY
};

struct ESCollision {
	ESCollision() {
		box.position = glm::vec3(0.0f);
		box.halfSize = glm::vec3(0.0f);
		type = EECollisionType::ALL;
		tag = "";
	} 

	// Determine if two collisions are overlapping
	static bool IsOverlapping(const ESCollision& col1, const ESCollision& col2) {
		return ESBox::BoxOverlap(col1.box, col2.box);
	}

	// Collision type
	EECollisionType type;

	// Custom tag for detection
	EString tag;

	// Box bounds
	ESBox box;

};