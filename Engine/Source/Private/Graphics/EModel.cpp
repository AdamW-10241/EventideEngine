#include "Graphics/EModel.h"

// External Libss
#include <ASSIMP/Importer.hpp>
#include <ASSIMP/scene.h>
#include <ASSIMP/postprocess.h>
#include <ASSIMP/mesh.h>

const std::vector<ESVertexData> polyVData = {
	//  X,	    Y,    Z	        R,    G,    B		// Tex Coords
   { { -1.0f,  0.5f, 0.0f }, { 1.0f, 1.0f, 1.0f}, { 0.0f, 1.0f } }, // Vertex Data 1 - TL
   { {  1.0f,  0.5f, 0.0f }, { 1.0f, 1.0f, 1.0f}, { 1.0f, 1.0f } }, // Vertex Data 2 - TR
   { { -1.0f, -0.5f, 0.0f }, { 1.0f, 1.0f, 1.0f}, { 0.0f, 0.0f } }, // Vertex Data 3 - BL
   { {  1.0f, -0.5f, 0.0f }, { 1.0f, 1.0f, 1.0f}, { 1.0f, 0.0f } }  // Vertex Data 4 - BR
};

const std::vector<uint32_t> polyIData = {
	0, 1, 2, // Tri 1
	1, 2, 3 // Tri 2
};

const std::vector<ESVertexData> cubeVData = {
	 //  X,	    Y,     Z	     R,    G,    B		   Tex Coords
	{ { -1.0f,  1.0f,  1.0f }, { 1.0f, 1.0f, 1.0f }, { 0.0f, 1.0f } }, // Vertex Data 0 - Front TL
	{ {  1.0f,  1.0f,  1.0f }, { 1.0f, 1.0f, 1.0f }, { 1.0f, 1.0f } }, // Vertex Data 1 - Front TR
	{ { -1.0f, -1.0f,  1.0f }, { 1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f } }, // Vertex Data 2 - Front BL
	{ {  1.0f, -1.0f,  1.0f }, { 1.0f, 1.0f, 1.0f }, { 1.0f, 0.0f } }, // Vertex Data 3 - Front BR
   
	{ {  1.0f,  1.0f, -1.0f }, { 1.0f, 1.0f, 1.0f }, { 0.0f, 1.0f } }, // Vertex Data 4 - Back TL
	{ { -1.0f,  1.0f, -1.0f }, { 1.0f, 1.0f, 1.0f }, { 1.0f, 1.0f } }, // Vertex Data 5 - Back TR
	{ {  1.0f, -1.0f, -1.0f }, { 1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f } }, // Vertex Data 6 - Back BL
	{ { -1.0f, -1.0f, -1.0f }, { 1.0f, 1.0f, 1.0f }, { 1.0f, 0.0f } }, // Vertex Data 7 - Back BR

	{ { -1.0f,  1.0f, -1.0f }, { 1.0f, 1.0f, 1.0f }, { 0.0f, 1.0f } }, // Vertex Data 8 - Left TL
	{ { -1.0f,  1.0f,  1.0f }, { 1.0f, 1.0f, 1.0f }, { 1.0f, 1.0f } }, // Vertex Data 9 - Left TR
	{ { -1.0f, -1.0f, -1.0f }, { 1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f } }, // Vertex Data 10 - Left BL
	{ { -1.0f, -1.0f,  1.0f }, { 1.0f, 1.0f, 1.0f }, { 1.0f, 0.0f } }, // Vertex Data 11 - Left BR

	{ {  1.0f,  1.0f,  1.0f }, { 1.0f, 1.0f, 1.0f }, { 0.0f, 1.0f } }, // Vertex Data 12 - Right TL
	{ {  1.0f,  1.0f, -1.0f }, { 1.0f, 1.0f, 1.0f }, { 1.0f, 1.0f } }, // Vertex Data 13 - Right TR
	{ {  1.0f, -1.0f,  1.0f }, { 1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f } }, // Vertex Data 14 - Right BL
	{ {  1.0f, -1.0f, -1.0f }, { 1.0f, 1.0f, 1.0f }, { 1.0f, 0.0f } }, // Vertex Data 15 - Right BR

	{ { -1.0f,  1.0f, -1.0f }, { 1.0f, 1.0f, 1.0f }, { 0.0f, 1.0f } }, // Vertex Data 16 - Top TL
	{ {  1.0f,  1.0f, -1.0f }, { 1.0f, 1.0f, 1.0f }, { 1.0f, 1.0f } }, // Vertex Data 17 - Top TR
	{ { -1.0f,  1.0f,  1.0f }, { 1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f } }, // Vertex Data 18 - Top BL
	{ {  1.0f,  1.0f,  1.0f }, { 1.0f, 1.0f, 1.0f }, { 1.0f, 0.0f } }, // Vertex Data 19 - Top BR

	{ { -1.0f, -1.0f,  1.0f }, { 1.0f, 1.0f, 1.0f }, { 0.0f, 1.0f } }, // Vertex Data 20 - Bottom TL
	{ {  1.0f, -1.0f,  1.0f }, { 1.0f, 1.0f, 1.0f }, { 1.0f, 1.0f } }, // Vertex Data 21 - Bottom TR
	{ { -1.0f, -1.0f, -1.0f }, { 1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f } }, // Vertex Data 22 - Bottom BL
	{ {  1.0f, -1.0f, -1.0f }, { 1.0f, 1.0f, 1.0f }, { 1.0f, 0.0f } }, // Vertex Data 23 - Bottom BR
};

