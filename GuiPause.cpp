#include "GuiPause.h"
#include "VboBuilder.h"
#include "OpenGLRenderer.h"
#include "Font.h"

GuiPause::GuiPause()
{
	backButton.setCallback([=] {
		OpenGLRenderer::guiRenderer->closeGui();
	});

	settingsButton.setCallback([=] {

	});

	exitButton.setCallback([=] {

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

	backButton.drawAt(x, y, gui_width / 3, gui_height, mx, my, colorLoc, pass);
	settingsButton.drawAt(x + gui_width / 3, y, gui_width / 3, gui_height, mx, my, colorLoc, pass);
	exitButton.drawAt(x + gui_width / 3 * 2, y, gui_width / 3, gui_height, mx, my, colorLoc, pass);
}

void GuiPause::onMouseClick(int btn, int a)
{
	backButton.click(btn, a);
	settingsButton.click(btn, a);
	exitButton.click(btn, a);
}