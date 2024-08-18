#include "Graphics/EModel.h"

// External Libss
#include <ASSIMP/Importer.hpp>
#include <ASSIMP/scene.h>
#include <ASSIMP/postprocess.h>
#include <ASSIMP/mesh.h>

EModel::EModel(unsigned int spawnID, EString path)
{
	m_spawnID = spawnID;
	m_path = path;
	m_offset.scale = glm::vec3(0.0f);
}

EModel::~EModel()
{
	// EDebug::Log("Model destroyed: " + m_path);
}

void EModel::ImportModel(const EString& filePath, const TShared<ESMaterial>& defaultMaterial)
{
	// Create an ASSIMP model importer
	Assimp::Importer importer;

	// Read the file and convert to an ASSIMP scene
	// Add post processing flag triangulate to make sure the model is triangles
	// Can add a processing flag here to remove bones
	// Added a flag to calculate the tangent space and get the tangent and bitTangent for normal maps
	const auto scene = importer.ReadFile(filePath, aiProcess_Triangulate | aiProcess_CalcTangentSpace);

	// Check if the import failed in any way
	// !scene is checking if the object was null
	// FLAGS_INCOMPLETE is checking if the import failed
	// rootNode is checking if the model has a mesh at all
	if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		EDebug::Log("Error importing model from - " + filePath + ": " + importer.GetErrorString(),
			LT_ERROR);
		return;
	}

	// Update the scene matrix to start at x0, y0, z0
	aiMatrix4x4 sceneTransform;
	aiMatrix4x4::Translation({ 0.0f, 0.0f, 0.0f }, sceneTransform);
	// Set the rotation to x0, y0, z0
	sceneTransform.FromEulerAnglesXYZ({ 0.0f, 0.0f, 0.0f });
	// Set the scale to x1, y1, z1
	aiMatrix4x4::Scaling({ 1.0f, 1.0f, 1.0f }, sceneTransform);

	// Meshes count
	EUi32 meshesCreated = 0;

	// Find all the meshes in the scene and fail in any of them fail
	if (!FindAndImportMeshes(*scene->mRootNode, *scene, sceneTransform, &meshesCreated)) {
		EDebug::Log("Model failed to convert ASSIMP scene: " + filePath, LT_ERROR);
		return;
	}

	// Set the material stack size to the amount of materials on the model
	m_materialStack.resize(scene->mNumMaterials);

	// Set all materials to the default material
	for (auto& materialRef : m_materialStack) {
		materialRef = defaultMaterial;
	}

	// Log the success of the model
	//EDebug::Log("Model successfully imported with (" + std::to_string(meshesCreated) + ") meshes: " + 
	//	filePath, LT_SUCCESS);
}

void EModel::Render(const TShared<EShaderProgram>& shader, const TArray<TShared<ESLight>>& lights)
{
	for (const auto& mesh : m_meshStack) {
		mesh->Render(shader, m_transform + m_offset, lights, m_materialStack[mesh->materialIndex]);
	}
}

void EModel::SetMaterialBySlot(unsigned int slot, const TShared<ESMaterial>& material)
{
	// Ensure that the material slot exists
	if (slot >= m_materialStack.size()) {
		EDebug::Log("No material slot exists at index: " + std::to_string(slot), LT_WARNING);
		return;
	}

	// Change the material if it does exist
	m_materialStack[slot] = material;
}

