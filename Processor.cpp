#include "Processor.h"
#include "SoundProxy.h"

#include <cmath>

Processor::Processor(Clip transferClip, int samples) : raw(), processed(), movingAvgK(), movingAvgRange() {
	raw.left = new double[samples];
	raw.right = new double[samples];
	processed.left = new double[samples];
	processed.right = new double[samples];

	raw.numSamples = samples;
	processed.numSamples = samples;

	planLeft = fftw_plan_r2r_1d(samples, transferClip.left, raw.left, FFTW_REDFT10, FFTW_MEASURE);
	planRight = fftw_plan_r2r_1d(samples, transferClip.right, raw.right, FFTW_REDFT10, FFTW_MEASURE);

	for (int i = 0; i < samples; i++) {
		raw.left[i] = 0;
		raw.right[i] = 0;

		processed.left[i] = 0;
		processed.right[i] = 0;
	}

	normalize = true;
	fullNorm = false;
	smooth = true;
	flashFlood = true;

	normScale = 1.0;

	movingAvgRange = 10;
	increaseMovingAvgRange(0);
}

void Processor::applyTransform() {
	fftw_execute(planLeft);
	fftw_execute(planRight);
}

void Processor::processTransform() {
	static float scale = 1.0/50000;
	static float power = 1.5;
	for (int i = 0; i < raw.numSamples; i++) {
		raw.left[i] = pow(fabs(scale * raw.left[i]), power);
		raw.right[i] = pow(fabs(scale * raw.right[i]), power);

		if (normalize) {
			normalizeAmplitudes(raw.left[i], raw.right[i], i);
		}

		if (smooth) {
			smoothOut(raw.left[i], raw.right[i], processed.left[i], processed.right[i]);
		} else {
			processed.left[i] = raw.left[i];
			processed.right[i] = raw.right[i];
		}
	}
}

void Processor::normalizeAmplitudes(double &left, double &right, int freq) {
	static double power = 0.7;
	static double scale = 0.05;
	static double fullScale = 0.02;

	if (fullNorm) {
		left *= (freq+1) * fullScale;
		right *= (freq+1) * fullScale;
	} else {
		left *= pow(freq+1, power) * scale * normScale;
		right *= pow(freq+1, power) * scale * normScale;
	}
}

void Processor::smoothOut(double &left, double &right, double &smoothLeft, double &smoothRight) {
	if (flashFlood) {
		if (smoothLeft < left) smoothLeft = left;
		else {
			smoothLeft *= movingAvgInverseK;
			smoothLeft += left * movingAvgK;
		}

		if (smoothRight < right) smoothRight = right;
		else {
			smoothRight *= movingAvgInverseK;
			smoothRight += right * movingAvgK;
		}
	} else {
		smoothLeft *= movingAvgInverseK;
		smoothLeft += left * movingAvgK;

		smoothRight *= movingAvgInverseK;
		smoothRight += right * movingAvgK;
	}
}

void Processor::toggleNormalize() {
	normalize = !normalize;
}

void Processor::toggleFullNorm() {
	fullNorm = !fullNorm;
}

void Processor::toggleSmooth() {
	smooth = !smooth;
}

void Processor::toggleFlashFlood() {
	flashFlood = !flashFlood;
}

void Processor::increaseMovingAvgRange(int inc) {
	movingAvgRange += inc;
	if (movingAvgRange < 0) movingAvgRange = 0;
	std::cout << "Moving average range: " << movingAvgRange << std::endl;

	movingAvgK = 2.0 / (1 + movingAvgRange);
	movingAvgInverseK = 1 - movingAvgK;
}

void Processor::scaleNormScale(double scale) {
	normScale *= scale;
}

double Processor::getNormScale() {
	return normScale;
}

Clip Processor::getProcessed() {
	return processed;
}

Processor::~Processor() {
	delete [] raw.left;
	delete [] raw.right;
	delete [] processed.left;
	delete [] processed.right;
}
