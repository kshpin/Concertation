//
// Created by aether on 2/24/20.
//

#include "Key.h"

Key::Key() {
	curPressed = false;
	lastPressed = false;
}

Key::Key(Keyboard::Key k) {
	key = k;
	curPressed = false;
	lastPressed = false;
}

void Key::setKey(Keyboard::Key k) {
	key = k;
}

void Key::update() {
	lastPressed = curPressed;
	curPressed = Keyboard::isKeyPressed(key);
}

bool Key::isDown() {
	return curPressed;
}

bool Key::isPressed() {
	return curPressed && !lastPressed;
}

bool Key::isReleased() {
	return !curPressed && lastPressed;
}