bool EModel::FindAndImportMeshes(const aiNode& node, const aiScene& scene,
	const aiMatrix4x4& parentTransform, EUi32* meshesCreated)
{
	// Looping though all of the meshes in the node
	for (EUi32 i = 0; i < node.mNumMeshes; ++i) {
		// Get a mesh from a mesh index
		const auto aMesh = scene.mMeshes[node.mMeshes[i]];

		// Store the mesh vertices and indices
		TArray<ESVertexData> meshVertices;
		TArray<EUi32> meshIndicies;

		// Loop through every vertex and get the data for conversion
		for (EUi64 j = 0; j < aMesh->mNumVertices; ++j) {
			// Create an empty vertex
			ESVertexData vertex;

			// Get the positions of the vertex
			vertex.m_position[0] = aMesh->mVertices[j].x;
			vertex.m_position[1] = aMesh->mVertices[j].y;
			vertex.m_position[2] = aMesh->mVertices[j].z;

			// If there are vertex colours then update
			if (aMesh->HasVertexColors(j)) {
				vertex.m_color[0] = aMesh->mColors[j]->r;
				vertex.m_color[1] = aMesh->mColors[j]->g;
				vertex.m_color[2] = aMesh->mColors[j]->b;
			}

			// Set the texture coordinates
			// Texture coordinate can have multiple sets
			// The first array index is the set [0]
			// The second array index is the vertex data [j]
			if (aMesh->HasTextureCoords(0)) {
				vertex.m_texCoords[0] = aMesh->mTextureCoords[0][j].x;
				vertex.m_texCoords[1] = aMesh->mTextureCoords[0][j].y;
			}	

			// Set the normals for the model
			vertex.m_normal[0] = aMesh->mNormals[j].x;
			vertex.m_normal[1] = aMesh->mNormals[j].y;
			vertex.m_normal[2] = aMesh->mNormals[j].z;

			// Set the tangents for the model
			vertex.m_tangent[0] = aMesh->mTangents[j].x;
			vertex.m_tangent[1] = aMesh->mTangents[j].y;
			vertex.m_tangent[2] = aMesh->mTangents[j].z;

			// Set the bitTangents for the model
			vertex.m_bitTangent[0] = aMesh->mBitangents[j].x;
			vertex.m_bitTangent[1] = aMesh->mBitangents[j].y;
			vertex.m_bitTangent[2] = aMesh->mBitangents[j].z;

			// Add the data into out vertex array
			meshVertices.push_back(vertex);
		}

		// The GPU requires a minimum of 3 vertices to render
		// Fail if there are less than 3
		if (meshVertices.size() < 3) {
			EDebug::Log("Mesh had less than 3 vertices", LT_ERROR);
			return false;
		}

		// Loop through all of the faces on the mesh to get the indicies
		for (EUi64 j = 0; j < aMesh->mNumFaces; ++j) {
			// Store the face as a variable
			const auto& face = aMesh->mFaces[j];

			// Looping thjrough all of the indices in the face
			// There should only be 3
			for (EUi32 k = 0; k < face.mNumIndices; ++k) {
				meshIndicies.push_back(face.mIndices[k]);
			}
		}

		// Create the mesh object
		auto eMesh = TMakeUnique<EMesh>();

		// Test if the mesh failed to create
		if (!eMesh->CreateMesh(meshVertices, meshIndicies)) {
			EDebug::Log("Mesh failed to convert from aMesh to eMesh", LT_ERROR);
			return false;
		}

		// Get the material index from the ASSIMP mesh and set our index to the same
		eMesh->materialIndex = aMesh->mMaterialIndex;

		// Set the relative transformation for the mesh
		aiMatrix4x4 relTransform = parentTransform * node.mTransformation;

		// Set a default matrix transform for GLM
		glm::mat4 matTransform(1.0f);

		// Convert the relative ASSIMP transform into a GLM transform
		matTransform[0][0] = relTransform.a1; matTransform[1][0] = relTransform.a2;
		matTransform[2][0] = relTransform.a3; matTransform[3][0] = relTransform.a4;

		matTransform[0][1] = relTransform.b1; matTransform[1][1] = relTransform.b2;
		matTransform[2][1] = relTransform.b3; matTransform[3][1] = relTransform.b4;

		matTransform[0][2] = relTransform.c1; matTransform[1][2] = relTransform.c2;
		matTransform[2][2] = relTransform.c3; matTransform[3][2] = relTransform.c4;

		matTransform[0][3] = relTransform.d1; matTransform[1][3] = relTransform.d2;
		matTransform[2][3] = relTransform.d3; matTransform[3][3] = relTransform.d4;

		// Update the relative transform on the mesh
		eMesh->SetRelativeTransform(matTransform);

		// Add the new mesh to the mesh stack
		m_meshStack.push_back(std::move(eMesh));

		// Count the meshes created
		++(*meshesCreated);
	}

	// Adding the relative transform to the parent transform
	const aiMatrix4x4 nodeRelTransform = parentTransform * node.mTransformation;

	// Loop though all of the child nodes inside this node
	for (EUi32 i = 0; i < node.mNumChildren; ++i) {
		if (!FindAndImportMeshes(*node.mChildren[i], scene, nodeRelTransform, meshesCreated)) {
			return false;
		}
	}

	return true;
}
