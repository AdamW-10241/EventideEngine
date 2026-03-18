#include "Graphics/EText.h"
#include <GLEW/glew.h>
#include <STB_IMAGE/stb_image.h>
#include <SDL/SDL_ttf.h>

#define Super ESprite

EText::EText(const ESAddSpriteConfig config) : ESprite(config)
{
	m_font = nullptr;
	m_surfaceData = nullptr;
	m_textColor = new SDL_Color({ 255, 255, 255, 255 });
	m_text = "Text";
	SetDefaultFontSize(16);
	m_repeat = false;
	m_linear = false;

	// Load texture
	LoadTexture(config.texturePath, config.texturePath, m_repeat, m_linear);
}

EText::~EText()
{
	CleanupFont();

	if (m_textColor != nullptr) {
		delete m_textColor;
		m_textColor = nullptr;
	}
}

bool EText::LoadTexture(const EString& fileName, const EString& path, bool repeat, bool linear)
{
	// Assign the file name and path
	m_fileName = fileName;
	m_repeat = repeat;
	m_linear = linear;
	
	// Only reload font if path or size changed
	if (m_path != path || m_font == nullptr) {
		m_path = path;
		CleanupFont();
		// Import the font
		m_font = TTF_OpenFont(m_path.c_str(), m_fontSize);

		if (m_font == nullptr) {
			EDebug::Log("Failed to load font - " + m_fileName + ": " + EString(TTF_GetError()), LT_ERROR);
			return false;
		}
	}

	// Import the image and convert it to a surface
	// Fill in the object with the data for the image
	SDL_Surface* surf = TTF_RenderText_Blended(m_font, m_text.c_str(), *m_textColor);
	SDL_Surface* converted = SDL_ConvertSurfaceFormat(surf, SDL_PIXELFORMAT_RGBA32, 0);
	SDL_FreeSurface(surf);
	surf = converted;

	// Set dimensions so SetScaleToTextureSize works correctly
	m_width = surf->w;
	m_height = surf->h;
	
	// Check if the import failed
	if (surf == nullptr) {
		EString errorMsg = "Failed to convert font to surface - " + m_fileName + ": " + EString(TTF_GetError());
		EDebug::Log(errorMsg, LT_ERROR);
		// Clear Surface Image data
		CleanupSurface(surf);
		return false;
	}
	//SDL_SaveBMP(surf, "debug_text.bmp");

	// Generate the texture ID in OpenGL
	glGenTextures(1, &m_ID);

	// Test if the generate failed
	if (m_ID == 0) {
		EString error = reinterpret_cast<const char*>(glewGetErrorString(glGetError()));
		EString errorMsg = "Failed to generate texture ID - " + m_fileName + ": " + error;
		EDebug::Log(errorMsg, LT_ERROR);
		// Clear Surface Image data
		CleanupSurface(surf);
		return false;
	}

	// Bind the texture
	// Tells OpenGL that we want to use this texture
	glBindTexture(GL_TEXTURE_2D, m_ID);

	// Set default parameters for the texture
	// Set the texture wrapping parameters
	// If the texture does not fit the model, repeat texture
	GLint wrapMode = repeat ? GL_REPEAT : GL_CLAMP_TO_EDGE;
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapMode);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapMode);

	// Set the filtering parameters
	// How much to blur pixels 
	// The resolution of the texture is lower than the size of the model
	GLint filter = linear ? GL_LINEAR : GL_NEAREST;
	GLint minFilter = linear ? GL_LINEAR_MIPMAP_LINEAR : GL_NEAREST;
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
	
	GLint intFormat = GL_RGBA;

	// Create the texture from the surface
	glTexImage2D(
		GL_TEXTURE_2D,      // Use a 2D Texture
		0,                  // Levels
		intFormat,          // Internal Texture format
		surf->w, surf->h,	// Width & height
		0,                  // Image border (legacy)
		intFormat,          // External texture format
		GL_UNSIGNED_BYTE,   // Data type passed in
		surf->pixels        // Surface Data from TTF Render Font
	);

	// Generate mip maps
	// Lower resolutions versions of texture
	glGenerateMipmap(GL_TEXTURE_2D);

	// Unbind the texture from OpenGL
	// Makes room for next texture
	Unbind();

	// Clear Surface Image data
	CleanupSurface(surf);

	return true;
}

void EText::CleanupSurface(SDL_Surface* surface)
{
	if (surface != nullptr) {
		SDL_FreeSurface(surface);
	}
}

void EText::CleanupFont()
{
	if (m_font != nullptr && TTF_WasInit()) {
		TTF_CloseFont(m_font);
		m_font = nullptr;
	}
}

void EText::SetText(EString newText)
{
	// Compare value and return if same
	if (newText == m_text) return;

	// Copy value
	m_text = newText;

	UpdateFont();
}

void EText::SetFontSize(int newSize)
{
	if (m_fontSize == newSize) return;
	m_fontSize = newSize;
	CleanupFont(); // force reload on next LoadTexture
	UpdateFont();
}

void EText::SetFontSizeMulti(float multi)
{
	int newSize = (int)((float)m_defaultFontSize * multi);
	SetFontSize(newSize);
}

void EText::SetDefaultFontSize(int newSize, bool setFontSize)
{
	m_defaultFontSize = newSize;
	if (setFontSize) m_fontSize = m_defaultFontSize;
}

void EText::SetFontColor(SDL_Color color)
{
	if (m_textColor == &color) {
		return;
	}

	m_textColor->r = color.r;
	m_textColor->g = color.g;
	m_textColor->b = color.b;
	m_textColor->a = color.a;

	UpdateFont();
}

void EText::UpdateFont()
{
	if (m_path.empty()) return;

	// Load new texture
	LoadTexture(m_fileName, m_path, m_repeat, m_linear);

	// Adjust scale
	if (m_transform.scale != glm::vec2(0.0f)) {
		SetScaleToTextureSize();
	}
}
