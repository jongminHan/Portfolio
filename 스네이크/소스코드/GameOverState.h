#pragma once
#include "IGameState.h"

// 게임 오버 상태 클래스
class GameOverState : public IGameState
{
public:
	GameOverState();
	~GameOverState();
	void Run(Game& game);
};