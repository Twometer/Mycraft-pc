#include "GuiPause.h"
#include "VboBuilder.h"
#include "OpenGLRenderer.h"
#include "Font.h"
#include "TextureIds.h"

GuiPause::GuiPause()
{
}


GuiPause::~GuiPause()
{
}

int GuiPause::getIdentifier()
{
	return GUI_IDENTIFIER;
}

void GuiPause::onRender(int mx, int my, RENDERPASS pass, int colorLoc)
{
	int wid = OpenGLRenderer::width;
	int hei = OpenGLRenderer::height;
	int x = wid / 2 - gui_width / 2;
	int y = hei / 2 - gui_height / 2;

	if (pass == RENDERPASS::FLAT)
	{
		VboBuilder builder = VboBuilder(2);
		builder.drawRect(x, y, gui_width, gui_height, COLORDATA(10, 10, 50, 210));
		builder.buildAndRender();
	}
	else if (pass == RENDERPASS::FONT)
	{
		int d = 552 / 3;
		drawButtonTitle("Back to game", 0, wid, hei, colorLoc);
		drawButtonTitle("Settings", 1, wid, hei, colorLoc);
		drawButtonTitle("Disconnect", 2, wid, hei, colorLoc);
	}
	else if (pass == RENDERPASS::TEX)
	{
		int xmod = 552 / 3;
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, TextureIds::tex_guipause_back);
		VboBuilder builder = VboBuilder(2);
		builder.drawRect(x + 0 * xmod + xmod / 2 - 128 / 2, y + gui_height / 2 - 128 / 2, 128, 128, COLORDATA(255, 255, 255, 255), true);
		builder.buildAndRender();

		glBindTexture(GL_TEXTURE_2D, TextureIds::tex_guipause_settings);
		builder = VboBuilder(2);
		builder.drawRect(x + 1 * xmod + xmod / 2 - 128 / 2, y + gui_height / 2 - 128 / 2, 128, 128, COLORDATA(255, 255, 255, 255), true);
		builder.buildAndRender();

		glBindTexture(GL_TEXTURE_2D, TextureIds::tex_guipause_leave);
		builder = VboBuilder(2);
		builder.drawRect(x + 2 * xmod + xmod / 2 - 128 / 2, y + gui_height / 2 - 128 / 2, 128, 128, COLORDATA(255, 255, 255, 255), true);
		builder.buildAndRender();
	}
}

void GuiPause::drawButtonTitle(std::string text, int d, int w, int h, int cl)
{
	int x = w / 2 - gui_width / 2;
	int y = h / 2 - gui_height / 2;
	int xmod = 552 / 3;
	Font::roboto.renderText(text, cl, x + d * xmod + xmod / 2 - Font::roboto.getSize(text).x / 2, y + gui_height - 30, 1.0, glm::vec3(255, 255, 255));
}