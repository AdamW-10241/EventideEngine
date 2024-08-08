#include "Graphics/EMesh.h"
#include "Debug/EDebug.h"
#include "Graphics/EShaderProgram.h"

// External Libs
#include <GLEW/glew.h>

EMesh::EMesh()
{
	m_vao = m_vbo = m_eao = 0;
	m_matTransform = glm::mat4(1.0f);
	materialIndex = 0;
}

EMesh::~EMesh() {}

bool EMesh::CreateMesh(const std::vector<ESVertexData>& vertices, const std::vector<uint32_t>& indices)
{
	// Store the vertex data
	m_vertices = vertices;
	m_indices = indices;

	// Calculate the tangents
	CalculateTangents(m_vertices, m_indices);

	// Create a vertex array object (VAO)
	// Assign the ID for object to the m_vao variable
	// Stores a reference to any VBO's attached to the VAO
	glGenVertexArrays(1, &m_vao);

	// Test if the VAO failed
	if (m_vao == 0) {
		// Convert the error into a readable string
		std::string errorMsg = reinterpret_cast<const char*>(glewGetErrorString(glGetError()));
		EDebug::Log("Mesh failed to create VAO: " + errorMsg, LT_WARNING);
		return false;
	}

	// Bind the VAO as the active working VAO for any VAO functions
	glBindVertexArray(m_vao);

	// Create a vertex buffer object (VBO)
	// Vertex Buffer Object holds the data for the vertices in the GPU
	glGenBuffers(1, &m_vbo);

	// Test if the VBO failed
	if (m_vbo == 0) {
		// Convert the error into a readable string
		std::string errorMsg = reinterpret_cast<const char*>(glewGetErrorString(glGetError()));
		EDebug::Log("Mesh failed to create VBO: " + errorMsg, LT_WARNING);
		return false;
	}

	// Bind the VBO as the active working VBO for any VBO functions
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

	// Create an Element Array Buffer
	glGenBuffers(1, &m_eao);

	// Test if the EAO failed
	if (m_eao == 0) {
		// Convert the error into a readable string
		std::string errorMsg = reinterpret_cast<const char*>(glewGetErrorString(glGetError()));
		EDebug::Log("Mesh failed to create EAO: " + errorMsg, LT_WARNING);
		return false;
	}

	// Bind the EAO as the active working EAO for any EAO functions
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_eao);

	// Set the buffer data
	// Start with the VBO which stores the vertex data
	glBufferData(
		GL_ARRAY_BUFFER, // Type of data stored
		static_cast<GLsizeiptr>(m_vertices.size() * sizeof(ESVertexData)), // Size of each data in bytes
		m_vertices.data(), // Memory location of data
		GL_STATIC_DRAW // Data will not be modified
	);

	// Set the data for the EAO
	glBufferData(
		GL_ELEMENT_ARRAY_BUFFER,
		static_cast<GLsizeiptr>(m_indices.size() * sizeof(uint32_t)),
		m_indices.data(),
		GL_STATIC_DRAW
	);

	// Position
	// Pass out the vertex data in seperate formats
	glEnableVertexAttribArray(0);

	// Set the position of that data to the 0 index of the attribute array
	glVertexAttribPointer(
		0, // Location to store the data in the attribute array
		3, // How many numbers to pass into the attribute array index
		GL_FLOAT, // The type of data to store
		GL_FALSE, // Should we normalise the values (generally no)
		sizeof(ESVertexData), // How big is each data array in a VertexData
		nullptr // How many numbers to skip in bytes
	);

	// Color
	// Pass out the vertex data in seperate formats
	glEnableVertexAttribArray(1);

	// Set the position of that data to the 1 index of the attribute array
	glVertexAttribPointer(
		1, // Location to store the data in the attribute array
		3, // How many numbers to pass into the attribute array index
		GL_FLOAT, // The type of data to store
		GL_FALSE, // Should we normalise the values (generally no)
		sizeof(ESVertexData), // How big is each data array in a VertexData
		(void*)(sizeof(float) * 3) // How many numbers to skip in bytes
	);

	// Tex Coords
	// Pass out the vertex data in seperate formats
	glEnableVertexAttribArray(2);

	// Set the position of that data to the 2 index of the attribute array
	glVertexAttribPointer(
		2, // Location to store the data in the attribute array
		2, // How many numbers to pass into the attribute array index
		GL_FLOAT, // The type of data to store
		GL_FALSE, // Should we normalise the values (generally no)
		sizeof(ESVertexData), // How big is each data array in a VertexData
		(void*)(sizeof(float) * 6) // How many numbers to skip in bytes
	);

	// Normals
	// Pass out the vertex data in seperate formats
	glEnableVertexAttribArray(3);

	// Set the position of that data to the 3 index of the attribute array
	glVertexAttribPointer(
		3, // Location to store the data in the attribute array
		3, // How many numbers to pass into the attribute array index
		GL_FLOAT, // The type of data to store
		GL_FALSE, // Should we normalise the values (generally no)
		sizeof(ESVertexData), // How big is each data array in a VertexData
		(void*)(sizeof(float) * 8) // How many numbers to skip in bytes
	);

	// Tangents
	// Pass out the tangent data in seperate formats
	glEnableVertexAttribArray(4);

	// Set the position of that data to the 4 index of the attribute array
	glVertexAttribPointer(
		4, // Location to store the data in the attribute array
		3, // How many numbers to pass into the attribute array index
		GL_FLOAT, // The type of data to store
		GL_FALSE, // Should we normalise the values (generally no)
		sizeof(ESVertexData), // How big is each data array in a VertexData
		(void*)(sizeof(float) * 11) // How many numbers to skip in bytes
	);

	// Bit Tangents
	// Pass out the vertex data in seperate formats
	glEnableVertexAttribArray(5);

	// Set the position of that data to the 5 index of the attribute array
	glVertexAttribPointer(
		5, // Location to store the data in the attribute array
		3, // How many numbers to pass into the attribute array index
		GL_FLOAT, // The type of data to store
		GL_FALSE, // Should we normalise the values (generally no)
		sizeof(ESVertexData), // How big is each data array in a VertexData
		(void*)(sizeof(float) * 14) // How many numbers to skip in bytes
	);

	// Common practice to clear the VAO from the GPU
	glBindVertexArray(0);

	return true;
}

