//
// Created by aether on 2/26/20.
//

#ifndef CONCERTATIONREALTIME_BUTTONPANEL_H
#define CONCERTATIONREALTIME_BUTTONPANEL_H


#include <SFML/Graphics/Font.hpp>
#include "Button.h"

using namespace sf;

class ButtonPanel : public Drawable {
private:
	Font font;
	std::vector<Button> buttons;

	bool curLmbPressed;
	bool lastLmbPressed;

	virtual void draw(RenderTarget &target, RenderStates states) const;

public:
	ButtonPanel();

	void addButton(std::function<bool (const std::string&)> action, const std::string &text, ButtonStyle buttonStyle, int posX, int posY);

	void update(Vector2i pos, bool lmb);

	std::vector<Button> getButtons();
};


#endif //CONCERTATIONREALTIME_BUTTONPANEL_H
