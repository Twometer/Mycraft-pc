#include "AsyncVboBuildingManager.h"
#include <thread>
#include <iostream>
#include <concurrent_queue.h>
#include "Section.h"
#include "SectionBuilder.h"

using namespace std;
using namespace concurrency;

AsyncVboBuildingManager::AsyncVboBuildingManager()
{
}


AsyncVboBuildingManager::~AsyncVboBuildingManager()
{
}


void AsyncVboBuildingManager::initialize() {

	cout << "Setting up queue" << endl;
	sectionQueue = new concurrent_queue<Section*>;

	for (int i = 0; i < 4; i++) {
		cout << "Starting VBO Builder #" << i << endl;
		thread* wt = new thread(&AsyncVboBuildingManager::workerThread, this);
	}

}

void AsyncVboBuildingManager::schedule(Section* sec) {
	sectionQueue->push(sec);
}

void AsyncVboBuildingManager::workerThread() {
	cout << "VBO Builder started" << endl;
	while (true) {
		Section* sec;
		if (sectionQueue->try_pop(sec)) {
			sec->buildVertexData();
		}
		else {
			this_thread::sleep_for(chrono::milliseconds(20));
		}
	}
}