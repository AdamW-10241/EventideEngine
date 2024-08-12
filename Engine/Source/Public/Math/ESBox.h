#pragma once

// External Libs
#include <GLM/vec3.hpp>
#include <GLM/geometric.hpp>

struct ESBox {
	ESBox() {
		position = glm::vec3(0.0f);
		halfSize = glm::vec3(0.0f);
	}

	ESBox(glm::vec3 position, glm::vec3 halfSize) :
		position(position), halfSize(halfSize) {}

	// Detect if two boxes are overlapping
	static bool BoxOverlap(const ESBox& box1, const ESBox& box2) {
		// Ensure that a zero size always returns false
		if (glm::length(box1.halfSize) == 0.0f ||
			glm::length(box2.halfSize) == 0.0f)
			return false;
		
		// Detect if x is in bounds
		if (box1.GetMin().x > box2.GetMax().x ||
			box1.GetMax().x < box2.GetMin().x)
			return false;

		// Detect if y is in bounds
		if (box1.GetMin().y > box2.GetMax().y ||
			box1.GetMax().y < box2.GetMin().y)
			return false;

		// Detect if z is in bounds
		if (box1.GetMin().z > box2.GetMax().z ||
			box1.GetMax().z < box2.GetMin().z)
			return false;

		return true;
	}

	glm::vec3 GetMax() const {
		return position + halfSize;
	}

	glm::vec3 GetMin() const {
		return position - halfSize;
	}

	// Position in world space
	glm::vec3 position;
	// Size of the box, will start from center so half size
	glm::vec3 halfSize;
};