#ifndef CONCERTATIONREALTIME_RENDERER_H
#define CONCERTATIONREALTIME_RENDERER_H


#include "SFML/Graphics.hpp"

#include "SoundProxy.h"
#include "Mode.h"
#include "inputOutput/ButtonPanel.h"

class Renderer {
private:
	RenderWindow *window;
	Color clearColor;

	int width;
	int height;

	Clip processed;
	int numSamples;

	RectangleShape rect;
	Vertex line[2];

	Mode mode;

	bool offCenter;

	void drawProcessedLines();

	void drawProcessedRects();

	void drawProcessedCircle();

public:
	Renderer(int width, int height, bool vSync, int targetFPS, Clip processed, int numSamples, Color clearColor);

	void drawRaw(const Clip &clip, int offset);

	void drawSound();

	void drawButtons(ButtonPanel &buttonPanel);

	void beginFrame();

	void endFrame();

	void setMode(Mode mode);

	void toggleOffCenter();

	Vector2i getMousePos();

	void pollUpdates(bool &running, bool &inFocus);

	static void HSVtoRGB(double H, double S, double V, Color &color);
};


#endif //CONCERTATIONREALTIME_RENDERER_H
