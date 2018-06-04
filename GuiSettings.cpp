#include "GuiSettings.h"
#include "Settings.h"
#include "OpenGLRenderer.h"
#include "VboBuilder.h"
#include "Font.h"


GuiSettings::GuiSettings()
{
	btn_adv_water.set_callback([=] {
		Settings::ADVANCED_WATER = !Settings::ADVANCED_WATER;
		btn_adv_water.set_toggle_val(Settings::ADVANCED_WATER);
	});
	btn_bloom.set_callback([=] {
		Settings::BLOOM = !Settings::BLOOM;
		btn_bloom.set_toggle_val(Settings::BLOOM);
	});
	btn_ao.set_callback([=] {
		Settings::AMBIENT_OCCLUSION = !Settings::AMBIENT_OCCLUSION;
		btn_ao.set_toggle_val(Settings::AMBIENT_OCCLUSION);
		OpenGLRenderer::world->reload();
	});
	btn_skybox.set_callback([=] {
		Settings::SKYBOX = !Settings::SKYBOX;
		btn_skybox.set_toggle_val(Settings::SKYBOX);
	});
}


GuiSettings::~GuiSettings()
{
}

void GuiSettings::onMouseClick(int button, int action)
{
	btn_adv_water.click(button, action);
	btn_ao.click(button, action);
	btn_bloom.click(button, action);
	btn_skybox.click(button, action);
}

void GuiSettings::onRender(int mouseX, int mouseY, RENDERPASS pass, int colorLoc)
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
		Font::robotoTitle.renderTextWithShadow("Settings", colorLoc, wid / 2.0f - Font::robotoTitle.getSize("Settings").x / 2.0f - 10, y - 50, 1.0f, glm::vec3(1.0f, 1.0f, 1.0f));
	}

	btn_adv_water.draw_at(x, y, gui_width / 4, gui_height / 2, mouseX, mouseY, colorLoc, pass);
	btn_ao.draw_at(x + gui_width / 4, y, gui_width / 4, gui_height / 2, mouseX, mouseY, colorLoc, pass);
	btn_bloom.draw_at(x + gui_width / 4 * 2, y, gui_width / 4, gui_height / 2, mouseX, mouseY, colorLoc, pass);
	btn_skybox.draw_at(x + gui_width / 4 * 3, y, gui_width / 4, gui_height / 2, mouseX, mouseY, colorLoc, pass);
}

int GuiSettings::getIdentifier()
{
	return GUI_IDENTIFIER;
}
