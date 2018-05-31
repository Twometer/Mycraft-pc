#pragma once
#include "Gui.h"
#include <string>
class GuiPause :
	public Gui
{
private:
	void drawButtonTitle(std::string text, int d, int w, int h, int cl);
	const int gui_width = 552;
	const int gui_height = 300;
public:
	static const int GUI_IDENTIFIER = 1;
	GuiPause();
	~GuiPause();
	virtual int getIdentifier();
	virtual void onRender(int mouseX, int mouseY, RENDERPASS pass, int colorLocation);
};

