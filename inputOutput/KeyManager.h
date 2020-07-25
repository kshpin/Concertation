//
// Created by aether on 2/24/20.
//

#ifndef CONCERTATIONREALTIME_KEYMANAGER_H
#define CONCERTATIONREALTIME_KEYMANAGER_H


#include <cctype>

#include "Key.h"

#define NUM_KEYS 31

class KeyManager {
private:
	Key keys[NUM_KEYS];

public:
	KeyManager();

	void update();

	Key getKey(char k);
};


#endif //CONCERTATIONREALTIME_KEYMANAGER_H