const std::vector<uint32_t> cubeIData = {
	0, 1, 2,	// Front Tri 1
	1, 2, 3,	// Front Tri 2

	4, 5, 6,	// Back Tri 1
	5, 6, 7,	// Back Tri 2

	8, 9, 10,	// Left Tri 1
	9, 10, 11,	// Left Tri 2

	12, 13, 14,	// Right Tri 1
	13, 14, 15,	// Right Tri 2

	16, 17, 18, // Top Tri 1
	17, 18, 19, // Top Tri 2

	20, 21, 22,	// Bottom Tri 1
	21, 22, 23	// Bottom Tri 2
};

const std::vector<ESVertexData> spikeVData = {
	//   X,	    Y,     Z	     R,    G,    B		  TX,    TY
	// Front
	{ {  0.0f,  0.0f,  2.0f }, { 1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f } }, // Vertex Data 0 - Front Spike

	{ { -1.0f,  1.0f,  1.0f }, { 1.0f, 1.0f, 1.0f }, { 0.0f, 1.0f } }, // Vertex Data 1 - Front TL
	{ {  1.0f,  1.0f,  1.0f }, { 1.0f, 1.0f, 1.0f }, { 1.0f, 1.0f } }, // Vertex Data 2 - Front TR
	{ {  1.0f, -1.0f,  1.0f }, { 1.0f, 1.0f, 1.0f }, { 1.0f, 0.0f } }, // Vertex Data 3 - Front BR

	{ {  1.0f, -1.0f,  1.0f }, { 1.0f, 1.0f, 1.0f }, { 0.0f, 1.0f } }, // Vertex Data 4 - Front BR
	{ { -1.0f, -1.0f,  1.0f }, { 1.0f, 1.0f, 1.0f }, { 1.0f, 1.0f } }, // Vertex Data 5 - Front BL
	{ { -1.0f,  1.0f,  1.0f }, { 1.0f, 1.0f, 1.0f }, { 1.0f, 0.0f } }, // Vertex Data 6 - Front TL

	// Back
	{ {  0.0f,  0.0f, -2.0f }, { 1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f } }, // Vertex Data 7 - Back Spike

	{ {  1.0f,  1.0f, -1.0f }, { 1.0f, 1.0f, 1.0f }, { 0.0f, 1.0f } }, // Vertex Data 8 - Back TL
	{ { -1.0f,  1.0f, -1.0f }, { 1.0f, 1.0f, 1.0f }, { 1.0f, 1.0f } }, // Vertex Data 9 - Back TR
	{ { -1.0f, -1.0f, -1.0f }, { 1.0f, 1.0f, 1.0f }, { 1.0f, 0.0f } }, // Vertex Data 10 - Back BR

	{ { -1.0f, -1.0f, -1.0f }, { 1.0f, 1.0f, 1.0f }, { 0.0f, 1.0f } }, // Vertex Data 11 - Back BR
	{ {  1.0f, -1.0f, -1.0f }, { 1.0f, 1.0f, 1.0f }, { 1.0f, 1.0f } }, // Vertex Data 12 - Back BL
	{ {  1.0f,  1.0f, -1.0f }, { 1.0f, 1.0f, 1.0f }, { 1.0f, 0.0f } }, // Vertex Data 13 - Back TL

	// Left
	{ { -2.0f,  0.0f,  0.0f }, { 1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f } }, // Vertex Data 14 - Left Spike

	{ { -1.0f,  1.0f, -1.0f }, { 1.0f, 1.0f, 1.0f }, { 0.0f, 1.0f } }, // Vertex Data 15 - Left TL
	{ { -1.0f,  1.0f,  1.0f }, { 1.0f, 1.0f, 1.0f }, { 1.0f, 1.0f } }, // Vertex Data 16 - Left TR
	{ { -1.0f, -1.0f,  1.0f }, { 1.0f, 1.0f, 1.0f }, { 1.0f, 0.0f } }, // Vertex Data 17 - Left BR

	{ { -1.0f, -1.0f,  1.0f }, { 1.0f, 1.0f, 1.0f }, { 0.0f, 1.0f } }, // Vertex Data 18 - Left BR
	{ { -1.0f, -1.0f, -1.0f }, { 1.0f, 1.0f, 1.0f }, { 1.0f, 1.0f } }, // Vertex Data 19 - Left BL
	{ { -1.0f,  1.0f, -1.0f }, { 1.0f, 1.0f, 1.0f }, { 1.0f, 0.0f } }, // Vertex Data 20 - Left TL

	// Right
	{ {  2.0f,  0.0f,  0.0f }, { 1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f } }, // Vertex Data 21 - Right Spike

	{ {  1.0f,  1.0f,  1.0f }, { 1.0f, 1.0f, 1.0f }, { 0.0f, 1.0f } }, // Vertex Data 22 - Right TL
	{ {  1.0f,  1.0f, -1.0f }, { 1.0f, 1.0f, 1.0f }, { 1.0f, 1.0f } }, // Vertex Data 23 - Right TR
	{ {  1.0f, -1.0f, -1.0f }, { 1.0f, 1.0f, 1.0f }, { 1.0f, 0.0f } }, // Vertex Data 24 - Right BR

	{ {  1.0f, -1.0f, -1.0f }, { 1.0f, 1.0f, 1.0f }, { 0.0f, 1.0f } }, // Vertex Data 25 - Right BR
	{ {  1.0f, -1.0f,  1.0f }, { 1.0f, 1.0f, 1.0f }, { 1.0f, 1.0f } }, // Vertex Data 26 - Right BL
	{ {  1.0f,  1.0f,  1.0f }, { 1.0f, 1.0f, 1.0f }, { 1.0f, 0.0f } }, // Vertex Data 27 - Right TL

	// Top
	{ {  0.0f,  2.0f,  0.0f }, { 1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f } }, // Vertex Data 28 - Top Spike

	{ { -1.0f,  1.0f, -1.0f }, { 1.0f, 1.0f, 1.0f }, { 0.0f, 1.0f } }, // Vertex Data 29 - Top TL
	{ {  1.0f,  1.0f, -1.0f }, { 1.0f, 1.0f, 1.0f }, { 1.0f, 1.0f } }, // Vertex Data 30 - Top TR
	{ {  1.0f,  1.0f,  1.0f }, { 1.0f, 1.0f, 1.0f }, { 1.0f, 0.0f } }, // Vertex Data 31 - Top BR
	
	{ {  1.0f,  1.0f,  1.0f }, { 1.0f, 1.0f, 1.0f }, { 0.0f, 1.0f } }, // Vertex Data 32 - Top BR
	{ { -1.0f,  1.0f,  1.0f }, { 1.0f, 1.0f, 1.0f }, { 1.0f, 1.0f } }, // Vertex Data 33 - Top BL
	{ { -1.0f,  1.0f, -1.0f }, { 1.0f, 1.0f, 1.0f }, { 1.0f, 0.0f } }, // Vertex Data 34 - Top TL

	// Bottom
	{ {  0.0f, -2.0f,  0.0f }, { 1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f } }, // Vertex Data 35 - Bottom Spike

	{ { -1.0f, -1.0f,  1.0f }, { 1.0f, 1.0f, 1.0f }, { 0.0f, 1.0f } }, // Vertex Data 36 - Bottom TL
	{ {  1.0f, -1.0f,  1.0f }, { 1.0f, 1.0f, 1.0f }, { 1.0f, 1.0f } }, // Vertex Data 37 - Bottom TR
	{ {  1.0f, -1.0f, -1.0f }, { 1.0f, 1.0f, 1.0f }, { 1.0f, 0.0f } }, // Vertex Data 38 - Bottom BR

	{ {  1.0f, -1.0f, -1.0f }, { 1.0f, 1.0f, 1.0f }, { 0.0f, 1.0f } }, // Vertex Data 39 - Bottom BR
	{ { -1.0f, -1.0f, -1.0f }, { 1.0f, 1.0f, 1.0f }, { 1.0f, 1.0f } }, // Vertex Data 40 - Bottom BL
	{ { -1.0f, -1.0f,  1.0f }, { 1.0f, 1.0f, 1.0f }, { 1.0f, 0.0f } }, // Vertex Data 41 - Bottom TL
};

