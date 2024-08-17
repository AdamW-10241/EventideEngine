#pragma once
#include "Math/ESBox.h"
#include "EngineTypes.h"

enum class EECollisionType {
	ALL,
	COLLECTABLE,
	PLAYER,
	ENEMY
};

class EMesh;

struct ESCollision {
	ESCollision() {
		box.position = glm::vec3(0.0f);
		box.halfSize = glm::vec3(0.0f);
		debugColour = glm::vec3(0.0f, 1.0f, 0.0f);
		type = EECollisionType::ALL;
		tag = "";
	}

	ESCollision(const ESBox& newBox) {
		box.position = newBox.position;
		box.halfSize = newBox.halfSize;
		debugColour = glm::vec3(0.0f, 1.0f, 0.0f);
		type = EECollisionType::ALL;
		tag = "";
	}

	// Determine if two collisions are overlapping
	static bool IsOverlapping(const ESCollision& col1, const ESCollision& col2) {
		return ESBox::BoxOverlap(col1.box, col2.box);
	}

	// Store the debug mesh for the collision
	TShared<EMesh> debugMesh;

	// Colour of the debug wireframe;
	glm::vec3 debugColour;

	// Collision type
	EECollisionType type;

	// Custom tag for detection
	EString tag;

	// Box bounds
	ESBox box;

};