void EMesh::Render(const std::shared_ptr<EShaderProgram>& shader, const ESTransform& transform, 
	const TArray<TShared<ESLight>>& lights, const TShared<ESMaterial>& material)
{
	// Update the material in the shader
	shader->SetMaterial(material);

	// Update the transform of the mesh based on the model transform
	shader->SetModelTransform(transform);
	
	// Set the relative transform for the mesh in the shader
	shader->SetMeshTransform(m_matTransform);

	// Set the lights in the shader for the mesh
	shader->SetLights(lights);

	// Binding this mesh as the active VAO
	glBindVertexArray(m_vao);

	// Render the VAO
	glDrawElements(
		GL_TRIANGLES, // Draw the mesh as triangles
		static_cast<GLsizei>(m_indices.size()), // How many vertices are there
		GL_UNSIGNED_INT, // What type of data is the index array
		nullptr // How many vertices are skipped
	);

	// Clear the VAO
	glBindVertexArray(0);
}

void EMesh::CalculateTangents(std::vector<ESVertexData>& vertices, std::vector<uint32_t>& indices)
{
	// Used learnOpenGL to make this code
	// LearnOpenGL 2024, Normal Mapping, viewed August 9, https://learnopengl.com/Advanced-Lighting/Normal-Mapping
	// Loop through every 3 indices for each triangle
	for (int i = 0; i < indices.size(); i += 3) {
		// Get each vertex
		ESVertexData& vertex0 = vertices[indices[i]];
		ESVertexData& vertex1 = vertices[indices[i + 1]];
		ESVertexData& vertex2 = vertices[indices[i + 2]];

		// Get vec3 positions since the positions are stored as float arrays
		glm::vec3 position0(vertex0.m_position[0], vertex0.m_position[1], vertex0.m_position[2]);
		glm::vec3 position1(vertex1.m_position[0], vertex1.m_position[1], vertex1.m_position[2]);
		glm::vec3 position2(vertex2.m_position[0], vertex2.m_position[1], vertex2.m_position[2]);

		// Get vec2 texCoords since the texCoords are stored as float arrays
		glm::vec2 texCoords0(vertex0.m_texCoords[0], vertex0.m_texCoords[1]); 
		glm::vec2 texCoords1(vertex1.m_texCoords[0], vertex1.m_texCoords[1]);
		glm::vec2 texCoords2(vertex2.m_texCoords[0], vertex2.m_texCoords[1]);

		// Get the edges and deltaUVs from the positions and texture coordinates
		// These will be used to find the tangent and bit tangents
		glm::vec3 edge1 = position1 - position0;
		glm::vec3 edge2 = position2 - position0;
		glm::vec2 deltaUV1 = texCoords1 - texCoords0;
		glm::vec2 deltaUV2 = texCoords2 - texCoords0;

		// Calculate the tangents and bit tangents
		float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

		glm::vec3 tangent(0.0f);
		tangent.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
		tangent.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
		tangent.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
		tangent = glm::normalize(tangent);

		glm::vec3 bitTangent(0.0f);
		bitTangent.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
		bitTangent.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
		bitTangent.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
		bitTangent = glm::normalize(bitTangent);

		// Copy the tangent values to each vertex
		vertex0.m_tangent[0] = tangent.x; vertex0.m_tangent[1] = tangent.y; vertex0.m_tangent[2] = tangent.z;
		vertex1.m_tangent[0] = tangent.x; vertex1.m_tangent[1] = tangent.y; vertex1.m_tangent[2] = tangent.z;
		vertex2.m_tangent[0] = tangent.x; vertex2.m_tangent[1] = tangent.y; vertex2.m_tangent[2] = tangent.z;

		// Copy the bitTangent values to each vertex
		vertex0.m_bitTangent[0] = tangent.x; vertex0.m_bitTangent[1] = tangent.y; vertex0.m_bitTangent[2] = tangent.z;
		vertex1.m_bitTangent[0] = tangent.x; vertex1.m_bitTangent[1] = tangent.y; vertex1.m_bitTangent[2] = tangent.z;
		vertex2.m_bitTangent[0] = tangent.x; vertex2.m_bitTangent[1] = tangent.y; vertex2.m_bitTangent[2] = tangent.z;
	}
}