const std::vector<uint32_t> spikeIData = {
	// Front Spike
	0, 1, 2,	// Top
	0, 2, 3,	// Right
	0, 4, 5,	// Bottom
	0, 5, 6,	// Left

	// Back Spike
	7, 8, 9,	// Top
	7, 9, 10,	// Right
	7, 11, 12,	// Bottom
	7, 12, 13, // Left

	// Left Spike
	14, 15, 16,	// Top
	14, 16, 17,	// Right
	14, 18, 19,	// Bottom
	14, 19, 20, // Left

	// Right Spike
	21, 22, 23,	// Top
	21, 23, 24,	// Right
	21, 25, 26,	// Bottom
	21, 26, 27, // Left

	// Top Spike
	28, 29, 30,	// Top
	28, 30, 31,	// Right
	28, 32, 33,	// Bottom
	28, 33, 34, // Left

	// Bottom Spike
	35, 36, 37,	// Top
	35, 37, 38,	// Right
	35, 39, 40,	// Bottom
	35, 40, 41	// Left
};

void EModel::MakeCube(const TShared<ETexture>& texture)
{
	// Create the meshe
	TUnique<EMesh> mesh = TMakeUnique<EMesh>();

	// Create the mesh and test if it failed
	if (!mesh->CreateMesh(cubeVData, cubeIData)) {
		EDebug::Log("Failed to create the cube mesh.");
		return;
	}

	// Add the texture to the mesh and add it to the mesh stack
	mesh->SetTexture(texture);
	// std::move will move the reference without destroying or copying it
	m_meshStack.push_back(std::move(mesh));
}

