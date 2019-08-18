#pragma once
#include <SFML/Graphics.hpp>

class Circle : public sf::CircleShape
{
public:
	// 원의 반지름을 매개변수로 받는 생성자
	Circle(float radius);
	virtual ~Circle();

	// 원의 중심 좌표 반환
	sf::Vector2f GetCenterPosition();

	// 원의 중심 좌표 설정
	void SetCenterPosition(float x, float y);
};