#pragma once
#include "Gui.h"
#include <string>
#include "TextureIds.h"
#include "GuiImageButton.h"
class GuiPause :
	public Gui
{
private:
	const int gui_width = 582;
	const int gui_height = 250;
	GuiImageButton backButton = GuiImageButton(TextureIds::tex_guipause_back, "Back to game");
	GuiImageButton settingsButton = GuiImageButton(TextureIds::tex_guipause_settings, "Settings");
	GuiImageButton exitButton = GuiImageButton(TextureIds::tex_guipause_leave, "Disconnect");
public:
	static const int GUI_IDENTIFIER = 1;
	GuiPause();
	~GuiPause();
	virtual int getIdentifier();
	virtual void onRender(int mouseX, int mouseY, RENDERPASS pass, int colorLocation);
	virtual void onMouseClick(int btn, int a);
};

