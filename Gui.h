#pragma once
class Gui
{
public:
	Gui();
	~Gui();
	virtual void onMouseClick(int button, int action);
	virtual void onKeyPress(int key, int action);
	virtual void onCharPress(unsigned int chr);
	virtual void onLoad();
	virtual void onRender(int mouseX, int mouseY) = 0;
	virtual int getIdentifier() = 0;
};

