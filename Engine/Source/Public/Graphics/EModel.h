#pragma once
#include "EngineTypes.h"
#include "Graphics/EMesh.h"
#include "Math/ESTransform.h"

// External Libs
#include <ASSIMP/matrix4x4.h>

class ETexture;
class EShaderProgram;
struct aiScene;
struct aiNode;

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

	// Import a 3D model from file
	// Uses the ASSIMP import library, check docs to know which file types are accepted
	void ImportModel(const EString& filePath);
	
	// Render all of the meshes within the model
	// Transform of meshes will be based on models transform
	void Render(const TShared<EShaderProgram>& shader);

	// Get the transform of the model
	ESTransform& GetTransform() { return m_transform; }

private:
	// Find all of the meshes in a scene and convert them to an EMesh
	bool FindAndImportMeshes(const aiNode& node, const aiScene& scene, const aiMatrix4x4& parentTransform);

private:
	// Array of meshes
	TArray<TUnique<EMesh>> m_meshStack;

	// Transform for the model in 3D space
	ESTransform m_transform;
};