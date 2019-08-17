#include "Game.h"
#include <unordered_map>


Game::Game()
	: mClickNumber(0)
	, mCarNumber(0)
	, mGoatNumber(0)
	, mbWin(false)
	, mbIsPlaying(false)
{
	mDoorList.reserve(3);
}

Game::~Game()
{
}

bool Game::Init()
{
	mWindow.create(sf::VideoMode(WIDTH, HEIGHT), "Monty Hall Problem Simulator");
	mGui = std::make_shared<tgui::Gui>(mWindow); // Create the gui and attach it to the window

	if (!mClosedDoorTexture.loadFromFile("resources/closedDoor.png"))
	{
		std::cout << "Cannot load image\n";
		return false;
	}

	if (!mGoatDoorTexture.loadFromFile("resources/goatDoor.png"))
	{
		std::cout << "Cannot load image\n";
		return false;
	}

	if (!mCarDoorTexture.loadFromFile("resources/carDoor.png"))
	{
		std::cout << "Cannot load image\n";
		return false;
	}

	// Load the text font
	if (!mFont.loadFromFile("resources/sansation.ttf"))
	{
		std::cout << "Cannot load font\n";
		return false;
	}

	// Initialize the start message
	mStartMessage.setFont(mFont);
	mStartMessage.setCharacterSize(40);
	mStartMessage.setPosition(170.f, 150.f);
	mStartMessage.setFillColor(sf::Color::Black);
	mStartMessage.setString("This is Monty Hall Problem simulator.\n(Press any key to continue.)");

	// Initialize three doors
	mDoorList.push_back(std::make_shared<Door>(mClosedDoorTexture)); 
	mDoorList.push_back(std::make_shared<Door>(mClosedDoorTexture));
	mDoorList.push_back(std::make_shared<Door>(mClosedDoorTexture));


	mDoorList[0]->setPosition(150, 160);
	mDoorList[1]->setPosition(500, 160);
	mDoorList[2]->setPosition(850, 160);

	std::srand(static_cast<unsigned int>(std::time(nullptr)));

	mDoorList[rand() % 3]->SetCar(true); // True for car. False for goat.

	return true;
}

void Game::Run()
{
	for (auto iter = mDoorList.begin(); iter != mDoorList.end(); iter++)
	{
		mGui->add(*iter);
		(*iter)->connect("Clicked", &Game::SignalHandler, this, *iter);
	}
	
	tgui::Label::Ptr label1 = tgui::Label::create();
	label1->setText("Run simulation");
	label1->setSize(167.2, 48);
	label1->setPosition(90, 560);
	label1->setTextSize(18);

	mGui->add(label1);

	tgui::Label::Ptr label2 = tgui::Label::create();
	label2->setText("times.    And");
	label2->setSize(127.2, 38);
	label2->setPosition(450, 560);
	label2->setTextSize(18);

	mGui->add(label2);

	tgui::Label::Ptr winMessage = tgui::Label::create();
	winMessage->setText("");
	winMessage->setSize(387.2, 46.5575);
	winMessage->setPosition(250, 40);
	winMessage->setTextSize(18);

	mGui->add(winMessage);


	tgui::ComboBox::Ptr repeatNumBox = tgui::ComboBox::create();
	repeatNumBox->addItem("10");
	repeatNumBox->addItem("50");
	repeatNumBox->addItem("100");
	repeatNumBox->addItem("500");
	repeatNumBox->addItem("10000");
	repeatNumBox->setPosition(270, 560);
	repeatNumBox->setSize(160, 22);
	repeatNumBox->setTextSize(18);

	tgui::ComboBox::Ptr choiceBox = tgui::ComboBox::create();
	choiceBox->addItem("Keep the choice.");
	choiceBox->addItem("Change the choice.");
	choiceBox->setPosition(600, 560);
	choiceBox->setSize(200, 22);
	choiceBox->setTextSize(18);

	mGui->add(repeatNumBox);
	mGui->add(choiceBox);

	tgui::Label::Ptr carLabel = tgui::Label::create();
	carLabel->setText("Car: " + std::to_string(mCarNumber));
	carLabel->setPosition(930, 560);
	carLabel->setSize(200.f, 30.6524);
	carLabel->setTextSize(18);

	mGui->add(carLabel);

	tgui::Label::Ptr goatLabel = tgui::Label::create();
	goatLabel->setText("Goat: " + std::to_string(mGoatNumber));
	goatLabel->setPosition(930, 590);
	goatLabel->setSize(200.f, 30.6524);
	goatLabel->setTextSize(18);

	mGui->add(goatLabel);

	tgui::Button::Ptr runButton = tgui::Button::create();
	runButton->setText("RUN");
	runButton->setPosition(270, 620);
	runButton->setSize(170, 62.3333);
	runButton->setTextSize(30);

	mGui->add(runButton);

	runButton->connect("Clicked", &Game::RunSignal, this, repeatNumBox, choiceBox, winMessage, carLabel, goatLabel);

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

			// Space key pressed: play
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

			if (mbIsPlaying) // If game is not started yet, then don't pass the event to the widgets.
			{
				mGui->handleEvent(event); // Pass the event to the widgets
			}
		}

		if (mClickNumber == 0)  // Remove the text when the game restarts.
		{
			winMessage->setText("");
		}

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
			mGui->draw(); // Draw all widgets
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

	for (auto iter = mDoorList.begin(); iter != mDoorList.end(); iter++)
	{
		(*iter)->getRenderer()->setTexture(mClosedDoorTexture);
		(*iter)->SetCar(false);
		(*iter)->SetOpen(false);
		(*iter)->SetClicked(false);
	}
	mbWin = false;

	mDoorList[rand() % 3]->SetCar(true); // True for car. False for goat.
}


