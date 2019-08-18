#pragma once
#include <vector>
#include <cmath>
#include <iostream>
#include <SFML/Audio.hpp>
#include "Circle.h"
#include "GameMenuState.h"
#include "GamePlayState.h"
#include "GameOverState.h"

class Game
{
	friend class GameMenuState;
	friend class GamePlayState;
	friend class GameOverState;
public:
	Game();
	virtual ~Game();

	// 게임 초기화
	bool Init();

	// 게임 실행
	void Run();

	// Snake 움직이기
	void MoveSnake(float x, float y);

	// 두 점의 직선 거리 계산
	float GetDistance(sf::Vector2f point1, sf::Vector2f point2);

	// 두 점이 만드는 직선이 만드는 각도에 해당하는 코사인 값 계산
	float GetCOS(sf::Vector2f point1, sf::Vector2f point2);

	// 두 점이 만드는 직선이 만드는 각도에 해당하는 사인 값 계산
	float GetSIN(sf::Vector2f point1, sf::Vector2f point2);

	// 먹이 충돌 감지
	void DetectFoodCollision();

	// 독먹이 충돌 감지
	void DetectPoisonCollision();
private:
	sf::RenderWindow mWindow;
	sf::SoundBuffer mEatingSoundBuffer;
	sf::Sound mEatingSound;
	std::vector<Circle*> mSnake;
	std::vector<Circle*> mFood;
	std::vector<Circle*> mPoison;
	float mDeltaTime;
	float mRadius;
	sf::Font mFont;
	sf::Text mPauseMessage;
	sf::Clock mClock;
	IGameState* mState;
	GameMenuState* mGameMenuState;
	GameOverState* mGameOverState;
	GamePlayState* mGamePlayState;
};