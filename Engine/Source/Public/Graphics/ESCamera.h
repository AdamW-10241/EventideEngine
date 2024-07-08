#pragma once
#include "Math/ESTransform.h"

struct ESCamera {
	ESCamera() {
		SetFOV(70.0f);
		aspectRatio = 1.0f;
		nearClip = 0.01f;
		farClip = 10000.0f;
		moveSpeed = 0.001f;
		rotateSpeed = 0.1f;
		zoomSpeed = 3.0f;
	}

	// Rotate the camera based on the passed rotation
	void Rotate(glm::vec3 rotation, glm::vec3 scale = glm::vec3(1.0f)) {

		if (glm::length(rotation) != 0.0f)
			rotation = glm::normalize(rotation);

		transform.rotation += rotation * scale * rotateSpeed;

		if (transform.rotation.x < -89.0f)
			transform.rotation.x = -89.0f;

		if (transform.rotation.x > 89.0f)
			transform.rotation.x = 89.0f;
	};

	// Translate the camera based on the passed translation
	void Translate(glm::vec3 translation, glm::vec3 scale = glm::vec3(1.0f)) {
		// Move the input direction forward if required
		glm::vec3 moveDir = transform.Forward() * translation.z;
		moveDir += transform.Right() * translation.x;
		moveDir.y += translation.y;

		if (glm::length(moveDir) != 0.0f)
			moveDir = glm::normalize(moveDir);

		transform.position += moveDir * scale * moveSpeed;
	};

	// Zoom in the fov based on the amount added
	void Zoom(const float& amount) {
		fov -= amount * zoomSpeed;
	};

	// Reset the zoom to the default fov
	void ResetZoom() {
		fov = defaultFov;
	}

	// Use to set the fov so that default fov is updated
	void SetFOV(const float& newFov) {
		fov = newFov;
		defaultFov = newFov;
	}

	ESTransform transform;
	float defaultFov;
	float fov;
	float aspectRatio;
	float nearClip;
	float farClip;
	float moveSpeed;
	float rotateSpeed;
	float zoomSpeed;
};