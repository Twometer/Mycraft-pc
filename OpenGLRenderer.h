#pragma once
#include "AsyncVboBuildingManager.h"
#include "World.h"
#include "Controls.h"
#include "Frustum.h"
#include "ChatMessage.h"
#include "IPacket.h"
#include "GuiRenderer.h"
class OpenGLRenderer
{
public:
	static int width;
	static int height;
	static AsyncVboBuildingManager* manager;
	static Controls* controls;
	static Frustum* frustum;
	static World* world;
	static GuiRenderer* guiRenderer;
	static vector<CHATMESSAGE>* chatMessages;
	static void sendPacket(IPacket * packet);
	static void setCursorVisibility(bool visible);
	static void onKilled();
	OpenGLRenderer();
	~OpenGLRenderer();
	void start();
};

