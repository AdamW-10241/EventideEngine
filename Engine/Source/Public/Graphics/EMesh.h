#pragma once
#include "EngineTypes.h"

class EShaderProgram;
struct ESTransform;
class ETexture;

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
	void Render(const std::shared_ptr<EShaderProgram>& shader, const ESTransform& transform);

	// Set the texture for the mesh
	void SetTexture(const TShared<ETexture>& texture) { m_texture = texture; }

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

	// Texture for the mesh
	TShared<ETexture> m_texture;
};