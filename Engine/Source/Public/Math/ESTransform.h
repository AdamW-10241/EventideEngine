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

	ESTransform(const glm::vec3& p, const glm::vec3& r, const glm::vec3& s) {
		position = p;
		rotation = r;
		scale = s;
	}

	// Get the forward vector of the local rotation
	glm::vec3 Forward() {
		glm::vec3 forward = glm::vec3(0.0f);

		// Get the forward x value by * sin of y by cos of x
		forward.x = sin(glm::radians(rotation.y)) * cos(glm::radians(rotation.x));
		// Get the forward y value by from sin of x
		forward.y = sin(glm::radians(-rotation.x));
		// Get the forward z value by * cos of y by cos of x
		forward.z = cos(glm::radians(rotation.y)) * cos(glm::radians(rotation.x));

		// Make sure we don't normalise 0
		if (glm::length(forward) != 0.0f)
			forward = glm::normalize(forward);

		return forward;
	}

	glm::vec3 Right() {
		// Get the right value by crossing the forward and world up vector
		glm::vec3 right = glm::cross(Forward(), glm::vec3(0.0f, 1.0f, 0.0f));

		// Make sure we don't normalise 0
		if (glm::length(right) != 0.0f)
			right = glm::normalize(right);

		return right;
	}

	glm::vec3 Up() {
		// Get the up value by crossing the right and forward local direction vectors
		glm::vec3 up = glm::cross(Right(), Forward());

		// Make sure we don't normalise 0
		if (glm::length(up) != 0.0f)
			up = glm::normalize(up);

		return up;
	}

	ESTransform operator+(const ESTransform& other) const {
		return {
			position + other.position,
			rotation + other.rotation,
			scale + other.scale
		};
	}

	ESTransform& operator+=(const ESTransform& other) {
		return *this = *this + other;
	}

	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;
};

struct ESTransform2D {
	ESTransform2D() {
		position = glm::vec2(0.0f);
		rotation = 0.0f;
		scale = glm::vec2(0.0f);
	}

	ESTransform2D(const glm::vec2& p) {
		position = p;
		rotation = 0.0f;
		scale = glm::vec2(0.0f);
	}

	ESTransform2D(const glm::vec2& p, const float& r, const glm::vec2& s) {
		position = p;
		rotation = r;
		scale = s;
	}

	void CenterOnPosition() { position -= scale * 0.5f; }

	void UncenterPosition() { position += scale * 0.5f; }

	ESTransform2D operator+(const ESTransform2D& other) const {
		return {
			position + other.position,
			rotation + other.rotation,
			scale + other.scale
		};
	}

	ESTransform2D& operator+=(const ESTransform2D& other) {
		return *this = *this + other;
	}

	ESTransform2D& operator=(const ESTransform2D& other) {
		if (this != &other) {
			position = other.position;
			rotation = other.rotation;
			scale = other.scale;
		}
		return *this;
	}

	glm::vec2 position;
	float rotation;
	glm::vec2 scale;
};