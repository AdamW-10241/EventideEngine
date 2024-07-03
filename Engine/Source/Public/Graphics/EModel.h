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

	// Create a poly model and add a texture to it
	void MakePoly(const TShared<ETexture>& texture);
	
	// Render all of the meshes within the model
	// Transform of meshes will be based on models transform
	void Render(const TShared<EShaderProgram>& shader);

	// Get the transform of the model
	ESTransform& GetTransform() { return m_transform; }

private:
	// Array of meshes
	TArray<TUnique<EMesh>> m_meshStack;

	// Transform for the model in 3D space
	ESTransform m_transform;
};