#pragma once
#include "Math/ESTransform.h"

struct ESCamera {
	ESCamera() {
		fov = 70.0f;
		aspectRatio = 1.0f;
		nearClip = 0.01f;
		farClip = 10000.0f;
	}

	ESTransform transform;
	float fov;
	float aspectRatio;
	float nearClip;
	float farClip;
};