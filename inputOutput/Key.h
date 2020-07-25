//
// Created by aether on 2/24/20.
//

#ifndef CONCERTATIONREALTIME_KEY_H
#define CONCERTATIONREALTIME_KEY_H

#include <SFML/Window/Keyboard.hpp>

using namespace sf;

class Key {
private:
	Keyboard::Key key;
	bool curPressed;
	bool lastPressed;

public:
	Key();

	Key(Keyboard::Key k);

	void setKey(Keyboard::Key k);

	void update();

	bool isDown();

	bool isPressed();

	bool isReleased();
};


#endif //CONCERTATIONREALTIME_KEY_H
