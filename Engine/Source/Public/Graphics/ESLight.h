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

		linear = 0.045f;
		quadratic = 0.0075f;
	};

	~ESSpotLight() = default;

	glm::vec3 position;
	glm::vec3 direction;

	// Fall off values for how far the light can reach
	float linear;
	float quadratic;
};