//
// Created by aether on 2/24/20.
//

#include "KeyManager.h"

KeyManager::KeyManager() {
	keys[0].setKey(Keyboard::Key::A);
	keys[1].setKey(Keyboard::Key::B);
	keys[2].setKey(Keyboard::Key::C);
	keys[3].setKey(Keyboard::Key::D);
	keys[4].setKey(Keyboard::Key::E);
	keys[5].setKey(Keyboard::Key::F);
	keys[6].setKey(Keyboard::Key::G);
	keys[7].setKey(Keyboard::Key::H);
	keys[8].setKey(Keyboard::Key::I);
	keys[9].setKey(Keyboard::Key::J);
	keys[10].setKey(Keyboard::Key::K);
	keys[11].setKey(Keyboard::Key::L);
	keys[12].setKey(Keyboard::Key::M);
	keys[13].setKey(Keyboard::Key::N);
	keys[14].setKey(Keyboard::Key::O);
	keys[15].setKey(Keyboard::Key::P);
	keys[16].setKey(Keyboard::Key::Q);
	keys[17].setKey(Keyboard::Key::R);
	keys[18].setKey(Keyboard::Key::S);
	keys[19].setKey(Keyboard::Key::T);
	keys[20].setKey(Keyboard::Key::U);
	keys[21].setKey(Keyboard::Key::V);
	keys[22].setKey(Keyboard::Key::W);
	keys[23].setKey(Keyboard::Key::X);
	keys[24].setKey(Keyboard::Key::Y);
	keys[25].setKey(Keyboard::Key::Z);
	keys[26].setKey(Keyboard::Key::Up);
	keys[27].setKey(Keyboard::Key::Down);
	keys[28].setKey(Keyboard::Key::Left);
	keys[29].setKey(Keyboard::Key::Right);
	keys[30].setKey(Keyboard::Key::Space);
}

void KeyManager::update() {
	for (Key &key : keys) {
		key.update();
	}
}

Key KeyManager::getKey(char k) {
	if (k == '^') return keys[26];
	if (k == '_') return keys[27];
	if (k == '<') return keys[28];
	if (k == '>') return keys[29];
	if (k == ' ') return keys[30];

	return keys[toupper(k) - 65];
}
