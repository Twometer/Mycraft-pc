#include "GuiChat.h"
#include "ChatMessage.h"
#include "VboBuilder.h"
#include "OpenGLRenderer.h"
#include "C01PacketChat.h"
#include <glm/glm.hpp>

using namespace glm;

GuiChat::GuiChat()
{
}


GuiChat::~GuiChat()
{
}

int GuiChat::getIdentifier() {
	return GUI_IDENTIFIER;
}

void GuiChat::onKeyPress(int key, int action)
{
	if (key == GLFW_KEY_BACKSPACE && chatInput.length() > 0 && action != GLFW_RELEASE) {
		chatInput = chatInput.substr(0, chatInput.length() - 1);
	}
	if (key == GLFW_KEY_ENTER) {
		Controls::first = true;
		OpenGLRenderer::sendPacket(new C01PacketChat(chatInput.c_str()));
		OpenGLRenderer::guiRenderer->closeGui();
	}
}
void GuiChat::onCharPress(unsigned int chr)
{
	chatInput += chr;
}
void GuiChat::onRender(int mouseX, int mouseY)
{
	VboBuilder builder = VboBuilder(2);
	builder.drawRect(15, 15, OpenGLRenderer::width - 30, 25, COLORDATA(64, 64, 64, 128));
	builder.buildAndRender();
}
void GuiChat::onLoad()
{
	chatInput = "";
}
std::string GuiChat::getInput() {
	return chatInput;
}