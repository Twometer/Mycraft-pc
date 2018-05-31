#pragma once
#include "Gui.h"
#include <string>
#include <functional>
class GuiImageButton
{
private:
	unsigned int texture_id;
	std::string text;
	int scale = 0;

	bool isMouseOver;

	std::function<void()> callback;
public:
	GuiImageButton(unsigned int texture_id, std::string text);
	~GuiImageButton();
	void drawAt(int x, int y, int w, int h, int mouseX, int mouseY, int cl, RENDERPASS renderPass);
	void click(int btn, int action);
	void setCallback(std::function<void()> callback);
};

