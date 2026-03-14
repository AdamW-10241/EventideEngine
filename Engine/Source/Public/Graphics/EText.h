#pragma once
#include "Graphics/ESprite.h"

typedef struct _TTF_Font TTF_Font;
struct SDL_Color;
struct SDL_Surface;

class EText : public ESprite {
public:
	EText() : EText(ESTransform2D(), 0, glm::vec4(1.0f)) {}
	~EText();

	EText(const ESTransform2D transform, const EUi32 renderOrder, const glm::vec4 renderColor = glm::vec4(1.0f))
		: EText("", transform, renderOrder, renderColor) {
	}

	EText(const EString& texturePath, const ESTransform2D transform, const EUi32 renderOrder, const glm::vec4 renderColor = glm::vec4(1.0f));

	// Import a file and convert it to a texture
	virtual bool LoadTexture(const EString& fileName, const EString& path, bool repeat = true, bool linear = true) override;

	// Cleanup surface
	void CleanupSurface(SDL_Surface* surface);

	void CleanupFont();

	// Change the text of the text
	void SetText(EString NewText);

	EString GetText() const { return m_text; }

	// Change the size of the text
	void SetFontSize(int NewSize);

	int GetFontSize() const { return m_fontSize; }

	// Chang ethe color of the font
	void SetFontColor(SDL_Color Color);

	SDL_Color* GetFontColor() const { return m_textColor; }
	

protected:
	void UpdateFont();

private:
	// TTF font
	TTF_Font* m_font;

	// SDL surface with pixel data
	SDL_Surface* m_surfaceData;

	// Color of the font
	SDL_Color* m_textColor;
	
	// Text to render using the font
	EString m_text;

	// Size of the font
	int m_fontSize;

	// Load texture flags stored for easy rerender
	bool m_repeat;
	bool m_linear;
};