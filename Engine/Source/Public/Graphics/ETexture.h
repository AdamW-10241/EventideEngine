#pragma once
#include "EngineTypes.h"

class ETexture {
public:
	ETexture();
	~ETexture();

	// Import a file and convert it to a texture
	virtual bool LoadTexture(const EString& path, bool repeat = true, bool linear = true);

	// Activates the texture to use for OpenGL
	void BindTexture(const EUi32& textureNumber);

	// Deactivates the texture for OpenGL
	void Unbind();

	// Gets the path of the file used by texture
	EString GetPath() const { return m_path; }

	// Gets the ID of the texture for OpenGL
	EUi32 GetID() const { return m_ID; }

	// Get the number of channels
	int GetChannels() const { return m_channels; }
	
protected:
	// Import path of the image
	EString m_path;

	// ID for the texture in OpenGL
	EUi32 m_ID;

	// Texture parameters
	int m_width, m_height, m_channels;
};