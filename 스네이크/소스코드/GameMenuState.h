#pragma once
#include "IGameState.h"
#include "eButtonFocus.h"

// 게임 메뉴 상태 클래스
class GameMenuState : public IGameState
{
public:
	GameMenuState();
	virtual ~GameMenuState();
	void Run(Game& game);
private:
	// 게임 제목
	sf::Sprite mGameTitle;

	// 게임 시작 버튼
	sf::Sprite mStartButton;

	// 게임 종료 버튼
	sf::Sprite mQuitButton;

	sf::Texture mStartButtonTexture;
	sf::Texture mQuitButtonTexture;
	sf::Texture mGameTitleTexture;
	sf::Texture mQuitButtonFocusTexture;
	sf::Texture mStartButtonFocusTexture;

	// 어느 버튼이 포커스되어 있는지 저장
	eButtonFocus mButtonFocus;
};