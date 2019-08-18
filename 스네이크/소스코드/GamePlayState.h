#pragma once
#include "IGameState.h"

// 게임 플레이 상태 클래스
class GamePlayState : public IGameState
{
public:
	GamePlayState();
	~GamePlayState();
	void Run(Game& game);

	// 게임패드 신호 감지 여부를 설정
	void SetIsJoystickSignalDetected(bool enabled);
private:
	// 게임패드가 연결되었을 경우 true, 아니면 false. (false로 초기화)
	bool mbJoystickSignalDetected;
};