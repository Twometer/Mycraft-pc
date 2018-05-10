#pragma once
#include <string>
#include <ctime>

struct CHATMESSAGE {
	std::string* content;
	int timeCreated;

	CHATMESSAGE(std::string* content) {
		this->content = content;
		timeCreated = clock();
	}

	CHATMESSAGE() {

	}
};