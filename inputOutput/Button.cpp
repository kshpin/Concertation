//
// Created by aether on 2/26/20.
//

#include <SFML/Graphics.hpp>
#include "Button.h"

Button::Button(std::function<bool (const std::string&)> action, const std::string &t, Font &font, int size, ButtonStyle buttonStyle, int posX, int posY) {
	doAction = action;

	style = buttonStyle;

	setStyle(STYLE_STANDARD);

	text.setString(t);
	text.setFont(font);
	text.setCharacterSize(size);

	box.setPosition(posX, posY);

	x = posX;
	y = posY;

	FloatRect bound = text.getLocalBounds();
	textWidth = bound.width;
	textHeight = bound.height;

	pressed = false;
	hovering = false;
}

void Button::setStyle(int s) {
	text.setFillColor(style.states[s].textColor);
	box.setFillColor(style.states[s].fillColor);
	box.setOutlineColor(style.states[s].outlineColor);

	text.setPosition(x + style.states[s].paddingLeft, y + style.states[s].paddingTop);
	box.setSize(Vector2f(textWidth + style.states[s].paddingLeft + style.states[s].paddingRight, textHeight + style.states[s].paddingTop + style.states[s].paddingBottom));

	box.setOutlineThickness(style.states[s].outlineThickness);
}

void Button::draw(RenderTarget &target, RenderStates states) const {
	target.draw(box, states);
	target.draw(text, states);
}

void Button::setPressed() {
	pressed = true;
	setStyle(STYLE_PRESSING);
}

void Button::clearPressed() {
	pressed = false;
	setStyle(STYLE_HOVERING);

	std::string str = text.getString();
	doAction.operator()(str);
}

void Button::setHovering() {
	hovering = true;
	setStyle(STYLE_HOVERING);
}

void Button::clearHovering() {
	hovering = false;
	pressed = false;
	setStyle(STYLE_STANDARD);
}

FloatRect Button::getBound() {
	return box.getGlobalBounds();
}
