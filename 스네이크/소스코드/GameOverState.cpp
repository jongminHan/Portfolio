#include "GameOverState.h"
#include "Game.h"

GameOverState::GameOverState()
{
}

GameOverState::~GameOverState()
{
}

void GameOverState::Run(Game& game)
{
	// 뱀 메모리 해제
	for (auto iter = game.mSnake.begin(); iter != game.mSnake.end(); iter++)  
	{
		delete *iter;
	}
	game.mSnake.clear();

	// 먹이 메모리 해제
	for (auto iter = game.mFood.begin(); iter != game.mFood.end(); iter++) 
	{
		if (*iter != nullptr)
		{
			delete *iter;
		}
	}
	game.mFood.clear();

	// 독먹이 메모리 해제
	for (auto iter = game.mPoison.begin(); iter != game.mPoison.end(); iter++)
	{
		if (*iter != nullptr)
		{
			delete *iter;
		}
	}
	game.mPoison.clear();

	game.mPauseMessage.setString("GAME OVER\nPress A to restart the game\nPress B to quit the game");

	// 게임패드 B버튼이 눌러졌을 경우 게임 종료
	if (sf::Joystick::isButtonPressed(0, 1))
	{
		game.mWindow.close();
	}

	game.mWindow.clear(sf::Color(0, 0, 0));
	// 게임 오버 메시지 출력
	game.mWindow.draw(game.mPauseMessage);
	game.mWindow.display();
}