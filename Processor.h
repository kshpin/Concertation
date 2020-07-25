#ifndef CONCERTATIONREALTIME_PROCESSOR_H
#define CONCERTATIONREALTIME_PROCESSOR_H


#include "SFML/Graphics.hpp"

#include "fftw3.h"

#include "SoundProxy.h"

using namespace sf;

class Processor {
private:
	Clip raw;
	Clip processed;

	fftw_plan planLeft;
	fftw_plan planRight;

	bool normalize;
	bool fullNorm;
	bool smooth;
	bool flashFlood;

	double normScale;

	int movingAvgRange;
	double movingAvgK;
	double movingAvgInverseK;

	void normalizeAmplitudes(double &left, double &right, int freq);

	void smoothOut(double &left, double &right, double &smoothLeft, double &smoothRight);

public:
	Processor(Clip transferClip, int samples);

	void applyTransform();

	void processTransform();

	void toggleNormalize();

	void toggleFullNorm();

	void toggleSmooth();

	void toggleFlashFlood();

	void increaseMovingAvgRange(int inc);

	void scaleNormScale(double scale);

	double getNormScale();

	Clip getProcessed();

	~Processor();
};


#endif //CONCERTATIONREALTIME_PROCESSOR_H
