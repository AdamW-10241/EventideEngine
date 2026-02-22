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
		// Move the input direction forward if required
		glm::vec3 moveDir = transform.Forward() * translation.z;
		moveDir += transform.Right() * translation.x;
		moveDir.y += translation.y;

		if (glm::length(moveDir) != 0.0f)
			moveDir = glm::normalize(moveDir);

		glm::vec3 direction = moveDir * scale;

		// Cancel out vertical movements if flag set
		if (!canMoveVertical)
			direction.y = 0.0f;

		// Get delta time
		float deltaTime = 1.0f;
		if (const auto& ge = EGameEngine::GetGameEngine()) {
			deltaTime = ge->DeltaTimeF();
		}

		// Get movement
		glm::vec3 movement = direction * moveSpeed * deltaTime;

		// Adjust position
		transform.position += movement;
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