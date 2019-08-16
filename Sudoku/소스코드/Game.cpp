#include "Game.h"

Game::Game()
{
	// Initialising seed for random number generation
	srand(static_cast<unsigned int>(time(nullptr)));

	// Creating an instance of Sudoku
	mSudoku = std::make_shared<Sudoku>();

	// Creating a seed for puzzle generation
	mSudoku->CreateSeed();

	// Generating the puzzle
	mSudoku->GenPuzzle();

	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			mSudokuGrid[i][j] = mSudoku->GetGrid(i, j);
		}
	}
}

void Game::signalHandler(int row, int col, tgui::EditBox::Ptr editBox, tgui::Label::Ptr label)
{
	try
	{
		int enteredNum = std::stoi(editBox->getText().toAnsiString());
		

		if (!IsSafe(mSudokuGrid, row, col, enteredNum)) // If the given number is not safe then,
		{
			label->setText("Warning!");
			mSudokuGrid[row][col] = enteredNum;
		}
		else
		{
			std::cout << "Safe\n";
			mSudokuGrid[row][col] = enteredNum;
		}
	}
	catch (std::invalid_argument)
	{
		if (editBox->getText().toAnsiString().size() == 0)  // Remove the assigned number when the number is deleted
		{	
			mSudokuGrid[row][col] = 0;
			label->setText(""); // Remove the text
		}
		return;
	}
	
}

bool Game::Init()
{
	mWindow.create(sf::VideoMode(WIDTH, HEIGHT), "Sudoku!");
	mGui = std::make_shared<tgui::Gui>(mWindow); // Create the gui and attach it to the window

	
	return true;
}

void Game::Run()
{
	tgui::Label::Ptr winLabel = tgui::Label::create();
	winLabel->setPosition(720, 150);
	winLabel->setSize(267.2f, 78.7863f);
	winLabel->setTextSize(50);
	winLabel->getRenderer()->setTextColor(tgui::Color::Red);
	mGui->add(winLabel);

	tgui::Button::Ptr newGameButton = tgui::Button::create();
	newGameButton->setPosition(720, 520);
	newGameButton->setSize(190.f, 69.6667f);
	newGameButton->setTextSize(25);
	newGameButton->setText("New Game");
	newGameButton->connect("pressed", &Game::CreateNewGameSignal, this);
	mGui->add(newGameButton);

	tgui::EditBox::Ptr editBox;

	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			editBox = tgui::EditBox::create();
			editBox->setPosition(150 + 60 * i, 60 + 60 * j);
			editBox->setSize(50, 50);
			editBox->setAlignment(tgui::EditBox::Alignment::Center);

			int gridNum = mSudokuGrid[i][j];

			if (gridNum != 0) // Show no number if the given number is zero.
			{
				sf::String sfStr(std::to_string(gridNum));
				editBox->setDefaultText(sfStr);
				editBox->setEnabled(false);
			}

			mSudokuEditBoxGrid[i * 9 + j] = editBox;
			mGui->add(editBox);
			editBox->connect("TextChanged", &Game::signalHandler, this, i, j, editBox, winLabel);
		}
	}


	/////////////////////Child Window/////////////////////////////////////////
	tgui::ChildWindow::Ptr winMessageWindow = tgui::ChildWindow::create();
	winMessageWindow->setPosition(430, 170);
	winMessageWindow->setSize(400, 300);
	winMessageWindow->setVisible(false);

	tgui::Label::Ptr solvedPuzzleLabel = tgui::Label::create();
	solvedPuzzleLabel->setPosition(70, 100);
	solvedPuzzleLabel->setSize(267.2f, 36.0645f);
	solvedPuzzleLabel->setText("You solved the puzzle!");
	solvedPuzzleLabel->setTextSize(20);
	solvedPuzzleLabel->setHorizontalAlignment(tgui::Label::HorizontalAlignment::Center);
	
	winMessageWindow->add(solvedPuzzleLabel);  // Add label to child window.

	tgui::Button::Ptr newGameButton2 = tgui::Button::create(); // New Game button inside child window.
	newGameButton2->setPosition(160, 170);
	newGameButton2->setSize(80.f, 29.3333f);
	newGameButton2->setText("New Game");
	newGameButton2->setTextSize(13);
	newGameButton2->connect("pressed", &Game::CreateNewGameSignalFromChild, this, winMessageWindow);
	winMessageWindow->add(newGameButton2); // Add button to child window.

	mGui->add(winMessageWindow);
	/////////////////////Child Window/////////////////////////////////////////

	sf::Vertex line1[2];
	line1[0].position = sf::Vector2f(150, 235);
	line1[0].color = sf::Color::Red;
	line1[1].position = sf::Vector2f(680, 235);
	line1[1].color = sf::Color::Red;

	sf::Vertex line2[2];
	line2[0].position = sf::Vector2f(150, 415);
	line2[0].color = sf::Color::Red;
	line2[1].position = sf::Vector2f(680, 415);
	line2[1].color = sf::Color::Red;

	sf::Vertex line3[2];
	line3[0].position = sf::Vector2f(325, 60);
	line3[0].color = sf::Color::Red;
	line3[1].position = sf::Vector2f(325, 590);
	line3[1].color = sf::Color::Red;

	sf::Vertex line4[2];
	line4[0].position = sf::Vector2f(505, 60);
	line4[0].color = sf::Color::Red;
	line4[1].position = sf::Vector2f(505, 590);
	line4[1].color = sf::Color::Red;

	while (mWindow.isOpen())
	{
		sf::Event event;
		while (mWindow.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				mWindow.close();
			}

			mGui->handleEvent(event); // Pass the event to the widgets
		}

		bool isSolved = true;

		for (int i = 0; i < 9; i++)
		{
			for (int j = 0; j < 9; j++)
			{
				if (mSudoku->GetSolutionGrid(i, j) != mSudokuGrid[i][j])
				{
					isSolved = false;
				}
			}
		}

		
		if (isSolved) // If the puzzle was solved, then show child window.
		{
			winMessageWindow->setVisible(true);
		}

		mWindow.clear(sf::Color::White);
		mWindow.draw(line1, 2, sf::Lines);
		mWindow.draw(line2, 2, sf::Lines);
		mWindow.draw(line3, 2, sf::Lines);
		mWindow.draw(line4, 2, sf::Lines);
		mGui->draw(); // Draw all widgets
		
		mWindow.display();
	}
}

void Game::CreateNewGameSignal()
{
	// Creating an new instance of Sudoku
	mSudoku = std::make_shared<Sudoku>();

	// Creating a new seed for puzzle generation
	mSudoku->CreateSeed();

	// Generating the puzzle
	mSudoku->GenPuzzle();

	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			mSudokuGrid[i][j] = mSudoku->GetGrid(i, j);
		}
	}

	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			int gridNum = mSudokuGrid[i][j];

			if (gridNum == 0) // Show no number if the given number is zero.
			{
				mSudokuEditBoxGrid[i * 9 + j]->setDefaultText("");
				mSudokuEditBoxGrid[i * 9 + j]->setEnabled(true);
			}
			else 
			{
				sf::String sfStr(std::to_string(gridNum));
				mSudokuEditBoxGrid[i * 9 + j]->setDefaultText(sfStr);
				mSudokuEditBoxGrid[i * 9 + j]->setEnabled(false);
			}
		}
	}
}

void Game::CreateNewGameSignalFromChild(tgui::ChildWindow::Ptr childWindow)
{
	CreateNewGameSignal();

	childWindow->setVisible(false); // Remove a childWindow from screen.
}
