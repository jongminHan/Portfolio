#include "Game.h"

Game::Game()
{
	// seed값 생성
	srand(static_cast<unsigned int>(time(nullptr)));

	// 스도쿠 오브젝트 생성
	mSudoku = std::make_shared<Sudoku>();

	// 스도쿠 seed 생성
	mSudoku->CreateSeed();

	// 스도쿠 퍼즐 생성
	mSudoku->GenPuzzle();

	// mSudoku의 격자를 Game의 격자로 복사
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			mSudokuGrid[i][j] = mSudoku->GetGrid(i, j);
		}
	}
}

void Game::EditBoxSignalHandler(int row, int col, tgui::EditBox::Ptr editBox, tgui::Label::Ptr label)
{
	try
	{
		// editBox에 입력된 텍스트를 int로 변환. 변환이 불가능할 경우 예외 발생
		int enteredNum = std::stoi(editBox->getText().toAnsiString());
		

		// 입력된 값이 안전하지 않은 경우 경고 메시지 출력.
		if (!IsSafe(mSudokuGrid, row, col, enteredNum)) 
		{
			label->setText("Warning!");
			mSudokuGrid[row][col] = enteredNum;
		}
		else 
		{
			mSudokuGrid[row][col] = enteredNum;
		}
	}
	catch (std::invalid_argument) // 예외처리: 유효하지 않은 값
	{
		// editBox의 내용이 삭제되었을 경우 해당 위치에 0을 입력
		if (editBox->getText().toAnsiString().size() == 0)
		{	
			mSudokuGrid[row][col] = 0;

			// 텍스트 제거
			label->setText("");
		}
		return;
	}
	
}

bool Game::Init()
{
	// 윈도우 생성
	mWindow.create(sf::VideoMode(WIDTH, HEIGHT), "Sudoku!");

	// GUI 생성
	mGui = std::make_shared<tgui::Gui>(mWindow);

	return true;
}

void Game::Run()
{
	// 경고 메시지를 출력하는 레이블
	tgui::Label::Ptr warningLabel = tgui::Label::create();
	warningLabel->setPosition(720, 150);
	warningLabel->setSize(267.2f, 78.7863f);
	warningLabel->setTextSize(50);
	warningLabel->getRenderer()->setTextColor(tgui::Color::Red);
	mGui->add(warningLabel);


	// 새 게임을 만드는 버튼
	tgui::Button::Ptr newGameButton = tgui::Button::create();
	newGameButton->setPosition(720, 520);
	newGameButton->setSize(190.f, 69.6667f);
	newGameButton->setTextSize(25);
	newGameButton->setText("New Game");
	newGameButton->connect("pressed", &Game::CreateNewGameSignal, this);
	mGui->add(newGameButton);

	tgui::EditBox::Ptr editBox;

	// 9 X 9 형태의 EditBox로 이루어진 격자 생성
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			editBox = tgui::EditBox::create();
			editBox->setPosition(150 + 60 * i, 60 + 60 * j);
			editBox->setSize(50, 50);
			editBox->setAlignment(tgui::EditBox::Alignment::Center);

			int gridNum = mSudokuGrid[i][j];

			// 현재 위치의 mSudokuGrid 값이 0이 아닐 경우 EditBox에 숫자 출력
			if (gridNum != 0)
			{
				sf::String sfStr(std::to_string(gridNum));
				editBox->setDefaultText(sfStr);
				editBox->setEnabled(false);
			}

			mSudokuEditBoxGrid[i * 9 + j] = editBox;
			mGui->add(editBox);
			editBox->connect("TextChanged", &Game::EditBoxSignalHandler, this, i, j, editBox, warningLabel);
		}
	}


	///////////////////// 자식 윈도우 /////////////////////
	// 스도쿠를 풀었을 경우 팝업되는 윈도우
	tgui::ChildWindow::Ptr winMessageWindow = tgui::ChildWindow::create();
	winMessageWindow->setPosition(430, 170);
	winMessageWindow->setSize(400, 300);

	// 스도쿠를 풀기 전에는 보이지 않도록 설정
	winMessageWindow->setVisible(false);

	// 축하 메시지를 출력하는 레이블
	tgui::Label::Ptr solvedPuzzleLabel = tgui::Label::create();
	solvedPuzzleLabel->setPosition(70, 100);
	solvedPuzzleLabel->setSize(267.2f, 36.0645f);
	solvedPuzzleLabel->setText("You solved the puzzle!");
	solvedPuzzleLabel->setTextSize(20);
	solvedPuzzleLabel->setHorizontalAlignment(tgui::Label::HorizontalAlignment::Center);
	

	// 축하 메시지 레이블을 자식 윈도우에 추가
	winMessageWindow->add(solvedPuzzleLabel);

	// 새 게임 시작 버튼
	tgui::Button::Ptr newGameButton2 = tgui::Button::create(); 
	newGameButton2->setPosition(160, 170);
	newGameButton2->setSize(80.f, 29.3333f);
	newGameButton2->setText("New Game");
	newGameButton2->setTextSize(13);
	newGameButton2->connect("pressed", &Game::CreateNewGameSignalFromChild, this, winMessageWindow);

	// 새 게임 시작 버튼을 자식 윈도우에 추가
	winMessageWindow->add(newGameButton2);

	mGui->add(winMessageWindow);
	///////////////////// 자식 윈도우 /////////////////////


	// 9 X 9 격자를 9개의 상자로 나눠주는 4개의 빨간 선
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

			// event를 위젯에 전달
			mGui->handleEvent(event);
		}

		// mSudoku의 솔루션과 Game의 mSudokuGrid의 값이 하나라도 다르면 문제를 못푼 것으로 간주
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

		// 문제가 풀렸을 경우, 자식 윈도우를 보여줌.
		if (isSolved)
		{
			winMessageWindow->setVisible(true);
		}

		mWindow.clear(sf::Color::White);
		mWindow.draw(line1, 2, sf::Lines);
		mWindow.draw(line2, 2, sf::Lines);
		mWindow.draw(line3, 2, sf::Lines);
		mWindow.draw(line4, 2, sf::Lines);
		mGui->draw();
		mWindow.display();
	}
}

void Game::CreateNewGameSignal()
{
	// 스도쿠 오브젝트를 새로 생성
	mSudoku = std::make_shared<Sudoku>();

	// 스도쿠 seed 생성
	mSudoku->CreateSeed();

	// 스도쿠 퍼즐 생성
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

			// 현재 값이 0일 경우 텍스트를 출력하지 않음
			if (gridNum == 0)
			{
				mSudokuEditBoxGrid[i * 9 + j]->setDefaultText("");
				mSudokuEditBoxGrid[i * 9 + j]->setEnabled(true);
			}
			else // 0 이외의 값일 경우 그 숫자를 출력
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

	// 새 게임 버튼이 클릭되었으므로 자식 윈도우를 비활성화
	childWindow->setVisible(false); 
}
