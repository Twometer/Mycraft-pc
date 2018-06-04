#include "GuiPause.h"
#include "VboBuilder.h"
#include "OpenGLRenderer.h"
#include "Font.h"
#include "GuiSettings.h"

GuiPause::GuiPause()
{
	backButton.set_callback([=] {
		OpenGLRenderer::guiRenderer->closeGui();
	});

	settingsButton.set_callback([=] {
		OpenGLRenderer::guiRenderer->displayGui(new GuiSettings());
	});

	exitButton.set_callback([=] {

	});
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

	backButton.draw_at(x, y, gui_width / 3, gui_height, mx, my, colorLoc, pass);
	settingsButton.draw_at(x + gui_width / 3, y, gui_width / 3, gui_height, mx, my, colorLoc, pass);
	exitButton.draw_at(x + gui_width / 3 * 2, y, gui_width / 3, gui_height, mx, my, colorLoc, pass);
}

void GuiPause::onMouseClick(int btn, int a)
{
	backButton.click(btn, a);
	settingsButton.click(btn, a);
	exitButton.click(btn, a);
}