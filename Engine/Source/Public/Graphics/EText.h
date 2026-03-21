#pragma once
#include "Graphics/ESprite.h"

typedef struct _TTF_Font TTF_Font;
struct SDL_Color;
struct SDL_Surface;

#define FONT_PRESS_START "Fonts/Press_Start_2P/PressStart2P-Regular.ttf"

#define DEFAULT_TEXT "Text"
#define DEFAULT_FONT FONT_PRESS_START
#define DEFAULT_FONT_SIZE 28

struct ESAddTextConfig : public ESAddSpriteConfigBase<ESAddTextConfig>{
	EString path = DEFAULT_FONT;
	EString text = DEFAULT_TEXT;
	int fontSize = DEFAULT_FONT_SIZE;

	ESAddTextConfig& SetPath(EString _path) { path = _path; return *this; }
	ESAddTextConfig& SetText(EString _text) { text = _text; return *this; }
	ESAddTextConfig& SetFontSize(int _fontSize) { fontSize = _fontSize; return *this; }
};

class EText : public ESprite {
public:
	EText(const ESAddTextConfig& config);
	~EText();

	// Import a file and convert it to a texture
	virtual bool LoadTexture(const EString& path, bool repeat = true, bool linear = true) override;

	virtual void UpdateTransform(glm::vec2 windowSize) override;

	// Cleanup surface
	void CleanupSurface(SDL_Surface* surface);

	void CleanupFont();

	// Change the text of the text
	void SetText(EString newText);

	EString GetText() const { return m_text; }

	// Change the size of the text
	void SetFontSize(int newSize);

	void SetFontSizeMulti(float multi);

	void SetDefaultFontSize(int newSize, bool setFontSize = true);

	int GetFontSize() const { return m_fontSize; }

	// Chang ethe color of the font
	void SetFontColor(SDL_Color color);

	SDL_Color* GetFontColor() const { return m_textColor; }
	

protected:
	void UpdateFont();

private:
	// TTF font
	TTF_Font* m_font;

	// Color of the font
	SDL_Color* m_textColor;
	
	// Text to render using the font
	EString m_text;

	// Size of the font
	int m_defaultFontSize;
	int m_fontSize;

	// Load texture flags stored for easy rerender
	bool m_repeat;
	bool m_linear;
};