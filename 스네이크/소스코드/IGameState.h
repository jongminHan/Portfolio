#pragma once
#include <vector>
#include <cmath>
#include <SFML/Graphics.hpp>

#include "Global.h"

class Game;

// 게임 상태 인터페이스
class IGameState
{
public:
	// 순수 가상 함수. 게임 상태에 따라 Run 함수의 동작이 바뀜.
	virtual void Run(Game& game) = 0;
};