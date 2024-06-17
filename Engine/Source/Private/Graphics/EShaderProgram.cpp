#include "Graphics/EShaderProgram.h"
#include "Debug/EDebug.h"
#include "Math/ESTransform.h"
#include "Graphics/ETexture.h"

// External Libs
#include <GLEW/glew.h>
#include <GLM/gtc/type_ptr.hpp>

// System Libs
#include <fstream>
#include <sstream>

#define EGET_GLEW_ERROR reinterpret_cast<const char*>(glewGetErrorString(glGetError()))

EShaderProgram::EShaderProgram()
{
	m_programID = 0;
}

EShaderProgram::~EShaderProgram()
{
	EDebug::Log("Shader program " + std::to_string(m_programID) + " destroyed.");
}

bool EShaderProgram::InitShader(const EString& vShaderPath, const EString& fShaderPath)
{
	// Create the shader program in OpenGL
	m_programID = glCreateProgram();

	// Test if the create program failed
	if (m_programID == 0) {
		const EString errorMsg = EGET_GLEW_ERROR;
		EDebug::Log("Shader program failed to initialise, could not create program: " + errorMsg);
		return false;
	}

	// If either of the shaders fail to import then fail the whole program
	if (!ImportShaderByType(vShaderPath, ST_VERTEX) || !ImportShaderByType(fShaderPath, ST_FRAGMENT)) {
		EDebug::Log("Shader program failed to initalise, could not import shaders.");
		return false;
	}
	
	return LinkToGPU();
}

void EShaderProgram::Activate()
{
	glUseProgram(m_programID);
}

void EShaderProgram::SetModelTransform(const ESTransform& transform)
{
	// Translate (move) > rotate > scale
	// This allows us to rotate around the new location
	// Initialise a default matrix transform
	glm::mat4 matrixT = glm::mat4(1.0f);

	// Translate matrix
	matrixT = glm::translate(matrixT, transform.position);

	// Rotate matrix
	matrixT = glm::rotate(matrixT, glm::radians(transform.rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	matrixT = glm::rotate(matrixT, glm::radians(transform.rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	matrixT = glm::rotate(matrixT, glm::radians(transform.rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

	// Scale matrix
	matrixT = glm::scale(matrixT, transform.scale);

	// Find the variable in the shader
	// All the uniform variables are given an ID by OpenGL
	const int varID = glGetUniformLocation(m_programID, "model");
	// Update the value
	glUniformMatrix4fv(varID, 1, GL_FALSE, glm::value_ptr(matrixT));
}

void EShaderProgram::RunTexture(const TShared<ETexture>& texture, const EUi32& slot)
{
	// Bind the texture
	texture->BindTexture(slot);

	// ID for the variable in the shader
	int varID = 0;

	// Get the ID depending on the slot
	switch (slot) {
	case 0:
		varID = glGetUniformLocation(m_programID, "colourMap");
		break;
	default:
		break;
	}

	// Update the shader
	glUniform1i(varID, slot);
}

bool EShaderProgram::ImportShaderByType(const EString& filePath, EEShaderType shaderType)
{
	// Convert the shader to a string
	const EString shaderStr = ConvertFileToString(filePath);
	
	// Make sure there is a string path
	if (shaderStr.empty()) {
		EDebug::Log("Shader failed to import.", LT_ERROR);
		return false;
	}

	// Set and create and ID for the shader based on the shader type
	switch (shaderType)
	{
	case ST_VERTEX:
		m_shaderIDs[shaderType] = glCreateShader(GL_VERTEX_SHADER);
		break;
	case ST_FRAGMENT:
		m_shaderIDs[shaderType] = glCreateShader(GL_FRAGMENT_SHADER);
		break;
	default:
		break;
	}

	// Make sure there was a shader ID assigned
	if (m_shaderIDs[shaderType] == 0) {
		const EString errorMsg = EGET_GLEW_ERROR;
		EDebug::Log("Shader program could not assign shader ID: " + errorMsg, LT_ERROR);
		return false;
	}

	// Compile the shader onto the GPU
	const char* shaderCStr = shaderStr.c_str();
	glShaderSource(m_shaderIDs[shaderType], 1, &shaderCStr, nullptr);
	glCompileShader(m_shaderIDs[shaderType]);

	// Test if the compile worked
	GLint success;
	glGetShaderiv(m_shaderIDs[shaderType], GL_COMPILE_STATUS, &success);

	if (!success) {
		// Create an empty log
		char infoLog[512];
		// Fill the log with info from GL about what happened
		glGetShaderInfoLog(m_shaderIDs[shaderType], 512, nullptr, infoLog);
		// Log info
		EDebug::Log("Shader compilation error: " + EString(infoLog), LT_ERROR);
		return false;
	}

	// Attach the shader to the program ID
	glAttachShader(m_programID, m_shaderIDs[shaderType]);

	return true;
}

EString EShaderProgram::ConvertFileToString(const EString& filePath)
{
	// Convert the file path into an ifstream
	std::ifstream shaderSource(filePath);

	// Test if we can open the file
	if (!shaderSource.is_open()) {
		EDebug::Log("Failed to open the file: " + filePath, LT_ERROR);
		return {};
	}
	
	// Initialise a sstream
	std::stringstream shaderStream;

	// Convert the fstream into an sstream
	// Turns the file into a string readable by our code
	shaderStream << shaderSource.rdbuf();

	// Close the file since we do not need it anymore
	shaderSource.close();

	return shaderStream.str();
}

bool EShaderProgram::LinkToGPU()
{
	// Link the program to the GPU
	glLinkProgram(m_programID);
	
	// Test if the link worked
	GLint success;
	glGetProgramiv(m_programID, GL_LINK_STATUS, &success);

	if (!success) {
		// Create an empty log
		char infoLog[512];
		// Fill the log with info from GL about what happened
		glGetShaderInfoLog(m_programID, 512, nullptr, infoLog);
		// Log info
		EDebug::Log("Shader link error: " + EString(infoLog), LT_ERROR);
		return false;
	}

	EDebug::Log("Shader successfully initialised and linked at index " + std::to_string(m_programID));

	return true;
}