void Game::SignalHandler(std::shared_ptr<Door> door)
{
	mClickNumber++;

	switch (mClickNumber)
	{
	case 1:
		door->SetClicked(true);
		// Step1: Show one door which has a goat.
		for (auto iter = mDoorList.begin(); iter != mDoorList.end(); iter++)
		{
			if (!(*iter)->IsClicked() && !(*iter)->IsCar()) // If the door wasn't clicked and doesn't have a car, then open this door.
			{
				(*iter)->getRenderer()->setTexture(mGoatDoorTexture);
				(*iter)->setEnabled(false); // Disable the door.
				break;
			}
		}
		break;
	case 2:
		// Step2: Make final decision
		if (door->IsCar()) // If the door has a car, then you won.
		{
			mbWin = true;
			mCarNumber++;
		}
		else  // If the door has a goat, the you lost.
		{
			mGoatNumber++; 
		}

		for (auto iter = mDoorList.begin(); iter != mDoorList.end(); iter++) // Open every door.
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
			(*iter)->setEnabled(true); // Enable every door.
		}
		break;
	case 3:
		Restart();
		break;
	}
}

void Game::RunSignal(tgui::ComboBox::Ptr repeat, tgui::ComboBox::Ptr choice, tgui::Label::Ptr winMessage, tgui::Label::Ptr carLabel, tgui::Label::Ptr goatLabel)
{
	if (repeat->getSelectedItem() == "" || choice->getSelectedItem() == "") // If the items were not selected correctly, then do nothing.
	{
		std::cout << "Do nothing\n";
		return;
	}

	int repeatNum = std::stoi(repeat->getSelectedItem().toAnsiString());  // Convert string to int

	for (int i = 0; i < repeatNum; i++)
	{
		// Step1: Choose one door randomly.
		mDoorList[rand() % 3]->SetClicked(true);


		// Step2: Show one door which has a goat.
		for (auto iter = mDoorList.begin(); iter != mDoorList.end(); iter++)
		{
			if (!(*iter)->IsClicked() && !(*iter)->IsCar()) // If the door wasn't clicked and doesn't have a car, then open this door.
			{
				(*iter)->getRenderer()->setTexture(mGoatDoorTexture);
				(*iter)->SetOpen(true);
				(*iter)->setEnabled(false); // Disable the door.
				break;
			}
		}

		// Step3: Make final decision depend on the choice.
		switch (choice->getSelectedItemIndex())
		{
		case 0: // Keep the choice.
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
		case 1: // Change the choice.
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

		for (auto iter = mDoorList.begin(); iter != mDoorList.end(); iter++) // Open every door.
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
			(*iter)->setEnabled(true); // Enable every door.
		}

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
		mGui->draw(); // Draw all widgets
		mWindow.display();

		Restart();
	}
}