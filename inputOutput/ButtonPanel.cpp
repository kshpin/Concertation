//
// Created by aether on 2/26/20.
//

#include <SFML/Graphics.hpp>
#include <iostream>
#include <functional>
#include "ButtonPanel.h"

void ButtonPanel::draw(RenderTarget &target, RenderStates states) const {
	for (Button button : buttons) {
		target.draw(button, states);
	}
}

ButtonPanel::ButtonPanel() {
	if (!font.loadFromFile("res/Roboto-Light.ttf")) {
		std::cout << "ERROR: loading font failed" << std::endl;
	}

	curLmbPressed = false;
	lastLmbPressed = false;
}

void ButtonPanel::addButton(std::function<bool (const std::string&)> action, const std::string &text, ButtonStyle buttonStyle, int posX, int posY) {
	buttons.push_back(Button(action, text, font, 20, buttonStyle, posX, posY));
}

void ButtonPanel::update(Vector2i pos, bool lmb) {
	lastLmbPressed = curLmbPressed;
	curLmbPressed = lmb;

	for (Button &button : buttons) {
		FloatRect bound = button.getBound();
		if (pos.x >= bound.left
		    && pos.x <= bound.left+bound.width
		    && pos.y >= bound.top
		    && pos.y <= bound.top+bound.height) {
			if (curLmbPressed) button.setPressed();
			else if (!curLmbPressed && lastLmbPressed) button.clearPressed();
			else button.setHovering();
		} else button.clearHovering();
	}
}

std::vector<Button> ButtonPanel::getButtons() {
	return buttons;
}
