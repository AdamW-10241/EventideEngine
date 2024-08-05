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
struct ESLight;
struct ESMaterial;

class EModel {
public:
	EModel() = default;
	~EModel() = default;
	
	// Import a 3D model from file
	// Uses the ASSIMP import library, check docs to know which file types are accepted
	void ImportModel(const EString& filePath);
	
	// Render all of the meshes within the model
	// Transform of meshes will be based on models transform
	void Render(const TShared<EShaderProgram>& shader, const TArray<TShared<ESLight>>& lights);

	// Get the transform of the model
	ESTransform& GetTransform() { return m_transform; }

	// Set a material by the slot number
	void SetMaterialBySlot(unsigned int slot, const TShared<ESMaterial>& material);

private:
	// Find all of the meshes in a scene and convert them to an EMesh
	bool FindAndImportMeshes(const aiNode& node, const aiScene& scene, 
		const aiMatrix4x4& parentTransform, EUi32* meshesCreated);

private:
	// Array of meshes
	TArray<TUnique<EMesh>> m_meshStack;

	// Array of materials for the model
	TArray<TShared<ESMaterial>> m_materialStack;

	// Transform for the model in 3D space
	ESTransform m_transform;
};