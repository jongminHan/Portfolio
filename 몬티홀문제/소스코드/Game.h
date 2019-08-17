#pragma once
#include <iostream>
#include "Door.h"

class Game
{
public:
	Game();
	virtual ~Game();
	bool Init();
	void Run();
	void Restart();
	void RunSignal(tgui::ComboBox::Ptr repeat, tgui::ComboBox::Ptr choice, tgui::Label::Ptr winMessage, tgui::Label::Ptr carLabel, tgui::Label::Ptr goatLabel);
	void SignalHandler(std::shared_ptr<Door> door);
private:
	const int WIDTH = 1280;
	const int HEIGHT = 720;
	sf::Font mFont;
	sf::Text mStartMessage;
	std::shared_ptr<tgui::Gui> mGui;
	sf::RenderWindow mWindow;
	sf::Texture mClosedDoorTexture;
	sf::Texture mGoatDoorTexture;
	sf::Texture mCarDoorTexture;
	std::vector<std::shared_ptr<Door>> mDoorList;
	int mClickNumber;
	int mCarNumber;
	int mGoatNumber;
	bool mbWin;
	bool mbIsPlaying;
};