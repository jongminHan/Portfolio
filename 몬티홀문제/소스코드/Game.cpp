#include "Game.h"
#include <unordered_map>


Game::Game()
	: mClickNumber(0)
	, mCarNumber(0)
	, mGoatNumber(0)
	, mbWin(false)
	, mbIsPlaying(false)
{
}

Game::~Game()
{

}

bool Game::Init()
{

	// 닫힌 문 텍스쳐 가져오기
	if (!mClosedDoorTexture.loadFromFile("resources/closedDoor.png"))
	{
		std::cout << "Cannot load image\n";
		return false;
	}

	// 염소 텍스쳐 가져오기
	if (!mGoatDoorTexture.loadFromFile("resources/goatDoor.png"))
	{
		std::cout << "Cannot load image\n";
		return false;
	}

	// 자동차 텍스쳐 가져오기
	if (!mCarDoorTexture.loadFromFile("resources/carDoor.png"))
	{
		std::cout << "Cannot load image\n";
		return false;
	}

	// 폰트 가져오기
	if (!mFont.loadFromFile("resources/sansation.ttf"))
	{
		std::cout << "Cannot load font\n";
		return false;
	}

	// 윈도우 생성
	mWindow.create(sf::VideoMode(WIDTH, HEIGHT), "Monty Hall Problem Simulator");

	// GUI 생성
	mGui = std::make_shared<tgui::Gui>(mWindow);

	// 시작 메시지 초기화
	mStartMessage.setFont(mFont);
	mStartMessage.setCharacterSize(40);
	mStartMessage.setPosition(170.f, 150.f);
	mStartMessage.setFillColor(sf::Color::Black);
	mStartMessage.setString("This is Monty Hall Problem simulator.\n(Press any key to continue.)");

	// 문을 저장할 메모리 예약
	mDoorList.reserve(3);

	// 문 3개 초기화
	mDoorList.push_back(std::make_shared<Door>(mClosedDoorTexture));
	mDoorList.push_back(std::make_shared<Door>(mClosedDoorTexture));
	mDoorList.push_back(std::make_shared<Door>(mClosedDoorTexture));

	mDoorList[0]->setPosition(150, 160);
	mDoorList[1]->setPosition(500, 160);
	mDoorList[2]->setPosition(850, 160);

	// seed값 생성
	std::srand(static_cast<unsigned int>(std::time(nullptr)));

	// 무작위로 한 개의 문을 선택 후 자동차 넣기. 나머지 문 두 개는 염소 넣기.
	mDoorList[rand() % 3]->SetCar(true);

	return true;
}

