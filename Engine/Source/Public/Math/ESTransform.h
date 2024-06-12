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

	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;
};