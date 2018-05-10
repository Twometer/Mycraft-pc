#pragma once
#include "AsyncVboBuildingManager.h"
#include "World.h"
#include "Controls.h"
#include "Frustum.h"
#include "ChatMessage.h"
#include "IPacket.h"
class OpenGLRenderer
{
public:
	static bool chatOpen;
	static int width;
	static int height;
	static AsyncVboBuildingManager* manager;
	static Controls* controls;
	static Frustum* frustum;
	static World* world;
	static vector<CHATMESSAGE>* chatMessages;
	static void sendPacket(IPacket * packet);
	OpenGLRenderer();
	~OpenGLRenderer();
	void start();
};

