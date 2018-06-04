#include "GuiRespawn.h"
#include "OpenGLRenderer.h"
#include "C16Respawn.h"
#include "VboBuilder.h"
#include "Font.h"


GuiRespawn::GuiRespawn()
{
	respawnButton.set_callback([=] {
		OpenGLRenderer::sendPacket(new C16Respawn());
		OpenGLRenderer::guiRenderer->closeGui();
	});
}


GuiRespawn::~GuiRespawn()
{
}

void GuiRespawn::onMouseClick(int button, int action)
{
	respawnButton.click(button, action);
	exitButton.click(button, action);
}

void GuiRespawn::onRender(int mouseX, int mouseY, RENDERPASS pass, int colorLoc)
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
	else if (pass == FONT)
	{
		Font::robotoTitle.renderTextWithShadow("You died!", colorLoc, wid / 2.0f - Font::robotoTitle.getSize("You died!").x / 2.0f - 10, y - 50, 1.0f, glm::vec3(1.0f, 1.0f, 1.0f));
	}

	respawnButton.draw_at(x, y, gui_width / 2, gui_height, mouseX, mouseY, colorLoc, pass);
	exitButton.draw_at(x + gui_width / 2, y, gui_width / 2, gui_height, mouseX, mouseY, colorLoc, pass);
}

int GuiRespawn::getIdentifier()
{
	return GUI_IDENTIFIER;
}

bool GuiRespawn::mayClose()
{
	return false;
}
