#pragma once
#include "EngineTypes.h"

// External Libs
#include <GLM/mat4x4.hpp>

class EShaderProgram;
class ETexture;
struct ESTransform;
struct ESLight;

struct ESVertexData {
	// 0 = x
	// 1 = y
	// 2 = z
	float m_position[3] = { 0.0f, 0.0f, 0.0f };
	// 0 = r
	// 1 = g
	// 2 = b
	float m_color[3] = { 1.0f, 1.0f, 1.0f };
	// 0 = x, u, s
	// 1 = y, v, t
	float m_texCoords[2] = { 0.0f, 0.0f };
	// 0 = x
	// 1 = y
	// 2 = z
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
	void Render(const std::shared_ptr<EShaderProgram>& shader, const ESTransform& transform, 
		const TArray<TShared<ESLight>>& lights);

	// Set the texture for the mesh
	void SetTexture(const TShared<ETexture>& texture) { m_texture = texture; }

	// Set the transform of the mesh relative to the model
	void SetRelativeTransform(const glm::mat4 &transform) { m_matTransform = transform; }

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

	// Relative transform of the mesh
	glm::mat4 m_matTransform;
};