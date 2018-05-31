#pragma once
#include "Gui.h"
#include <string>
class GuiChat :
	public Gui
{
private:
	std::string chatInput = std::string();
public:
	static const int GUI_IDENTIFIER = 0;
	GuiChat();
	~GuiChat();
	std::string getInput();
	virtual int getIdentifier();
	virtual void onLoad();
	virtual void onKeyPress(int key, int action);
	virtual void onCharPress(unsigned int chr);
	virtual void onRender(int mouseX, int mouseY, RENDERPASS pass, int colorLocation);
};

