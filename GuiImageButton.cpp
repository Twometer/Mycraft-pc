#include "GuiImageButton.h"
#include "VboBuilder.h"
#include "Font.h"
#include "GLFW/glfw3.h"
#include "OpenGLRenderer.h"

using namespace std;
using namespace glm;

GuiImageButton::GuiImageButton(unsigned int texture_id, string text)
{
	this->texture_id = texture_id;
	this->text = text;
	
}

GuiImageButton::GuiImageButton(unsigned texture_id, std::string text, bool toggle, bool initialValue)
{
	this->texture_id = texture_id;
	this->text = text;
	this->isToggle = toggle;
	this->toggleVal = initialValue;
}

GuiImageButton::~GuiImageButton()
{
}

void GuiImageButton::draw_at(int x, int y, int w, int h, int mouseX, int mouseY, int cl, RENDERPASS renderPass)
{
	bool mouseOver = mouseX >= x && mouseY >= y && mouseX < x + w && mouseY < y + h;
	if (mouseOver && scale < 8)
		scale++;
	else if (!mouseOver && scale > 0)
		scale--;
	if (renderPass == FONT)
	{
		string finalText = text;
		if(isToggle)
		{
			if (toggleVal) finalText += ": On";
			else finalText += ": Off";
		}
		Font::roboto.renderText(finalText, cl, x + w / 2 - Font::roboto.getSize(finalText).x / 2, y + h - 30, 1, vec3(255, 255, 255));
	}
	else if (renderPass == TEX)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture_id);
		VboBuilder builder = VboBuilder(2);
		builder.drawRect(x + w / 2 - 128 / 2 - scale, OpenGLRenderer::height - (y + h / 2 + 128 / 2 - scale), 128 + scale * 2, 128 + scale * 2, COLORDATA(255, 255, 255, 255), true);
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

void GuiImageButton::set_callback(function<void()> callback)
{
	this->callback = callback;
}

void GuiImageButton::set_toggle_val(bool v)
{
	this->toggleVal = v;
}
