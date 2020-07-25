#include "SFML/Graphics.hpp"
#include "SFML/Audio.hpp"
#include "SoundProxy.h"

#include "Processor.h"
#include "Renderer.h"
#include "network.h"
#include "inputOutput/KeyManager.h"
#include "inputOutput/ButtonPanel.h"

#include <iostream>
#include <chrono>
#include <thread>
#include <algorithm>

#define VSYNC true
#define TARGET_UPS 60
#define TARGET_FPS 60

const int TARGET_FRAME_LENGTH = 1000/TARGET_FPS;

#define TRANSFORM_NUM_SAMPLES 704

#define HEIGHT TRANSFORM_NUM_SAMPLES
#define WIDTH HEIGHT

#define NUM_LEDS 180

using namespace sf;

// TODO break input off into its own module, so that Concertation doesn't do that job by itself

class Concertation {
private:
	Mutex mutex;

	SoundProxy proxy;

	ButtonPanel buttonPanel;
	KeyManager keymap;

	Clock clock;

	Processor *processor;
	Renderer *renderer;

	Clip transferClip;
	Color clearColor;

	bool deviceSelected;

	bool paused = false;
	bool running = true;

public:
	Concertation() :
			clearColor(0x33, 0x33, 0x33),
			proxy(seconds(1.0/TARGET_UPS), &mutex),
			transferClip() {
		if (!SoundProxy::isAvailable()) {
			std::cout << "ERROR: no audio capture on system" << std::endl;
		}

		transferClip.left = new double[MAX_SAMPLES];
		transferClip.right = new double[MAX_SAMPLES];

		processor = new Processor(transferClip, TRANSFORM_NUM_SAMPLES);
		renderer = new Renderer(WIDTH, HEIGHT, VSYNC, TARGET_FPS, processor->getProcessed(), TRANSFORM_NUM_SAMPLES, clearColor);

		// proxy setup
		proxy.setChannelCount(2);
		proxy.start();

		ButtonStyle buttonStyle = {{
				{
						Color(0x70, 0x6F, 0x6F),
						Color(0x70, 0x6F, 0x6F),
						clearColor,
						10, 10, 5, 10,
						0
				},
				{
						clearColor,
						Color(0x70, 0x6F, 0x6F),
						Color(0x70, 0x6F, 0x6F),
						10, 10, 5, 10,
						1
				},
				{
						clearColor,
						Color::Red, // (0x70, 0x6F, 0x6F)
						Color(0x70, 0x6F, 0x6F),
						10, 10, 5, 10,
						2
				}
		}};

		// buttons
		std::vector<std::string> devices(SoundProxy::getAvailableDevices());
		for (int i = 0; i < devices.size(); i++) {
			buttonPanel.addButton(
					std::bind(&Concertation::setProxyDevice, this, std::placeholders::_1),
					devices[i],
					buttonStyle,
					10, 10 + 45 * i
			);
		}

		deviceSelected = false;
	}

	bool setProxyDevice(const std::string& deviceName) {
		deviceSelected = proxy.setDevice(deviceName);
		return deviceSelected;
	}

	int run() {
		while (running) {
			handleEvents();

			// sleep between frames to minimize CPU usage
			Time elapsed = clock.restart();
			std::this_thread::sleep_for(std::chrono::milliseconds(TARGET_FRAME_LENGTH - elapsed.asMilliseconds()));

			if (!paused && deviceSelected) {
				mutex.lock(); // ---------------------------------------------------------------------------------------

				Clip volatileClip = proxy.getClip();
				transferClip.numSamples = volatileClip.numSamples;
				if (transferClip.numSamples > MAX_SAMPLES) transferClip.numSamples = MAX_SAMPLES;

				std::copy(volatileClip.left, volatileClip.left+MAX_SAMPLES, transferClip.left);
				std::copy(volatileClip.right, volatileClip.right+MAX_SAMPLES, transferClip.right);

				mutex.unlock(); // -------------------------------------------------------------------------------------

				processor->applyTransform();
				processor->processTransform();

				send(processor->getProcessed());

				renderer->beginFrame();
				renderer->drawSound();
				renderer->endFrame();
			}

			if (!deviceSelected) {
				renderer->beginFrame();
				renderer->drawButtons(buttonPanel);
				renderer->endFrame();
			}
		}

		return 0;
	}

	static void send(Clip clip) {
		static Color udpColor;
		static char udpMsg[NUM_LEDS*3];

		for (int i = 0; i < NUM_LEDS * 3 / 2; i += 3) {
			double h = clip.left[i/3];
			double v = h > 255 ? 255 : h;
			Renderer::HSVtoRGB(h/255.0, 1, v, udpColor);

			udpMsg[i] = udpColor.r;
			udpMsg[i+1] = udpColor.g;
			udpMsg[i+2] = udpColor.b;

			h = clip.right[i/3];
			v = h > 255 ? 255 : h;

			Renderer::HSVtoRGB(h/255.0, 1, v, udpColor);

			udpMsg[NUM_LEDS*3-3-i] = udpColor.r;
			udpMsg[NUM_LEDS*3-2-i] = udpColor.g;
			udpMsg[NUM_LEDS*3-1-i] = udpColor.b;
		}

		udpSend(udpMsg, NUM_LEDS*3);
	}

	void handleEvents() {
		static bool inFocus = true;

		keymap.update();
		buttonPanel.update(renderer->getMousePos(), Mouse::isButtonPressed(Mouse::Left));

		// quit
		if (keymap.getKey('Q').isDown() && inFocus) running = false;

		// act on updates
		if (inFocus) {
			if (keymap.getKey('P').isPressed()) {
				paused = !paused;
				if (paused) proxy.stop();
				else proxy.start();
			}

			if (keymap.getKey('N').isPressed()) processor->toggleNormalize();
			if (keymap.getKey('S').isPressed()) processor->toggleSmooth();
			if (keymap.getKey('F').isPressed()) processor->toggleFlashFlood();
			if (keymap.getKey('>').isPressed()) processor->increaseMovingAvgRange(1);
			if (keymap.getKey('<').isPressed()) processor->increaseMovingAvgRange(-1);
			if (keymap.getKey('O').isPressed()) renderer->toggleOffCenter();
			if (keymap.getKey(' ').isPressed()) renderer->setMode(SPACED_OUT_LINES);
			if (keymap.getKey('L').isPressed()) renderer->setMode(LINES);
			if (keymap.getKey('R').isPressed()) renderer->setMode(RECTANGLES);
			if (keymap.getKey('C').isPressed()) {
				renderer->setMode(CIRCLE);
				processor->toggleFullNorm();
			}
			if (keymap.getKey('M').isPressed()) deviceSelected = false;

			if (keymap.getKey('^').isPressed()) {
				processor->scaleNormScale(1.1);
				std::cout << "Norm scale: " << processor->getNormScale() << std::endl;
			}
			if (keymap.getKey('_').isPressed()) {
				processor->scaleNormScale(1/1.1);
				std::cout << "Norm scale: " << processor->getNormScale() << std::endl;
			}
		}

		renderer->pollUpdates(running, inFocus);
	}

	~Concertation() {
		delete processor;
		delete renderer;
	}
};

int main() {
	Concertation concertation;
	return concertation.run();
}
