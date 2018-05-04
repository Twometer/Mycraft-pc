#pragma once
class Block
{
public:
	char id;
	int textureX;
	int textureY;

	Block(char id, int textureX, int textureY);
	~Block();
};

