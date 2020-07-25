//
// Created by aether on 2/26/20.
//

#ifndef CONCERTATIONREALTIME_BUTTON_H
#define CONCERTATIONREALTIME_BUTTON_H


#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <functional>

#define STYLE_STANDARD 0
#define STYLE_HOVERING 1
#define STYLE_PRESSING 2

using namespace sf;

struct ButtonStateStyle {
	Color fillColor;
	Color outlineColor;
	Color textColor;

	int paddingLeft;
	int paddingRight;
	int paddingTop;
	int paddingBottom;

	int outlineThickness;
};

struct ButtonStyle {
	ButtonStateStyle states[3];
};

class Button : public Drawable {
private:
	std::function<bool (std::string&)> doAction;

	Text text;
	RectangleShape box;

	ButtonStyle style;

	int x;
	int y;
	int textWidth;
	int textHeight;

	bool hovering;
	bool pressed;

	void setStyle(int s);

	virtual void draw(RenderTarget &target, RenderStates states) const;

public:
	Button(std::function<bool (const std::string&)> action, const std::string &t, Font &font, int size, ButtonStyle buttonStyle, int posX, int posY);

	void setPressed();

	void clearPressed();

	void setHovering();

	void clearHovering();

	FloatRect getBound();
};


#endif //CONCERTATIONREALTIME_BUTTON_H
