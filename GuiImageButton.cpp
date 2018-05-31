#include "GuiImageButton.h"
#include "VboBuilder.h"
#include "Font.h"
#include "GLFW/glfw3.h"

using namespace std;
using namespace glm;

GuiImageButton::GuiImageButton(unsigned int texture_id, string text)
{
	this->texture_id = texture_id;
	this->text = text;
	
}

GuiImageButton::~GuiImageButton()
{
}

void GuiImageButton::drawAt(int x, int y, int w, int h, int mouseX, int mouseY, int cl, RENDERPASS renderPass)
{
	bool mouseOver = mouseX >= x && mouseY >= y && mouseX < x + w && mouseY < y + h;
	if (mouseOver && scale < 8)
		scale++;
	else if (!mouseOver && scale > 0)
		scale--;
	if (renderPass == FONT)
	{
		Font::roboto.renderText(text, cl, x + w / 2 - Font::roboto.getSize(text).x / 2, y + h - 30, 1, vec3(255, 255, 255));
	}
	else if (renderPass == TEX)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture_id);
		VboBuilder builder = VboBuilder(2);
		builder.drawRect(x + w / 2 - 128 / 2 - scale, y + h / 2 - 128 / 2 - scale, 128 + scale * 2, 128 + scale * 2, COLORDATA(255, 255, 255, 255), true);
		builder.buildAndRender();
	}
	isMouseOver = mouseOver;
}

void GuiImageButton::click(int btn, int a)
{
	if (btn == GLFW_MOUSE_BUTTON_1 && a == GLFW_RELEASE && isMouseOver)
	{
		callback();
	}
}

void GuiImageButton::setCallback(function<void()> callback)
{
	this->callback = callback;
}