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

	bool isToggle = false;
	bool toggleVal = false;
public:
	GuiImageButton(unsigned int texture_id, std::string text);
	GuiImageButton(unsigned int texture_id, std::string text, bool toggle, bool initial_value);
	~GuiImageButton();
	void draw_at(int x, int y, int w, int h, int mouse_x, int mouse_y, int cl, RENDERPASS render_pass);
	void click(int btn, int action);
	void set_callback(std::function<void()> callback);
	void set_toggle_val(bool v);
};