void Game::Run()
{
	for (auto iter = mDoorList.begin(); iter != mDoorList.end(); iter++)
	{
		mGui->add(*iter);
		(*iter)->connect("Clicked", &Game::SignalHandler, this, *iter);
	}
	
	// 출력할 메시지: Run simulation {ComboBox} times. And {ComboBox}

	// 텍스트 레이블 생성 
	tgui::Label::Ptr label1 = tgui::Label::create();
	label1->setText("Run simulation");
	label1->setSize(167.2, 48);
	label1->setPosition(90, 560);
	label1->setTextSize(18);

	mGui->add(label1);

	// 텍스트 레이블 생성 
	tgui::Label::Ptr label2 = tgui::Label::create();
	label2->setText("times.    And");
	label2->setSize(127.2, 38);
	label2->setPosition(450, 560);
	label2->setTextSize(18);

	mGui->add(label2);

	// 자동차 선택 축하 메시지를 출력하는 레이블
	tgui::Label::Ptr winMessage = tgui::Label::create();
	winMessage->setText("");
	winMessage->setSize(387.2, 46.5575);
	winMessage->setPosition(250, 40);
	winMessage->setTextSize(18);

	mGui->add(winMessage);

	// 시뮬레이션을 반복할 횟수를 결정
	tgui::ComboBox::Ptr repeatNumBox = tgui::ComboBox::create();
	repeatNumBox->addItem("10");
	repeatNumBox->addItem("50");
	repeatNumBox->addItem("100");
	repeatNumBox->addItem("500");
	repeatNumBox->addItem("10000");
	repeatNumBox->setPosition(270, 560);
	repeatNumBox->setSize(160, 22);
	repeatNumBox->setTextSize(18);

	// 선택을 유지할지, 바꿀지를 결정
	tgui::ComboBox::Ptr choiceBox = tgui::ComboBox::create();
	choiceBox->addItem("Keep the choice.");
	choiceBox->addItem("Change the choice.");
	choiceBox->setPosition(600, 560);
	choiceBox->setSize(200, 22);
	choiceBox->setTextSize(18);

	mGui->add(repeatNumBox);
	mGui->add(choiceBox);

	// 자동차 선택 횟수 출력
	tgui::Label::Ptr carLabel = tgui::Label::create();
	carLabel->setText("Car: " + std::to_string(mCarNumber));
	carLabel->setPosition(930, 560);
	carLabel->setSize(200.f, 30.6524);
	carLabel->setTextSize(18);

	mGui->add(carLabel);

	// 염소 선택 횟수 출력
	tgui::Label::Ptr goatLabel = tgui::Label::create();
	goatLabel->setText("Goat: " + std::to_string(mGoatNumber));
	goatLabel->setPosition(930, 590);
	goatLabel->setSize(200.f, 30.6524);
	goatLabel->setTextSize(18);

	mGui->add(goatLabel);

	// 실행 버튼
	tgui::Button::Ptr runButton = tgui::Button::create();
	runButton->setText("RUN");
	runButton->setPosition(270, 620);
	runButton->setSize(170, 62.3333);
	runButton->setTextSize(30);

	mGui->add(runButton);

	runButton->connect("Clicked", &Game::RunSignal, this, repeatNumBox, choiceBox, winMessage, carLabel, goatLabel);

	// 키보드가 눌러진 횟수를 저장
	int keyPressCounter = 0;
	while (mWindow.isOpen())
	{
		sf::Event event;
		while (mWindow.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				mWindow.close();
			}

			// 키보드 버튼 입력 횟수에 따른 튜토리얼 메시지 출력.
			if (event.type == sf::Event::KeyPressed)
			{
				keyPressCounter++;
				switch (keyPressCounter)
				{
				case 1:
					mStartMessage.setString("You are invited to a famous TV show, \nhosted by Monty Hall.\n(Press any key to continue.)");
					break;
				case 2:
					mStartMessage.setString("You are going to play a game on TV show.\nThere are three doors.\n(Press any key to continue.)");
					break;
				case 3:
					mStartMessage.setString("One door has Lamborghini behind it.\nThe other two doors has goats.\n(Press any key to continue.)");
					break;
				case 4:
					mStartMessage.setString("You will get Lamborghini if you choose a door\nwhich has Lamborghini.\nYou will get a goat if you choose a door\nwhich has a goat.\n(Press any key to continue.)");
					break;
				case 5:
					mStartMessage.setString("If you choose one door, the host will open\none of two doors which has a goat.\nYou can either change your choice, or keep your choice.\nIt's up to you.\n(Press any key to continue.)");
					break;
				case 6:
					mStartMessage.setString("Then, let's play the game!\n(Press any key to continue.)");
					break;
				default:
					mbIsPlaying = true;
					break;

				}
			}

			// 게임이 시작되기 전에는 event를 위젯에 전달하지 않는다.
			if (mbIsPlaying)
			{
				mGui->handleEvent(event);
			}
		}

		// 게임을 재시작할 경우 텍스트 메시지를 삭제한다.
		if (mClickNumber == 0)
		{
			winMessage->setText("");
		}

		// 자동차를 얻었는지, 염소를 얻었는지 알려주는 메시지를 출력한다.
		if (mClickNumber == 2)
		{
			if (mbWin)
			{
				winMessage->setText("You won a car!\nClick any door to restart the game.");
				carLabel->setText("Car: " + std::to_string(mCarNumber));
			}
			else
			{
				winMessage->setText("You got a goat.\nClick any door to restart the game.");
				goatLabel->setText("Goat: " + std::to_string(mGoatNumber));
			}
		}

		mWindow.clear(sf::Color::White);
		if (mbIsPlaying)
		{
			mGui->draw(); // 모든 위젯을 그린다.
		}
		else
		{
			mWindow.draw(mStartMessage);
		}
		mWindow.display();
	}
}

void Game::Restart()
{
	mClickNumber = 0;

	// 모든 문의 속성을 초기화한다.
	for (auto iter = mDoorList.begin(); iter != mDoorList.end(); iter++)
	{
		(*iter)->getRenderer()->setTexture(mClosedDoorTexture);
		(*iter)->SetCar(false);
		(*iter)->SetOpen(false);
		(*iter)->SetClicked(false);
	}
	mbWin = false;

	// 3개의 문 중 하나를 무작위로 선택해 자동차를 넣는다. 나머지 두 개의 문은 염소를 넣는다.
	mDoorList[rand() % 3]->SetCar(true);
}


