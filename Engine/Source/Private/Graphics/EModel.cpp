#include "Graphics/EModel.h"

const std::vector<ESVertexData> planeVData = {
//      X  	   Y      Z	        R     G     B		 TX    TY
   { { -5.0f,  0.0f,  5.0f }, { 1.0f, 1.0f, 1.0f}, { 0.0f, 1.0f } }, // Vertex Data 1 - TL
   { {  5.0f,  0.0f,  5.0f }, { 1.0f, 1.0f, 1.0f}, { 1.0f, 1.0f } }, // Vertex Data 2 - TR
   { { -5.0f,  0.0f, -5.0f }, { 1.0f, 1.0f, 1.0f}, { 0.0f, 0.0f } }, // Vertex Data 3 - BL
   { {  5.0f,  0.0f, -5.0f }, { 1.0f, 1.0f, 1.0f}, { 1.0f, 0.0f } }  // Vertex Data 4 - BR
};

const std::vector<uint32_t> planeIData = {
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
	// Create the mesh
	TUnique<EMesh> mesh = TMakeUnique<EMesh>();

	// Create the mesh and test if it failed
	if (!mesh->CreateMesh(cubeVData, cubeIData)) {
		EDebug::Log("Failed to create the cube mesh.");
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
	}

	// Add the texture to the mesh and add it to the mesh stack
	mesh->SetTexture(texture);
	// std::move will move the reference without destroying or copying it
	m_meshStack.push_back(std::move(mesh));
}

void EModel::MakePlane(const TShared<ETexture>& texture)
{
	// Create the meshe
	TUnique<EMesh> mesh = TMakeUnique<EMesh>();

	// Create the mesh and test if it failed
	if (!mesh->CreateMesh(planeVData, planeIData)) {
		EDebug::Log("Failed to create the plane mesh.");
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

void EModel::TranslateLocal(glm::vec3 translation, glm::vec3 scale)
{
	// Move the input direction forward if required
	glm::vec3 moveDir = m_transform.Forward() * translation.z;
	moveDir += m_transform.Right() * translation.x;
	moveDir.y += translation.y;

	if (glm::length(moveDir) != 0.0f)
		moveDir = glm::normalize(moveDir);

	m_transform.position += moveDir * scale;
}

void EModel::TranslateWorld(glm::vec3 translation, glm::vec3 scale)
{
	// Normalise the move directon
	glm::vec3 moveDir = translation;

	if (glm::length(moveDir) != 0.0f)
		moveDir = glm::normalize(moveDir);

	m_transform.position += moveDir * scale;
}
