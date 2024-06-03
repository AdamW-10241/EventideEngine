#pragma once

// System Libs
#include <iostream>
#include <vector>
#include <cstdint>

struct ESVertexData {
	float m_position[3] = { 0.0f, 0.0f, 0.0f };
	float m_color[3] = { 1.0f, 1.0f, 1.0f };
	float m_texCoords[2] = { 0.0f, 0.0f };
	float m_normal[3] = { 0.0f, 0.0f, 0.0f };
};

class EMesh {
public:
	EMesh();
	~EMesh();

	// Create a mesh using vertex and index data
	bool CreateMesh(const std::vector<ESVertexData>& vertices,
		const std::vector<uint32_t>& indices);

	// Draw the mesh to the renderer
	void Render();

private:
	// Store the vertices
	std::vector<ESVertexData> m_vertices;

	// Store the indices for the vertex data
	std::vector<uint32_t> m_indices;

	// Store the ID for the vertex array object
	uint32_t m_vao;

	// Store the ID for the vertex buffer object
	uint32_t m_vbo;

	// Store the ID for the element array object
	uint32_t m_eao;
};