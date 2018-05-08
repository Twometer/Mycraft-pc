#pragma once
#include "AsyncVboBuildingManager.h"
#include "World.h"
#include "Controls.h"
#include "Frustum.h"
class OpenGLRenderer
{
public:
	static int width;
	static int height;
	static AsyncVboBuildingManager* manager;
	static Controls* controls;
	static Frustum* frustum;
	static World* world;
	static vector<string*>* chatMessages;
	OpenGLRenderer();
	~OpenGLRenderer();
	void start();
};

