#pragma once

// System Libs
#include <string>
#include <cstdint>

// Enum to determine the type of shader
enum EEShaderType : uint8_t {
	ST_VERTEX = 0U,
	ST_FRAGMENT
};

struct ESTransform;

class EShaderProgram {
public:
	EShaderProgram();
	~EShaderProgram();

	// Create the shader using a vertex and fragment file
	bool InitShader(const std::string& vShaderPath,
		const std::string& fShaderPath);

	// Activate the shader to update
	// You can't change values in a shader without activating it
	void Activate();

	// Set the transform of the model in the shader
	void SetModelTransform(const ESTransform& transform);

private:
	// Import a shader based on the shader type
	bool ImportShaderByType(const std::string& filePath, EEShaderType shaderType);

	// Convert a file into a string
	std::string ConvertFileToString(const std::string& filePath);

	// Link the shader to the GPU through OpenGL
	bool LinkToGPU();

private:
	// Store the file paths
	std::string m_filePath[2] = { "", "" };

	// Store the shader IDs
	uint32_t m_shaderIDs[2] = { 0, 0 };

	// Store the ID for the program
	uint32_t m_programID;
};