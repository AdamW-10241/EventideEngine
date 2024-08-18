#pragma once

// External Libs
#include <GLM/vec3.hpp>

struct ESLight {
	ESLight() {
		colour = glm::vec3(1.0f);
		intensity = 0.0f;
	}

 	virtual ~ESLight() = default;

	glm::vec3 colour;
	float intensity;
};

struct ESDirLight : public ESLight {
	ESDirLight() {
		ambient = glm::vec3(0.0f);
		direction = glm::vec3(0.0f);
	}

	~ESDirLight() = default;
	
	glm::vec3 ambient;
	glm::vec3 direction;
};

struct ESPointLight : public ESLight {
	ESPointLight() {
		position = glm::vec3(0.0f);

		linear = 0.045f;
		quadratic = 0.0075f;
	};

	~ESPointLight() = default;
	
	glm::vec3 position;

	// Fall off values for how far the light can reach
	float linear;
	float quadratic;
};

struct ESSpotLight : public ESLight {
	ESSpotLight() {
		direction = glm::vec3(0.0f);
		position = glm::vec3(0.0f);

		innerCutOff = 12.5f;
		outerCutOff = 17.5f;

		linear = 0.045f;
		quadratic = 0.0075f;
	};

	~ESSpotLight() = default;

	// Translate locally based on translation
	void TranslateLocally(glm::vec3 translation, glm::vec3 scale = glm::vec3(1.0f)) {
		// Move the input direction forward if required
		glm::vec3 moveDir =	Forward() * translation.z;
		moveDir += Right() * translation.x;
		moveDir.y += translation.y;

		if (glm::length(moveDir) != 0.0f)
			moveDir = glm::normalize(moveDir);

		// Adjust position
		position += moveDir * scale;
	}

	// Get the forward vector of the local rotation
	glm::vec3 Forward() {
		glm::vec3 forward = glm::vec3(0.0f);

		// Get the forward x value by * sin of y by cos of x
		forward.x = sin(glm::radians(direction.y)) * cos(glm::radians(direction.x));
		// Get the forward y value by from sin of x
		forward.y = sin(glm::radians(direction.x));
		// Get the forward z value by * cos of y by cos of x
		forward.z = cos(glm::radians(direction.y)) * cos(glm::radians(direction.x));

		// Make sure we don't normalise 0
		if (glm::length(forward) != 0.0f)
			glm::normalize(forward);

		return forward;
	}

	// Get the right vector of the local rotation
	glm::vec3 Right() {
		// Get the right value by crossing the forward and world up vector
		glm::vec3 right = glm::cross(Forward(), glm::vec3(0.0f, 1.0f, 0.0f));

		// Make sure we don't normalise 0
		if (glm::length(right) != 0.0f)
			glm::normalize(right);

		return right;
	}

	// Get the up vector of the local rotation
	glm::vec3 Up() {
		// Get the up value by crossing the right and forward local direction vectors
		glm::vec3 up = glm::cross(Right(), Forward());

		// Make sure we don't normalise 0
		if (glm::length(up) != 0.0f)
			glm::normalize(up);

		return up;
	}

	glm::vec3 position;
	glm::vec3 direction;

	float innerCutOff;
	float outerCutOff;

	// Fall off values for how far the light can reach
	float linear;
	float quadratic;
};