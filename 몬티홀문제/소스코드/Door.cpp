#include "Door.h"

Door::Door(const sf::Texture& texture)
	: tgui::Picture(texture)
	, mbIsCar(false)
	, mbIsOpened(false)
	, mbIsClicked(false)
{
}

Door::~Door()
{
}

bool Door::IsCar()
{
	return mbIsCar;
}

bool Door::IsOpened()
{
	return mbIsOpened;
}

bool Door::IsClicked()
{
	return mbIsClicked;
}

void Door::SetCar(bool car)
{
	mbIsCar = car;
}

void Door::SetOpen(bool open)
{
	mbIsOpened = open;
}

void Door::SetClicked(bool click)
{
	mbIsClicked = click;
}
