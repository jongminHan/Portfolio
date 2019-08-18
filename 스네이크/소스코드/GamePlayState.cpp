#include "GamePlayState.h"
#include "Game.h"

GamePlayState::GamePlayState()
	: mbJoystickSignalDetected(false)
{
}

GamePlayState::~GamePlayState()
{
}

void GamePlayState::Run(Game& game)
{
	// 먹이 생성
	if (rand() % 200 == 0) 
	{
		Circle* food = new Circle(game.mRadius);
		food->setPosition(static_cast<float>(rand() % (GAME_WIDTH - static_cast<int>(game.mRadius))), static_cast<float>(rand() % (GAME_HEIGHT - static_cast<int>(game.mRadius))));
		food->setFillColor(sf::Color(0, 0, 255));

		if (game.mFood.empty())
		{
			game.mFood.push_back(food);
		}
		else
		{
			bool bNullFound = false;
			size_t size = game.mFood.size();

			// 비어있는 메모리 공간 탐색
			for (size_t i = 0; i < size; i++) 
			{
				if (game.mFood[i] == nullptr)
				{
					game.mFood[i] = food;
					bNullFound = true;
					break;
				}
			}

			if (!bNullFound)
			{
				game.mFood.push_back(food);
			}
		}
	}

	// 독 먹이 생성
	if (rand() % 500 == 0)
	{
		Circle* poison = new Circle(game.mRadius);
		poison->setPosition(static_cast<float>(rand() % (GAME_WIDTH - static_cast<int>(game.mRadius))), static_cast<float>(rand() % (GAME_HEIGHT - static_cast<int>(game.mRadius))));
		poison->setFillColor(sf::Color(255, 0, 0)); // 독 먹이를 붉은색으로 설정.

		if (game.mPoison.empty())
		{
			game.mPoison.push_back(poison);
		}
		else
		{
			bool bNullFound = false;
			size_t size = game.mPoison.size();

			// 비어있는 메모리 공간 탐색
			for (size_t i = 0; i < size; i++)
			{
				if (game.mPoison[i] == nullptr)
				{
					game.mPoison[i] = poison;
					bNullFound = true;
					break;
				}
			}

			if (!bNullFound)
			{
				game.mPoison.push_back(poison);
			}
		}
	}

	game.mDeltaTime = game.mClock.restart().asSeconds();

	// 게임패드가 연결되어있는지 확인
	if (sf::Joystick::isConnected(0))
	{
		float x = sf::Joystick::getAxisPosition(0, sf::Joystick::X);
		float y = sf::Joystick::getAxisPosition(0, sf::Joystick::Y);


		// 불감대(dead zone) 설정
		// x축, y축 모두 절대값 15이하의 신호 무시
		if (x > 15.f || x < -15.f || y > 15.f || y < -15.f)
		{
			mbJoystickSignalDetected = true;
			game.MoveSnake(x, y);
			game.DetectFoodCollision();
			game.DetectPoisonCollision();
		}
		else
		{
			if (mbJoystickSignalDetected)
			{
				int speed = 30;
				// cos@ = delta x / 2r
				// sin@ = delta y / 2r
				// MoveSnake(speed * cos@, speed * sin@)
				game.MoveSnake(speed * (game.mSnake[0]->GetCenterPosition().x - game.mSnake[1]->GetCenterPosition().x) / (2 * game.mRadius),
					speed * (game.mSnake[0]->GetCenterPosition().y - game.mSnake[1]->GetCenterPosition().y) / (2 * game.mRadius));
				game.DetectFoodCollision();
				game.DetectPoisonCollision();
			}
		}
	}
	game.mWindow.clear(sf::Color(0, 0, 0));
	for (size_t i = 0; i < game.mSnake.size(); i++)
	{
		game.mWindow.draw(*game.mSnake[i]);
	}

	for (size_t i = 0; i < game.mFood.size(); i++)
	{
		if (game.mFood[i] != nullptr)
		{
			game.mWindow.draw(*game.mFood[i]);
		}
	}

	for (size_t i = 0; i < game.mPoison.size(); i++)
	{
		if (game.mPoison[i] != nullptr)
		{
			game.mWindow.draw(*game.mPoison[i]);
		}
	}
	game.mWindow.display();
}

void GamePlayState::SetIsJoystickSignalDetected(bool enabled)
{
	mbJoystickSignalDetected = enabled;
}
