#pragma once
#include <iostream>
#include "Door.h"

class Game
{
public:
	Game();
	virtual ~Game();

	// 게임 초기화 함수
	bool Init();

	// 게임 실행 함수
	void Run();

	// 게임 재시작 함수
	void Restart();

	// Run 버튼과 연결된 시그널 함수
	void RunSignal(tgui::ComboBox::Ptr repeat, tgui::ComboBox::Ptr choice, tgui::Label::Ptr winMessage, tgui::Label::Ptr carLabel, tgui::Label::Ptr goatLabel);

	// 각각의 문과 연결된 시그널 함수
	void SignalHandler(std::shared_ptr<Door> door);
private:
	const int WIDTH = 1280;
	const int HEIGHT = 720;
	sf::Font mFont;
	sf::Text mStartMessage;
	std::shared_ptr<tgui::Gui> mGui;
	sf::RenderWindow mWindow;
	sf::Texture mClosedDoorTexture; // 닫힌 문 텍스쳐
	sf::Texture mGoatDoorTexture;   // 염소 텍스쳐
	sf::Texture mCarDoorTexture;    // 자동차 텍스쳐
	std::vector<std::shared_ptr<Door>> mDoorList; // 문 3개를 저장하는 벡터
	int mClickNumber;
	int mCarNumber;  // 자동차를 선택한 횟수를 저장
	int mGoatNumber; // 염소를 선택한 횟수를 저장
	bool mbWin;
	bool mbIsPlaying;
};