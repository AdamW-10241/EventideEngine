#include "Graphics/EModel.h"

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

void EModel::MakeCube(const TShared<ETexture>& texture)
{
	// Create the meshe
	TUnique<EMesh> mesh = TMakeUnique<EMesh>();

	// Create the mesh and test if it failed
	if (!mesh->CreateMesh(cubeVData, cubeIData)) {
		EDebug::Log("Failed to create cube the mesh.");
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
	}

	// Add the texture to the mesh and add it to the mesh stack
	mesh->SetTexture(texture);
	// std::move will move the reference without destroying or copying it
	m_meshStack.push_back(std::move(mesh));
}

void EModel::Render(const TShared<EShaderProgram>& shader)
{
	for (const auto& mesh : m_meshStack) {
		mesh->Render(shader, m_transform);
	}
}
