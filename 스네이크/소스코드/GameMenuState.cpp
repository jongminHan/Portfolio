#include "GameMenuState.h"
#include "Game.h"

GameMenuState::GameMenuState()
	: mButtonFocus(START)
{
	// 게임 시작 버튼 텍스쳐 생성
	if (!mStartButtonTexture.loadFromFile("resources/startButton.png"))
	{
		std::cout << "Cannot load image.\n";
		return;
	}

	// 게임 종료 버튼 텍스쳐 생성
	if (!mQuitButtonTexture.loadFromFile("resources/quitButton.png"))
	{
		std::cout << "Cannot load image.\n";
		return;
	}

	// 게임 타이틀 텍스쳐 생성
	if (!mGameTitleTexture.loadFromFile("resources/gameTitle.png"))
	{
		std::cout << "Cannot load image.\n";
		return;
	}

	// 게임 시작 버튼 포커스 텍스쳐 생성
	if (!mStartButtonFocusTexture.loadFromFile("resources/startButtonFocus.png"))
	{
		std::cout << "Cannot load image.\n";
		return;
	}

	// 게임 종료 버튼 텍스쳐 생성
	if (!mQuitButtonFocusTexture.loadFromFile("resources/quitButtonFocus.png"))
	{
		std::cout << "Cannot load image.\n";
		return;
	}

	mGameTitle.setTexture(mGameTitleTexture);
	mGameTitle.setPosition(450, 200);

	mStartButton.setTexture(mStartButtonFocusTexture);
	mStartButton.setPosition(565, 380);

	mQuitButton.setTexture(mQuitButtonTexture);
	mQuitButton.setPosition(565, 440);
}

GameMenuState::~GameMenuState()
{
}

void GameMenuState::Run(Game& game)
{
	if (sf::Joystick::isConnected(0))
	{
		float y = sf::Joystick::getAxisPosition(0, sf::Joystick::Y);

		// 불감대(dead zone) 설정
		// y축 절대값 50 이하의 신호는 무시
		if (y > 50.f)
		{
			if (mButtonFocus == START)
			{
				mButtonFocus = QUIT;
				mStartButton.setTexture(mStartButtonTexture);
				mQuitButton.setTexture(mQuitButtonFocusTexture);
			}
		}

		// 불감대(dead zone) 설정
		// y축 절대값 50 이하의 신호는 무시
		if (y < -50.f)
		{
			if (mButtonFocus == QUIT)
			{
				mButtonFocus = START;
				mStartButton.setTexture(mStartButtonFocusTexture);
				mQuitButton.setTexture(mQuitButtonTexture);
			}
		}

		// 종료 버튼이 눌러졌을 경우 게임 종료
		if (mButtonFocus == QUIT && sf::Joystick::isButtonPressed(0, 0)) 
		{
			game.mWindow.close();
		}

		// 시작 버튼이 눌러졌을 경우 게임 시작
		if (mButtonFocus == START && sf::Joystick::isButtonPressed(0, 0))
		{
			game.mState = game.mGamePlayState;
		}

	}

	game.mWindow.clear(sf::Color(0, 0, 0));
	game.mWindow.draw(mGameTitle);
	game.mWindow.draw(mStartButton);
	game.mWindow.draw(mQuitButton);
	game.mWindow.display();
}
