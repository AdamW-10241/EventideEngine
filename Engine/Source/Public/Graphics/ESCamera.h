#pragma once
#include "Math/ESTransform.h"
#include "Game/EGameEngine.h"

struct ESCamera {
	ESCamera() {
		SetFOV(70.0f);
		aspectRatio = 1.0f;
		nearClip = 0.01f;
		farClip = 10000.0f;
		moveSpeed = 50.0f;
		rotateSpeed = 0.3f;
		zoomSpeed = 3.0f;
		canMoveVertical = false;
	}

	// Rotate the camera based on the passed rotation
	void Rotate(glm::vec3 rotation, glm::vec3 scale = glm::vec3(1.0f)) {
		// Adjust rotation
		transform.rotation += rotation * scale * rotateSpeed;

		// Cap rotation range
		if (transform.rotation.x < -89.0f)
			transform.rotation.x = -89.0f;

		if (transform.rotation.x > 89.0f)
			transform.rotation.x = 89.0f;
	};

	// Translate the camera based on the passed translation
	void Translate(glm::vec3 translation, glm::vec3 scale = glm::vec3(1.0f)) {
		// Use yaw-only forward/right so pitch doesn't affect movement
		glm::vec3 flatForward = glm::normalize(glm::vec3(transform.Forward().x, 0.0f, transform.Forward().z));
		glm::vec3 flatRight = glm::normalize(glm::vec3(transform.Right().x, 0.0f, transform.Right().z));

		glm::vec3 moveDir = flatForward * translation.z;
		moveDir += flatRight * translation.x;
		moveDir.y += translation.y;

		if (glm::length(moveDir) != 0.0f)
			moveDir = glm::normalize(moveDir);

		glm::vec3 direction = moveDir * scale;
		if (!canMoveVertical)
			direction.y = 0.0f;

		float deltaTime = EGameEngine::GetGameEngine()->DeltaTimeF();
		transform.position += direction * moveSpeed * deltaTime;
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

	// Set aspect ratio to match window size
	void SetWindowAspectRatio(glm::vec2 windowSize) {
		aspectRatio = windowSize.x / windowSize.y;
	}

	// Get the vertical movement status for the camera
	bool& GetVerticalMovementStatus() { return canMoveVertical; }

	ESTransform transform;
	float defaultFov;
	float fov;
	float aspectRatio;
	float nearClip;
	float farClip;
	float moveSpeed;
	float rotateSpeed;
	float zoomSpeed;
	bool canMoveVertical;
};