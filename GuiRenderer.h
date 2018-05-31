#pragma once
#include "Gui.h"
class GuiRenderer
{
private:
	Gui * currentGui;
public:
	GuiRenderer();
	~GuiRenderer();
	void onMouseClick(int button, int action);
	void onKeyPress(int key, int action);
	void onCharPress(unsigned int chr);
	void onRender(int mouseX, int mouseY, RENDERPASS pass, int colorLocation);

	void displayGui(Gui* gui);
	void closeGui();
	bool isGuiOpen();
	bool isGuiOpen(int id);
	Gui* getCurrentGui();
};