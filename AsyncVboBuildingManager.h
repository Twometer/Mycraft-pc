#pragma once
#include <concurrent_queue.h>
#include "Section.h"
class AsyncVboBuildingManager
{
private:
	concurrency::concurrent_queue<Section*>* sectionQueue;
public:
	void schedule(Section* sec);
	void initialize();
	void workerThread();
	AsyncVboBuildingManager();
	~AsyncVboBuildingManager();
};