void Game::SignalHandler(std::shared_ptr<Door> door)
{
	mClickNumber++;

	switch (mClickNumber)
	{
	case 1:
		door->SetClicked(true);

		// 1단계: 염소가 있는 문을 열어준다.
		for (auto iter = mDoorList.begin(); iter != mDoorList.end(); iter++)
		{
			// 클릭되지 않았고, 염소가 있는 문이라면 열어준다.
			if (!(*iter)->IsClicked() && !(*iter)->IsCar())
			{
				(*iter)->getRenderer()->setTexture(mGoatDoorTexture);

				// 열린 문을 비활성화한다.
				(*iter)->setEnabled(false); 
				break;
			}
		}
		break;
	case 2:
		// 2단계: 최종 선택을 내린다.
		if (door->IsCar()) // 최종 선택한 문에 자동차가 있었다면, 승리.
		{
			mbWin = true;
			mCarNumber++;
		}
		else  // 최종 선택한 문에 염소가 있었다면, 패배.
		{
			mGoatNumber++; 
		}

		// 모든 문을 열어준다.
		for (auto iter = mDoorList.begin(); iter != mDoorList.end(); iter++)
		{
			if ((*iter)->IsCar())
			{
				(*iter)->getRenderer()->setTexture(mCarDoorTexture);

			}
			else
			{
				(*iter)->getRenderer()->setTexture(mGoatDoorTexture);
			}
			(*iter)->SetOpen(true);
			(*iter)->setEnabled(true); // 모든 문을 활성화한다
		}
		break;
	case 3:
		Restart();
		break;
	}
}

void Game::RunSignal(tgui::ComboBox::Ptr repeat, tgui::ComboBox::Ptr choice, tgui::Label::Ptr winMessage, tgui::Label::Ptr carLabel, tgui::Label::Ptr goatLabel)
{
	// 콤보박스가 제대로 선택되지 않았다면, 아무것도 하지 않는다.
	if (repeat->getSelectedItem() == "" || choice->getSelectedItem() == "")
	{
		return;
	}

	// 콤보박스로부터 전달받은 string을 int로 변환한다.
	int repeatNum = std::stoi(repeat->getSelectedItem().toAnsiString()); 

	for (int i = 0; i < repeatNum; i++)
	{

		// 1단계: 문 하나를 무작위로 선택한다.
		mDoorList[rand() % 3]->SetClicked(true);


		// 2단계: 염소가 있는 문을 하나 열어준다.
		for (auto iter = mDoorList.begin(); iter != mDoorList.end(); iter++)
		{
			// 클릭되지 않았고, 염소가 있는 문이라면 열어준다.
			if (!(*iter)->IsClicked() && !(*iter)->IsCar())
			{
				(*iter)->getRenderer()->setTexture(mGoatDoorTexture);
				(*iter)->SetOpen(true);

				// 열린 문을 비활성화한다.
				(*iter)->setEnabled(false);
				break;
			}
		}

		// 3단계: choice 콤보박스의 옵션에 따라, 최종 결정을 내린다.
		switch (choice->getSelectedItemIndex())
		{
		case 0: // Keep the choice.를 선택했을 경우
			for (auto iter = mDoorList.begin(); iter != mDoorList.end(); iter++)
			{
				if ((*iter)->IsClicked())
				{
					if ((*iter)->IsCar())
					{
						mbWin = true;
						mCarNumber++;
					}
					else
					{
						mGoatNumber++;
					}
					break;
				}
			}
			break;
		case 1: // Change the choice.를 선택했을 경우
			for (auto iter = mDoorList.begin(); iter != mDoorList.end(); iter++)
			{
				if (!(*iter)->IsOpened() && !(*iter)->IsClicked())
				{
					if ((*iter)->IsCar())
					{
						mbWin = true;
						mCarNumber++;
					}
					else
					{
						mGoatNumber++;
					}
					break;
				}
			}
			break;
		}

		// 모든 문을 열어준다.
		for (auto iter = mDoorList.begin(); iter != mDoorList.end(); iter++)
		{
			if ((*iter)->IsCar())
			{
				(*iter)->getRenderer()->setTexture(mCarDoorTexture);

			}
			else
			{
				(*iter)->getRenderer()->setTexture(mGoatDoorTexture);
			}
			(*iter)->SetOpen(true);

			// 모든 문을 활성화한다.
			(*iter)->setEnabled(true);
		}

		// 승리했을 시 자동차 획득 메시지 출력
		// 패배했을 시 염소 획득 메시지 출력
		// 지금까지 얻은 자동차, 염소의 수를 출력
		// 자동차의 비율과 염소의 비율을 출력
		if (mbWin)
		{
			winMessage->setText("You won a car!\nClick any door to restart the game.");
			
			carLabel->setText("Car: " + std::to_string(mCarNumber) + "(" + std::to_string(static_cast<float>(mCarNumber) / static_cast<float>(mCarNumber + mGoatNumber)) + ")");
		}
		else
		{
			winMessage->setText("You got a goat.\nClick any door to restart the game.");
			goatLabel->setText("Goat: " + std::to_string(mGoatNumber) + "(" + std::to_string(static_cast<float>(mGoatNumber) / static_cast<float>(mCarNumber + mGoatNumber)) + ")");
		}

		mWindow.clear(sf::Color::White);
		mGui->draw();
		mWindow.display();

		// 게임 재시작
		Restart();
	}
}