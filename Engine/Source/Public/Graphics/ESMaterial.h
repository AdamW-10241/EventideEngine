#pragma once
#include "EngineTypes.h"

class ETexture;

struct ETexturePaths {
	EString base;
	EString normal = "";	// optional
	EString specular = "";  // optional
};

struct ESMaterialDesc {
	// Texture file paths
	ETexturePaths paths;

	// Shininess of the material
	float m_shininess = 32.0f;

	// Intensity of the specular
	float m_specularStrength = 0.05f;

	// Brightness of the material
	float m_brightness = 1.0f;

	// Texture depth of the material
	float m_textureDepth = 1.0f;

	// Set values and return for continued execution
	// grassDesc.WithTextureDepth(5.0f).WithSpecular(0.8f) ...
	ESMaterialDesc& withShininess(float val) { m_shininess = val; return *this; }
	ESMaterialDesc& withSpecularStrength(float val) { m_specularStrength = val; return *this; }
	ESMaterialDesc& withBrightness(float val) { m_brightness = val; return *this; }
	ESMaterialDesc& withTextureDepth(float val) { m_textureDepth = val; return *this; }
};

struct ESMaterialSlot {
	TArray<int> slotIndices;
	ESMaterialDesc desc;

	static ESMaterialSlot Range(int from, int to, ESMaterialDesc desc) {
		ESMaterialSlot slot;
		slot.desc = desc;
		for (int i = from; i <= to; i++) slot.slotIndices.push_back(i);
		return slot;
	}
};

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
	float m_shininess = 32.0f;

	// Intensity of the specular
	float m_specularStrength = 0.05f;

	// Brightness of the material
	float m_brightness = 1.0f;

	// Texture depth of the material
	float m_textureDepth = 1.0f;
};