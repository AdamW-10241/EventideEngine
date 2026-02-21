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
	EModel(unsigned int spawnID, EString path);
	~EModel();

	// Import a 3D model from file
	// Uses the ASSIMP import library, check docs to know which file types are accepted
	void ImportModel(const EString& filePath, const TShared<ESMaterial>& defaultMaterial);
	
	// Render all of the meshes within the model
	// Transform of meshes will be based on models transform
	void Render(const ESTransform& transform, const TShared<EShaderProgram>& shader, const TArray<TShared<ESLight>>& lights);

	// Set a material by the slot number
	void SetMaterialBySlot(unsigned int slot, const TShared<ESMaterial>& material);

	// Get the spawn ID of the model
	unsigned int GetSpawnID() const { return m_spawnID; }

	// Get the path of the model
	EString GetPath() const { return m_path; }

	// Get the models mesh stack
	TUnique<EMesh>& GetMesh(const int& index) { return m_meshStack.at(index); }

	// Get whether model has materials
	const bool HasMaterials() const { return m_materialStack.size() > 0; }

	// Get model materials
	const TArray<TShared<ESMaterial>>& GetMaterials() const { return m_materialStack; }

public:
	// Transform offset
	ESTransform m_offset;

private:
	// Find all of the meshes in a scene and convert them to an EMesh
	bool FindAndImportMeshes(const aiNode& node, const aiScene& scene, 
		const aiMatrix4x4& parentTransform, EUi32* meshesCreated);

private:
	// Array of meshes
	TArray<TUnique<EMesh>> m_meshStack;

	// Array of materials for the model
	TArray<TShared<ESMaterial>> m_materialStack;

	// Spawn ID
	unsigned int m_spawnID;

	// Model path
	EString m_path;
};