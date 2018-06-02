#pragma once
#include "Gui.h"
#include "GuiImageButton.h"
#include "TextureIds.h"

class GuiRespawn
	: public Gui
{
private:
	const int gui_width = 388;
	const int gui_height = 250;
	GuiImageButton respawnButton = GuiImageButton(TextureIds::tex_guirespawn_respawn, "Respawn");
	GuiImageButton exitButton = GuiImageButton(TextureIds::tex_guipause_leave, "Disconnect");
public:
	const int GUI_IDENTIFIER = 2;
	GuiRespawn();
	~GuiRespawn();
	void onMouseClick(int button, int action) override;
	void onRender(int mouseX, int mouseY, RENDERPASS pass, int colorLoc) override;
	int getIdentifier() override;
	bool mayClose() override;
};

