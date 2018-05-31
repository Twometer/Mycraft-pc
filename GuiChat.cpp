#include "GuiChat.h"
#include "ChatMessage.h"
#include "VboBuilder.h"
#include "OpenGLRenderer.h"
#include "C01PacketChat.h"
#include <glm/glm.hpp>
#include "Font.h"

using namespace glm;

GuiChat::GuiChat()
{
}


GuiChat::~GuiChat()
{
}

int GuiChat::getIdentifier()
{
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
void GuiChat::onRender(int mouseX, int mouseY, RENDERPASS pass, int colorLocation)
{
	if (pass == FLAT)
	{
		VboBuilder builder = VboBuilder(2);
		builder.drawRect(10, 10, OpenGLRenderer::width - 20, 25, COLORDATA(0, 0, 0, 128));
		builder.buildAndRender();
	}
	else if (pass == FONT)
	{
		string append = clock() % 1000 < 500 ? "|" : "";
		Font::roboto.renderTextWithShadow(chatInput + append, colorLocation, 15, OpenGLRenderer::height - 17, 1.0f, vec3(1, 1, 1));
	}
}
void GuiChat::onLoad()
{
	chatInput = "";
}
std::string GuiChat::getInput()
{
	return chatInput;
}