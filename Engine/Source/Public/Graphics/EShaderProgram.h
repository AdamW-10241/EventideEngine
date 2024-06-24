#pragma once
#include "EngineTypes.h"

class ETexture;
struct ESCamera;

// Enum to determine the type of shader
enum EEShaderType : EUi8 {
	ST_VERTEX = 0U,
	ST_FRAGMENT
};

struct ESTransform;

class EShaderProgram {
public:
	EShaderProgram();
	~EShaderProgram();

	// Create the shader using a vertex and fragment file
	bool InitShader(const EString& vShaderPath,
		const EString& fShaderPath);

	// Activate the shader to update
	// You can't change values in a shader without activating it
	void Activate();

	// Set the transform of the model in the shader
	void SetModelTransform(const ESTransform& transform);

	// Set the 3D coordinates for the model
	void SetWorldTransform(const TShared<ESCamera>& camera);

	// Set a texture in the shader based on the slot
	void RunTexture(const TShared<ETexture>& texture, const EUi32& slot);

private:
	// Import a shader based on the shader type
	bool ImportShaderByType(const EString& filePath, EEShaderType shaderType);

	// Convert a file into a string
	EString ConvertFileToString(const EString& filePath);

	// Link the shader to the GPU through OpenGL
	bool LinkToGPU();

private:
	// Store the file paths
	EString m_filePath[2] = { "", "" };

	// Store the shader IDs
	EUi32 m_shaderIDs[2] = { 0, 0 };

	// Store the ID for the program
	EUi32 m_programID;
};