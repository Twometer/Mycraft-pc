#pragma once
#include "AsyncVboBuildingManager.h"
#include "World.h"
#include "Controls.h"
#include "Frustum.h"
#include "ChatMessage.h"
#include "MinecraftSocket.h"
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
	OpenGLRenderer();
	~OpenGLRenderer();
	void start();
};

