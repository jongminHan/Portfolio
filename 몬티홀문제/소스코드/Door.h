#pragma once
#include <TGUI/TGUI.hpp>

class Door : public tgui::Picture
{
public:
	Door(const sf::Texture& texture);
	virtual ~Door();

	// 자동차일 경우 true를 반환,
	// 염소일 경우 false를 반환하는 함수
	bool IsCar();

	// 문이 열려있을 경우 true를 반환,
	// 문이 닫혀있을 경우 false를 반환하는 함수
	bool IsOpened();

	// 문이 클릭됐을 경우 true를 반환,
	// 문이 클릭된 상태가 아닐 경우 false를 반환하는 함수
	bool IsClicked();

	// 문 뒤에 자동차를 넣을 경우 true를 입력,
	// 문 뒤에 염소를 넣을 경우 false를 입력.
	void SetCar(bool car);

	// 문을 열 때는 true를 입력,
	// 문을 닫을 때는 false를 입력.
	void SetOpen(bool open);

	// 문을 클릭된 상태로 만들 때 true를 입력,
	// 문이 클릭되지 않은 상태로 만들 때 false를 입력.
	void SetClicked(bool click);
private:
	bool mbIsCar; // 자동차는 true, 염소는 false로 나타냄
	bool mbIsOpened;
	bool mbIsClicked;
};