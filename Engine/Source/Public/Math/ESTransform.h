#pragma once

// External Libs
#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>

struct ESTransform {
	ESTransform() {
		position = glm::vec3(0.0f);
		rotation = glm::vec3(0.0f);
		scale = glm::vec3(1.0f);
	}

	// Get the forward vector of the local rotation
	glm::vec3 Forward() {
		glm::vec3 forward = glm::vec3(0.0f);

		// Get the forward x value by * sin of y by cos of x
		forward.x = sin(glm::radians(rotation.y)) * cos(glm::radians(rotation.x));
		// Get the forward y value by from sin of x
		forward.y = sin(glm::radians(rotation.x));
		// Get the forward z value by * cos of y by cos of x
		forward.z = cos(glm::radians(rotation.y)) * cos(glm::radians(rotation.x));

		// Make sure we don't normalise 0
		if (glm::length(forward) != 0.0f)
			glm::normalize(forward);

		return forward;
	}

	glm::vec3 Right() {
		// Get the right value by crossing the forward and world up vector
		glm::vec3 right = glm::cross(Forward(), glm::vec3(0.0f, 1.0f, 0.0f));

		// Make sure we don't normalise 0
		if (glm::length(right) != 0.0f)
			glm::normalize(right);

		return right;
	}

	glm::vec3 Up() {
		// Get the up value by crossing the right and forward local direction vectors
		glm::vec3 up = glm::cross(Right(), Forward());

		// Make sure we don't normalise 0
		if (glm::length(up) != 0.0f)
			glm::normalize(up);

		return up;
	}

	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;
};