void EModel::MakeSpike(const TShared<ETexture>& texture)
{
	// Create the meshe
	TUnique<EMesh> mesh = TMakeUnique<EMesh>();

	// Create the mesh and test if it failed
	if (!mesh->CreateMesh(spikeVData, spikeIData)) {
		EDebug::Log("Failed to create the spike mesh.");
		return;
	}

	// Add the texture to the mesh and add it to the mesh stack
	mesh->SetTexture(texture);
	// std::move will move the reference without destroying or copying it
	m_meshStack.push_back(std::move(mesh));
}

void EModel::MakePoly(const TShared<ETexture>& texture)
{
	// Create the meshe
	TUnique<EMesh> mesh = TMakeUnique<EMesh>();

	// Create the mesh and test if it failed
	if (!mesh->CreateMesh(polyVData, polyIData)) {
		EDebug::Log("Failed to create the poly mesh.");
		return;
	}

	// Add the texture to the mesh and add it to the mesh stack
	mesh->SetTexture(texture);
	// std::move will move the reference without destroying or copying it
	m_meshStack.push_back(std::move(mesh));
}

void EModel::ImportModel(const EString& filePath)
{
	// Create an ASSIMP model importer
	Assimp::Importer importer;

	// Read the file and convert to an ASSIMP scene
	// Add post processing flag triangulate to make sure the model is triangles
	// Can add a processing flag here to remove bones
	const auto scene = importer.ReadFile(filePath, aiProcess_Triangulate);

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

	// Log the success of the model
	EDebug::Log("Model successfully imported with (" + std::to_string(meshesCreated) + ") meshes: " + 
		filePath, LT_SUCCESS);
}

void EModel::Render(const TShared<EShaderProgram>& shader, const TArray<TShared<ESLight>>& lights)
{
	for (const auto& mesh : m_meshStack) {
		mesh->Render(shader, m_transform, lights);
	}
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
			if (aMesh->HasTextureCoords(j)) {
				vertex.m_texCoords[0] = aMesh->mTextureCoords[0][j].x;
				vertex.m_texCoords[1] = aMesh->mTextureCoords[0][j].y;
			}	

			// Set the normals for the model
			vertex.m_normal[0] = aMesh->mNormals[j].x;
			vertex.m_normal[1] = aMesh->mNormals[j].y;
			vertex.m_normal[2] = aMesh->mNormals[j].z;

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
			auto face = aMesh->mFaces[j];

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
