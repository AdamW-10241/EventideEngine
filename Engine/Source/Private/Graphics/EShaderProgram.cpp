#include "Graphics/EShaderProgram.h"
#include "Debug/EDebug.h"
#include "Math/ESTransform.h"
#include "Graphics/ETexture.h"
#include "Graphics/ESCamera.h"
#include "Graphics/ESLight.h"
#include "Graphics/ESMaterial.h"

// External Libs
#include <GLEW/glew.h>
#include <GLM/gtc/type_ptr.hpp>

// System Libs
#include <fstream>
#include <sstream>

// Constant value for light amounts
const EUi32 maxDirLights = 2;
const EUi32 maxPointLights = 20;
const EUi32 maxSpotLights = 20;

#define EGET_GLEW_ERROR reinterpret_cast<const char*>(glewGetErrorString(glGetError()))

EShaderProgram::EShaderProgram()
{
	m_programID = 0;
	m_defaultTextureDepth = 1.0f;
	m_textureDepth = m_defaultTextureDepth;
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

void EShaderProgram::SetMeshTransform(const glm::mat4& matTransform)
{
	// Find the variable in the shader
	// All the uniform variables are given an ID by OpenGL
	const int varID = glGetUniformLocation(m_programID, "mesh");
	// Update the value
	glUniformMatrix4fv(
		varID, 1, GL_FALSE, glm::value_ptr(matTransform));
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
	glUniformMatrix4fv(
		varID, 1, GL_FALSE, glm::value_ptr(matrixT));
}

void EShaderProgram::SetWorldTransform(const TShared<ESCamera>& camera)
{
	// Initialise a matrix
	glm::mat4 matrixT = glm::mat4(1.0f);

	// Handle the view matrix
	// Translate and rotate the matrix based on the cameras forward and up vector
	matrixT = glm::lookAt(
		camera->transform.position,
		camera->transform.position + camera->transform.Forward(),
		camera->transform.Up()
	);

	// Find the variable in the shader for the view matrix
	int varID = glGetUniformLocation(m_programID, "view");

	// Update the view matrix value in the shader
	glUniformMatrix4fv(
		varID, 1, GL_FALSE, glm::value_ptr(matrixT));

	// Handle the projection matrix
	// Set the projection matrix to a perspective view
	matrixT = glm::perspective(
		glm::radians(camera->fov),	// Zoom of camera
		camera->aspectRatio,		// How wide the view is
		camera->nearClip,			// How close 3D models can be seen
		camera->farClip				// How far 3D models can be seen
	);								// - all other models will not render

	// Find the variable in the shader for the projection matrix
	varID = glGetUniformLocation(m_programID, "projection");

	// Update the projection matrix value in the shader
	glUniformMatrix4fv(
		varID, 1, GL_FALSE, glm::value_ptr(matrixT));
}

void EShaderProgram::SetLights(const TArray<TShared<ESLight>>& lights)
{	
	// Set number of lights in the shader
	// Max out to ensure lights can be added
	SetNumberOfLights((int)maxDirLights, (int)maxPointLights, (int)maxSpotLights);
	
	// Number of created lights
	EUi32 dirLights = 0;
	EUi32 pointLights = 0;
	EUi32 spotLights = 0;

	// Name of the variable array. Will set in the loop depending on light type
	EString lightIndexStr = "";
	int varID = 0;

	// Loop through all of the lights and add them to the shader
	for (EUi32 i = 0; i < lights.size(); ++i) {
		// ----------- DIRECTIONAL LIGHTS
		if (const auto& lightRef = std::dynamic_pointer_cast<ESDirLight>(lights[i])) {
			// Ignore dirLight if has reached max number
			if (dirLights >= maxDirLights)
				continue;
			
			// Add a dirLight and use as index
			lightIndexStr = "dirLights[" + std::to_string(dirLights) + "]";
			
			// --------- COLOR
			// Get the colour variable from the dirLight struct in the shader
			varID = glGetUniformLocation(m_programID,
				(lightIndexStr + ".colour").c_str());

			// Change the colour
			glUniform3fv(varID, 1, glm::value_ptr(lightRef->colour));

			// --------- AMBIENT
			// Get the ambient variable from the dirLight struct in the shader
			varID = glGetUniformLocation(m_programID,
				(lightIndexStr + ".ambient").c_str());

			// Change the ambient colour
			glUniform3fv(varID, 1, glm::value_ptr(lightRef->ambient));

			// --------- DIRECTION
			// Get the direction variable from the dirLight struct in the shader
			varID = glGetUniformLocation(m_programID,
				(lightIndexStr + ".direction").c_str());

			// Change the direction
			glUniform3fv(varID, 1, glm::value_ptr(lightRef->direction));

			// --------- INTENSITY
			// Get the intensity variable from the dirLight struct in the shader
			varID = glGetUniformLocation(m_programID,
				(lightIndexStr + ".intensity").c_str());

			// Change the intensity
			glUniform1f(varID, lightRef->intensity);

			// Increment the number of dirLights
			++dirLights;
			continue;
		}

		// ----------- POINT LIGHTS
		if (const auto& lightRef = std::dynamic_pointer_cast<ESPointLight>(lights[i])) {
			// Ensure only max lights
			if (pointLights >= maxPointLights) {
				continue;
			}
			
			// Add a point light and use as index
			lightIndexStr = "pointLights[" + std::to_string(pointLights) + "]";

			// --------- COLOR
			// Get the colour variable from the pointLight struct in the shader
			varID = glGetUniformLocation(m_programID,
				(lightIndexStr + ".colour").c_str());

			// Change the colour
			glUniform3fv(varID, 1, glm::value_ptr(lightRef->colour));

			// --------- POSITION
			// Get the position variable from the pointLight struct in the shader
			varID = glGetUniformLocation(m_programID,
				(lightIndexStr + ".position").c_str());

			// Change the position
			glUniform3fv(varID, 1, glm::value_ptr(lightRef->position));

			// --------- INTENSITY
			// Get the intensity variable from the pointLight struct in the shader
			varID = glGetUniformLocation(m_programID,
				(lightIndexStr + ".intensity").c_str());

			// Change the intensity
			glUniform1f(varID, lightRef->intensity);

			// --------- LINEAR ATTENUATION
			// Get the linear variable from the pointLight struct in the shader
			varID = glGetUniformLocation(m_programID,
				(lightIndexStr + ".linear").c_str());

			// Change the linear attenuation
			glUniform1f(varID, lightRef->linear);

			// --------- QUADRATIC ATTENUATION
			// Get the quadratic variable from the pointLight struct in the shader
			varID = glGetUniformLocation(m_programID,
				(lightIndexStr + ".quadratic").c_str());

			// Change the quadratic attenuation
			glUniform1f(varID, lightRef->quadratic);

			// Increment the number of pointLights
			++pointLights;
			continue;
		}

		// ----------- SPOT LIGHTS
		if (const auto& lightRef = std::dynamic_pointer_cast<ESSpotLight>(lights[i])) {
			// Ensure only max lights
			if (spotLights >= maxSpotLights) {
				continue;
			}

			// Add a spot light and use as index
			lightIndexStr = "spotLights[" + std::to_string(spotLights) + "]";

			// --------- COLOR
			// Get the colour variable from the spotLight struct in the shader
			varID = glGetUniformLocation(m_programID,
				(lightIndexStr + ".colour").c_str());

			// Change the colour
			glUniform3fv(varID, 1, glm::value_ptr(lightRef->colour));

			// --------- POSITION
			// Get the position variable from the spotLight struct in the shader
			varID = glGetUniformLocation(m_programID,
				(lightIndexStr + ".position").c_str());

			// Change the position
			glUniform3fv(varID, 1, glm::value_ptr(lightRef->position));

			// --------- DIRECTION
			// Get the direction variable from the spotLight struct in the shader
			varID = glGetUniformLocation(m_programID,
				(lightIndexStr + ".direction").c_str());

			// Change the direction
			glUniform3fv(varID, 1, glm::value_ptr(lightRef->direction));

			// --------- INTENSITY
			// Get the intensity variable from the spotLight struct in the shader
			varID = glGetUniformLocation(m_programID,
				(lightIndexStr + ".intensity").c_str());

			// Change the intensity
			glUniform1f(varID, lightRef->intensity);

			// --------- LINEAR ATTENUATION
			// Get the linear variable from the spotLight struct in the shader
			varID = glGetUniformLocation(m_programID,
				(lightIndexStr + ".linear").c_str());

			// Change the linear attenuation
			glUniform1f(varID, lightRef->linear);

			// --------- QUADRATIC ATTENUATION
			// Get the quadratic variable from the spotLight struct in the shader
			varID = glGetUniformLocation(m_programID,
				(lightIndexStr + ".quadratic").c_str());

			// Change the quadratic attenuation
			glUniform1f(varID, lightRef->quadratic);

			// Increment the number of spotLights
			++spotLights;
			continue;
		}
	}

	// Cap the values so unused lights are not drawn to the shader
	SetNumberOfLights((int)dirLights, (int)pointLights, (int)spotLights);
}

void EShaderProgram::SetNumberOfLights(const int& dirLights, const int& pointLights, const int& spotLights)
{
	int varID = 0;
	
	//  ---------- Set number of dir lights in shader
	// Get the addedDirLight variable in the shader
	varID = glGetUniformLocation(m_programID, "addedDirLights");
	// Set the number of added Dir Lights
	glUniform1i(varID, dirLights);

	//  ---------- Set number of point lights in shader
	// Get the addedPointLight variable in the shader
	varID = glGetUniformLocation(m_programID, "addedPointLights");
	// Set the number of added Point Lights
	glUniform1i(varID, pointLights);

	//  ---------- Set number of spot lights in shader
	// Get the addedSpotLight variable in the shader
	varID = glGetUniformLocation(m_programID, "addedSpotLights");
	// Set the number of added Spot Lights
	glUniform1i(varID, spotLights);
}

void EShaderProgram::SetMaterial(const TShared<ESMaterial>& material)
{
	if (material == nullptr)
		return;

	// ID for the variable in the shader
	int varID = 0;

	// --------- BASE COLOUR
	if (material->m_baseColourMap) {
		// Bind the texture to the 0 index
		material->m_baseColourMap->BindTexture(0);
		// Get the base colour map ID
		varID = glGetUniformLocation(m_programID, "material.baseColourMap");
		// Update the shader
		glUniform1i(varID, 0);
	}

	// --------- SPECULAR
	if (material->m_specularMap) {
		// Bind the texture to the 1 index
		material->m_specularMap->BindTexture(1);
		// Get the specular map ID
		varID = glGetUniformLocation(m_programID, "material.specularMap");
		// Update the shader
		glUniform1i(varID, 1);
	}

	// --------- NORMAL
	if (material->m_normalMap) {
		// Bind the texture to the 2 index
		material->m_normalMap->BindTexture(2);
		// Get the normal map ID
		varID = glGetUniformLocation(m_programID, "material.normalMap");
		// Update the shader
		glUniform1i(varID, 2);
	}

	// --------- SHININESS
	// Get the shininess ID
	varID = glGetUniformLocation(m_programID, "material.shininess");
	// Update the shader
	glUniform1f(varID, material->m_shininess);

	// --------- SPECULAR STRENGTH
	// Get the specular strength ID
	varID = glGetUniformLocation(m_programID, "material.specularStrength");
	// Update the shader
	glUniform1f(varID, material->m_specularStrength);

	// --------- BRIGHTNESS
	// Get the brightness ID
	varID = glGetUniformLocation(m_programID, "material.brightness");
	// Update the shader
	glUniform1f(varID, material->m_brightness);

	// --------- MATERIAL TEXTURE DEPTH
	// Get the material texture depth ID
	varID = glGetUniformLocation(m_programID, "materialTextureDepth");
	// Update the shader
	glUniform1f(varID, material->m_textureDepth);
}

void EShaderProgram::SetWireColour(const glm::vec3& colour)
{
	// Get the wire colour variable from the shader
	int varID = glGetUniformLocation(m_programID, "wireColour");

	// Change the colour
	glUniform3fv(varID, 1, glm::value_ptr(colour));
}

void EShaderProgram::SetBrightness(const float& brightness)
{
	// Get the brightness ID
	int varID = glGetUniformLocation(m_programID, "brightness");
	// Update the shader
	glUniform1f(varID, brightness);
}

void EShaderProgram::AdjustTextureDepth(float delta)
{
	// Find the variable in the shader
	// All the uniform variables are given an ID by OpenGL
	const int varID = glGetUniformLocation(m_programID, "textureDepth");
	// Update the value
	m_textureDepth += delta;
	glUniform1f(varID, m_textureDepth);
}

void EShaderProgram::ResetTextureDepth()
{
	// Find the variable in the shader
	// All the uniform variables are given an ID by OpenGL
	const int varID = glGetUniformLocation(m_programID, "textureDepth");
	// Update the value
	m_textureDepth = m_defaultTextureDepth;
	glUniform1f(varID, m_defaultTextureDepth);
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
