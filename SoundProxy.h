//
// Created by aether on 11/15/19.
//

#ifndef CONCERTATIONREALTIME_SOUNDPROXY_H
#define CONCERTATIONREALTIME_SOUNDPROXY_H


#include <iostream>
#include "SFML/Audio.hpp"

#define MAX_SAMPLES 5000

using namespace sf;

struct Clip {
	double *left;
	double *right;
	std::size_t numSamples;
};

class SoundProxy : public SoundRecorder {
private:
	Clip curClip;
	Mutex *mutex;

public:
	SoundProxy(Time interval, Mutex *m) {
		setProcessingInterval(interval);
		mutex = m;

		curClip.left = new double[MAX_SAMPLES];
		curClip.right = new double[MAX_SAMPLES];

		for (int i = 0; i < MAX_SAMPLES; i++) {
			curClip.left[i] = 0;
			curClip.right[i] = 0;
		}
	}

	virtual bool onProcessSamples(const Int16 *samples, std::size_t sampleCount) {
		mutex->lock();

		curClip.numSamples = sampleCount;
		unsigned long numPerEar = sampleCount/2;

		for (int i = 0; i < MAX_SAMPLES; i++) {
			curClip.left[i] = samples[2*i];
			curClip.right[i] = samples[2*i+1];
		}

		mutex->unlock();

		return true;
	}

	Clip getClip() {
		return curClip;
	}

	~SoundProxy() {
		stop();
		delete [] curClip.left;
		delete [] curClip.right;
	}
};


#endif //CONCERTATIONREALTIME_SOUNDPROXY_H
