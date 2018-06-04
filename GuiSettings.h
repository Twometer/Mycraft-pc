#pragma once
#include "Gui.h"
#include "GuiImageButton.h"
#include "TextureIds.h"
#include "Settings.h"

class GuiSettings
	: public Gui
{
private:
	const int gui_width = 776;
	const int gui_height = 500;
	GuiImageButton btn_adv_water = GuiImageButton(TextureIds::tex_guisettings_advanced_water, "Advanced water", true, Settings::ADVANCED_WATER);
	GuiImageButton btn_ao = GuiImageButton(TextureIds::tex_guisettings_ambient_occlusion, "Ambient occlusion", true, Settings::AMBIENT_OCCLUSION);
	GuiImageButton btn_bloom = GuiImageButton(TextureIds::tex_guisettings_bloom, "Bloom", true, Settings::BLOOM);
	GuiImageButton btn_skybox = GuiImageButton(TextureIds::tex_guisettings_skybox, "Skybox", true, Settings::SKYBOX);
public:
	const int GUI_IDENTIFIER = 3;
	GuiSettings();
	~GuiSettings();
	void onMouseClick(int button, int action) override;
	void onRender(int mouseX, int mouseY, RENDERPASS pass, int colorLoc) override;
	int getIdentifier() override;
};

