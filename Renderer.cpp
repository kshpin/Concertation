#include <cmath>
#include "Renderer.h"

void Renderer::drawProcessedLines() {
	static double spacingScale = (height-1) / log2(height);

	line[0].color = Color::White;

	for (int i = 1; i < numSamples; i++) {
		double h;
		if (mode == SPACED_OUT_LINES) h = height - spacingScale*log2(i) - 1;
		else h = height - i - 1;

		HSVtoRGB((numSamples - h)/numSamples, 1, 0xff, line[1].color);

		line[0].position.y = h;
		line[1].position.y = h;

		if (offCenter) line[0].position.x = width/2.0 + (processed.right[i] - processed.left[i])/2.0;
		else line[0].position.x = width/2.0;

		line[1].position.x = width/2.0 - processed.left[i];
		window->draw(line, 2, Lines);

		line[1].position.x = width/2.0 + processed.right[i];
		window->draw(line, 2, Lines);
	}
}

void Renderer::drawProcessedRects() {
	static double spacingScale = (height-1) / log2(height);
	static Color color;

	for (int i = 1; i < numSamples; i++) {
		double h = height - spacingScale*log2(i) - 1;

		HSVtoRGB((numSamples - h)/numSamples, 1, 0xff, color);
		rect.setOutlineColor(color);

		rect.setSize(Vector2f(processed.left[i] + processed.right[i], spacingScale*(log2(1 + 1.0/i))-1));
		rect.setPosition(width/2.0 - processed.left[i], height - spacingScale*log2(i+1));
		window->draw(rect);
	}
}

void Renderer::drawProcessedCircle() {
	static double radius = 169; // 169
	static double approachRate = 200; // higher is slower

	line[0].color = Color::White;

	for (int i = 1; i < numSamples; i++) {
		double percent = static_cast<double>(i)/numSamples;
		HSVtoRGB(percent, 1, 0xff, line[1].color);

		double leftVal = processed.left[i];
		leftVal = radius*leftVal / (leftVal + approachRate) + 1;
		double rightVal = processed.right[i];
		rightVal = radius*rightVal / (rightVal + approachRate) + 1;

		double sine = sin(percent*M_PI);
		double cosine = cos(percent*M_PI);

		line[1].position.x = width/2.0  - radius*sine;
		line[1].position.y = height/2.0 - radius*cosine;

		line[0].position.x = width/2.0  - (radius - leftVal)*sine;
		line[0].position.y = height/2.0 - (radius - leftVal)*cosine;

		window->draw(line, 2, Lines);

		line[1].position.x += 2*radius*sine;

		line[0].position.x = width/2.0  + (radius - rightVal)*sine;
		line[0].position.y = height/2.0 - (radius - rightVal)*cosine;

		window->draw(line, 2, Lines);
	}
}

// public --------------------------------------------------------------------------------------------------------------

Renderer::Renderer(int width, int height, bool vSync, int targetFPS, Clip processed, int numSamples, Color clearColor) : processed() {
	ContextSettings settings;
	settings.antialiasingLevel = 0;

	window = new RenderWindow(VideoMode(width, height), "Concertation but now with a bUtToN mAnAgEmEnT sYsTeM", Style::Default, settings);
	Image icon;
	icon.loadFromFile("res/iconCircle2.png"); // TODO get a better icon
	Vector2u size = icon.getSize();
	window->setIcon(size.x, size.y, icon.getPixelsPtr());

	if (vSync) window->setVerticalSyncEnabled(true); // actually just sets max fps to monitor refresh rate
	else window->setFramerateLimit(targetFPS);
	window->setKeyRepeatEnabled(false);

	this->clearColor = clearColor;

	this->width = width;
	this->height = height;

	this->processed = processed;
	this->numSamples = numSamples;

	rect.setFillColor(clearColor);
	rect.setOutlineThickness(1);

	offCenter = true;

	mode = LINES;
}

void Renderer::drawRaw(const Clip &clip, int offset) {
	static Color rawColor = Color::Red;
	static double rawScale = 800.0 / 0xffff;

	for (int i = 0; i < numSamples; i++) {
		line[0].position.x = i;
		line[0].position.y = offset;

		line[1].position.x = i;
		line[1].position.y = offset + rawScale*clip.left[i];

		line[0].color = rawColor;
		line[1].color = rawColor;

		window->draw(line, 2, Lines);
	}
}

void Renderer::drawSound() {
	if (mode == LINES || mode == SPACED_OUT_LINES) drawProcessedLines();
	if (mode == RECTANGLES) drawProcessedRects();
	if (mode == CIRCLE) drawProcessedCircle();
}

void Renderer::drawButtons(ButtonPanel &buttonPanel) {
	window->draw(buttonPanel);
}

void Renderer::beginFrame() {
	window->clear(clearColor);
}

void Renderer::endFrame() {
	window->display();
}

void Renderer::setMode(Mode m) {
	mode = m;
}

void Renderer::toggleOffCenter() {
	offCenter = !offCenter;
}

Vector2i Renderer::getMousePos() {
	return Mouse::getPosition(*window);
}

void Renderer::pollUpdates(bool &running, bool &inFocus) {
	Event event;
	while (window->pollEvent(event)) {
		switch (event.type) {
			case Event::Closed:
				running = false;
				break;

			case Event::Resized:
				break;

			case Event::LostFocus:
				inFocus = false;
				break;

			case Event::GainedFocus:
				inFocus = true;
				break;

				// for single characters, but not for modifier keys (ctrl, alt, shift)
				// includes non-printables like backspace
			case Event::TextEntered:
				// event.text.unicode for char or just event.text for sf::String
				break;

				// use sf::Keyboard::isKeyPressed for smooth real-time keyboard input, here really slow
			case Event::KeyPressed:
				break;
			case Event::KeyReleased:
				break;

			case Event::MouseWheelScrolled:
				// event.mouseWheelScroll.wheel, delta, x, y
				break;

			case Event::MouseButtonPressed:
				// event.mouseButton.button, x, y
				// supports left/right/middle/extra1/extra2
				break;
			case Event::MouseButtonReleased:
				break;

				// triggers when window not in focus too
			case Event::MouseMoved:
				// event.mouseMove.x, y
				break;

			case Event::MouseEntered:
				break;
			case Event::MouseLeft:
				break;

				// screw joysticks
			default:
				break;
		}
	}
}

void Renderer::HSVtoRGB(double H, double S, double V, Color &color) {
	double C = S * V;

	double part = 6.0*fmod(H, 1);
	double X = C*(1 - fabs(std::fmod(part, 2) - 1));

	color.r = 0;
	color.g = 0;
	color.b = 0;

	if (part < 1) {
		color.r = C; color.g = X;
	} else if (part < 2) {
		color.r = X; color.g = C;
	} else if (part < 3) {
		color.g = C; color.b = X;
	} else if (part < 4) {
		color.g = X; color.b = C;
	} else if (part < 5) {
		color.r = X; color.b = C;
	} else {
		color.r = C; color.b = X;
	}

	double m = V-C;
	color.r += m;
	color.g += m;
	color.b += m;
}
