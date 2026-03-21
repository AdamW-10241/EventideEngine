#include "Graphics/EText.h"
#include "Game/EGameEngine.h"

#include <GLEW/glew.h>
#include <STB_IMAGE/stb_image.h>
#include <SDL/SDL_ttf.h>

#define Super ESprite

EText::EText(const ESAddTextConfig& config) : Super(config)
{
	m_font = nullptr;
	m_textColor = new SDL_Color{ 255, 255, 255, 255 };
	SetDefaultFontSize(config.fontSize);
	m_text = config.text;
	m_repeat = false;
	m_linear = false;

	// Load texture
	if (!m_text.empty())
		LoadTexture(config.path, m_repeat, m_linear);

	UpdateFont();
}

EText::~EText()
{
	CleanupFont();

	if (m_textColor != nullptr) {
		delete m_textColor;
		m_textColor = nullptr;
	}
}

bool EText::LoadTexture(const EString& path, bool repeat, bool linear)
{
	// Assign the file name and path
	m_repeat = repeat;
	m_linear = linear;
	
	// Only reload font if path or size changed
	if (m_path != path || m_font == nullptr) {
		m_path = path;
		CleanupFont();
		// Import the font
		m_font = TTF_OpenFont(m_path.c_str(), m_fontSize);

		if (m_font == nullptr) {
			EDebug::Log("Failed to load font - " + m_path + ": " + EString(TTF_GetError()), LT_ERROR);
			return false;
		}
	}

	// Import the image and convert it to a surface
	// Fill in the object with the data for the image
	SDL_Surface* surf = TTF_RenderText_Blended(m_font, m_text.c_str(), *m_textColor);
	SDL_Surface* converted = SDL_ConvertSurfaceFormat(surf, SDL_PIXELFORMAT_RGBA32, 0);
	SDL_FreeSurface(surf);
	surf = converted;

	// Set dimensions
	m_width = surf->w;
	m_height = surf->h;
	
	// Check if the import failed
	if (surf == nullptr) {
		EString errorMsg = "Failed to convert font to surface - " + m_path + ": " + EString(TTF_GetError());
		EDebug::Log(errorMsg, LT_ERROR);
		// Clear Surface Image data
		CleanupSurface(surf);
		return false;
	}
	//SDL_SaveBMP(surf, "debug_text.bmp");

	// Delete old texture if exists
	if (m_ID > 0) {
		glDeleteTextures(1, &m_ID);
		m_ID = 0;
	}
	glGenTextures(1, &m_ID);

	// Test if the generate failed
	if (m_ID == 0) {
		EString error = reinterpret_cast<const char*>(glewGetErrorString(glGetError()));
		EString errorMsg = "Failed to generate texture ID - " + m_path + ": " + error;
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

void EText::UpdateTransform(glm::vec2 windowSize)
{
	float slateUnit = windowSize.y / SLATE_UNIT_SCALAR;

	// Only scale font size if m_doScale is true
	int newSize = m_doScale
		? (int)(m_defaultFontSize * slateUnit)
		: m_defaultFontSize;

	if (newSize != m_fontSize) {
		m_fontSize = newSize;
		CleanupFont();
		LoadTexture(m_path, m_repeat, m_linear);
	}

	glm::vec2 flippedAnchor = { m_anchor.x, 1.0f - m_anchor.y };
	glm::vec2 anchorPos = flippedAnchor * windowSize;

	glm::vec2 pixelSize = m_sizeInUnits != glm::vec2(0.0f)
		? CalcPixelSize(m_doScale ? slateUnit : 1.0f)
		: CalcPixelSize(1.0f);

	glm::vec2 pixelPos = anchorPos - pixelSize * m_alignment;
	m_transform.position = pixelPos;
	m_transform.scale = pixelSize;
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
	auto window = EGameEngine::GetGameEngine()->GetWindow().lock();
	if (!window) return;

	int newSize = (int)(m_defaultFontSize * window->GetSlateUnit());
	if (newSize != m_fontSize) {
		m_fontSize = newSize;
		CleanupFont();
	}

	// Load new texture
	LoadTexture(m_path, m_repeat, m_linear);
	Super::UpdateTransform();
}
