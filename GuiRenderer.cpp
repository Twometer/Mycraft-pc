#include "GuiRenderer.h"
#include "GLFW/glfw3.h"
#include "Controls.h"

GuiRenderer::GuiRenderer()
{
}


GuiRenderer::~GuiRenderer()
{
}

void GuiRenderer::onMouseClick(int button, int action)
{
	if (currentGui != nullptr)
		currentGui->onMouseClick(button, action);
}

void GuiRenderer::onKeyPress(int key, int action)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_RELEASE)
		closeGui();
	if (currentGui != nullptr)
		currentGui->onKeyPress(key, action);
}

void GuiRenderer::onCharPress(unsigned int chr)
{
	if (currentGui != nullptr)
		currentGui->onCharPress(chr);
}

void GuiRenderer::onRender(int mouseX, int mouseY, RENDERPASS pass, int colorLoc)
{
	if (currentGui != nullptr)
		currentGui->onRender(mouseX, mouseY, pass, colorLoc);
}

void GuiRenderer::displayGui(Gui* gui)
{
	currentGui = gui;
}

void GuiRenderer::closeGui()
{
	currentGui = nullptr;
	Controls::first = true;
}

bool GuiRenderer::isGuiOpen() 
{
	return currentGui != nullptr;
}

bool GuiRenderer::isGuiOpen(int id)
{
	return currentGui != nullptr && currentGui->getIdentifier() == id;
}

Gui* GuiRenderer::getCurrentGui()
{
	return currentGui;
}