#pragma once
#include "EngineTypes.h"

class ETexture {
public:
	ETexture();
	~ETexture();

	// Import a file and convert it to a texture
	bool LoadTexture(const EString& fileName, const EString& path, bool repeat = true, bool linear = true);

	// Activates the texture to use for OpenGL
	void BindTexture(const EUi32& textureNumber);

	// Deactivates the texture for OpenGL
	void Unbind();

	// Gets the import path of the texture
	EString GetImportPath() const { return m_path; }

	// Gets the custom file name of the texture
	EString GetName() const { return m_fileName; }

	// Gets the ID of the texture for OpenGL
	EUi32 GetID() const { return m_ID; }
	
protected:
	// Import path of the image
	EString m_path;

	// Custome name of the texture
	EString m_fileName;

	// ID for the texture in OpenGL
	EUi32 m_ID;

	// Texture parameters
	int m_width, m_height, m_channels;
};