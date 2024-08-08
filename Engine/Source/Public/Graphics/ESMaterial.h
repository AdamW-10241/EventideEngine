#pragma once
#include "EngineTypes.h"

class ETexture;

struct ESMaterial {
	ESMaterial() = default;

	// Colour map for the material
	TShared<ETexture> m_baseColourMap;

	// Specular, shinyness map for the material
	// Usually a black and white map
	// White is completely shiny and black is completely not
	TShared<ETexture> m_specularMap;

	// Normal map for the material
	// Influences the light bounce direction
	TShared<ETexture> m_normalMap;

	// Material properties
	// Shininess of the material
	float shininess = 32.0f;

	// Intensity of the specular
	float specularStrength = 0.5f;
};