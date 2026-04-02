#pragma once
#include "EngineTypes.h"
#include "Math/ESTransform.h"
#include "SDL/SDL_video.h"

class EGraphicsEngine;
class EInput;

#define SLATE_UNIT_SCALAR 1080.0f

struct ESWindowParams {
	// Default constructor
	ESWindowParams() {
		title = "Eventide Engine Window";
		x = y = 0;
		w = 1280;
		h = 720;
		vsync = false;
		fullscreen = false;
	}

	// Settings constructor
	ESWindowParams(EString title, int x, int y,
		unsigned int w, unsigned int h) : 
		title(title), 
		x(x), y(y), 
		w(w), h(h),
		vsync(false),
		fullscreen(false)
	{}

	// Title of the window
	EString title;
	// Position of the window
	int x, y;
	// Width and height of the window
	unsigned int w, h;
	// Vsync enable
	bool vsync;
	// Fullscreen enable
	bool fullscreen;
};

struct SDL_Window;

class EWindow {
public:
	EWindow();
	~EWindow();

	// Create the window with parameters
	bool CreateWindow(const ESWindowParams& params);

	// Close the window
	void CloseWindow() { m_shouldClose = true; }

	// Check if the window has been set to closed
	bool IsPendingClose() const { return m_shouldClose; }

	// Listen for input
	void RegisterInput(const TShared<EInput>& m_input);

	// Move Camera
	void MoveCamera();

	// Render the graphics engine to the window
	void Render();

	// Get a weak reference to the graphics engine
	TUnique<EGraphicsEngine>& GetGraphicsEngine() { return m_graphicsEngine; }

	// Get window params
	ESWindowParams& GetParams() { return m_params; }

	// Get default size
	glm::vec2 GetDefaultSize() { return glm::vec2(m_params.w, m_params.h); }

	// Get default center
	glm::vec2 GetDefaultCenter() { return GetDefaultSize() / 2.0f; }

	// Get current size
	glm::vec2 GetCurrentSize() {
		int windowWidth; int windowHeight;
		SDL_GetWindowSize(m_sdlWindow, &windowWidth, &windowHeight); // Current Size
		return glm::vec2(windowWidth, windowHeight);
	}

	// Get current center
	glm::vec2 GetCurrentCenter() { return GetCurrentSize() / 2.0f; }
	
	// Get aspect ratio
	float GetAspectRatio() {
		glm::vec2 currentSize = GetCurrentSize();
		return currentSize.x / currentSize.y;
	}

	// Get slate unit
	glm::vec2 GetSlateUnits() { return GetCurrentSize() / SLATE_UNIT_SCALAR; }

	float GetSlateUnitX() { return GetSlateUnits().x; }
	float GetSlateUnitY() { return GetSlateUnits().y; }

	// Get bounds of the display window is in
	SDL_Rect GetDisplayBounds();

public:
	// Flag to randomly change brightness
	bool m_randomlyChangeBrightness;

private:
	// A ref to the window in SDL
	SDL_Window* m_sdlWindow;

	// Window parameters
	ESWindowParams m_params;

	// Determine if the window should close
	bool m_shouldClose;

	// Store the graphics engine
	TUnique<EGraphicsEngine> m_graphicsEngine;

	// Amount to rotate the camera
	glm::vec3 m_cameraRotation;

	// Can zoom
	bool m_canZoom;

	// Double camera speed
	bool m_doubleCameraSpeed;

	// Can adjust texture depth
	bool m_canAdjustTextureDepth;
};