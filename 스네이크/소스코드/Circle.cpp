#include "Circle.h"

Circle::Circle(float radius) 
	: sf::CircleShape(radius)
{
}

Circle::~Circle()
{
}

sf::Vector2f Circle::GetCenterPosition()
{
	return sf::Vector2f(getPosition().x + getRadius(), getPosition().y + getRadius());
}

void Circle::SetCenterPosition(float x, float y)
{
	setPosition(x - getRadius(), y - getRadius());
}
