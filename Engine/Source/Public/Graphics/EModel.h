#pragma once
#include "EngineTypes.h"
#include "Graphics/EMesh.h"
#include "Math/ESTransform.h"

class ETexture;
class EShaderProgram;

class EModel {
public:
	EModel() = default;
	~EModel() = default;
	
	// Create a cube model and add a texture to it
	void MakeCube(const TShared<ETexture>& texture);

	// Create a spike model and add a texture to it
	void MakeSpike(const TShared<ETexture>& texture);

	// Create a plane model and add a texture to it
	void MakePlane(const TShared<ETexture>& texture);
	
	// Render all of the meshes within the model
	// Transform of meshes will be based on models transform
	void Render(const TShared<EShaderProgram>& shader);

	// Get the transform of the model
	ESTransform& GetTransform() { return m_transform; }

	// Translate the model in local space based on the passed translation
	void TranslateLocal(glm::vec3 translation, glm::vec3 scale = glm::vec3(1.0f));

	// Translate the model in world space based on the passed translation
	void TranslateWorld(glm::vec3 translation, glm::vec3 scale = glm::vec3(1.0f));

private:
	// Array of meshes
	TArray<TUnique<EMesh>> m_meshStack;

	// Transform for the model in 3D space
	ESTransform m_transform;
};