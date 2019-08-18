#include "Game.h"

Game::Game()
	: mDeltaTime(0.f)
	, mRadius(5.f)
	, mState(nullptr)
	, mGameMenuState(new GameMenuState())
	, mGameOverState(new GameOverState())
	, mGamePlayState(new GamePlayState())
{
	// 메모리 공간 할당
	mSnake.reserve(100);
	mFood.reserve(100);
	mPoison.reserve(100);
}

Game::~Game()
{
	delete mGameMenuState;
	delete mGamePlayState;
	delete mGameOverState;


	// mSnake 메모리 해제
	for (auto iter = mSnake.begin(); iter != mSnake.end(); iter++)  
	{
		delete *iter;
	}

	// mFood 메모리 해제
	for (auto iter = mFood.begin(); iter != mFood.end(); iter++) 
	{
		if (*iter != nullptr)
		{
			delete *iter;
		}
	}

	// mPoison 메모리 해제
	for (auto iter = mPoison.begin(); iter != mPoison.end(); iter++) 
	{
		if (*iter != nullptr)
		{
			delete *iter;
		}
	}
}

bool Game::Init()
{
	// 윈도우 생성
	mWindow.create(sf::VideoMode(GAME_WIDTH, GAME_HEIGHT), "Snake", sf::Style::Titlebar | sf::Style::Close);

	// 60프레임 고정
	mWindow.setFramerateLimit(60); 

	// 게임 메뉴 상태
	mState = mGameMenuState;

	// 폰트 가져오기
	if (!mFont.loadFromFile("resources/sansation.ttf"))
	{
		return false;
	}

	// mPauseMessage 초기화
	mPauseMessage.setFont(mFont);
	mPauseMessage.setCharacterSize(40);
	mPauseMessage.setPosition(170.f, 150.f);
	mPauseMessage.setFillColor(sf::Color::White);

	// 사운드 가져오기
	if (!mEatingSoundBuffer.loadFromFile("resources/eatingFood.wav"))
	{
		return false;
	}
	// mEatingSound 초기화
	mEatingSound.setBuffer(mEatingSoundBuffer);

	// Snake 초기화
	Circle* head = new Circle(mRadius);
	head->setFillColor(sf::Color(145, 2, 247));
	head->setPosition(static_cast<float>(GAME_WIDTH) / 2.f, static_cast<float>(GAME_HEIGHT) / 2.f);

	Circle* body1 = new Circle(mRadius);
	body1->setFillColor(sf::Color(0, 255, 0));
	body1->setPosition(head->getPosition().x, head->getPosition().y + 2 * mRadius);

	Circle* body2 = new Circle(mRadius);
	body2->setFillColor(sf::Color(0, 255, 0));
	body2->setPosition(body1->getPosition().x, body1->getPosition().y + 2 * mRadius);

	mSnake.push_back(head);
	mSnake.push_back(body1);
	mSnake.push_back(body2);

	
	return true;
}
void Game::Run()
{
	// seed값 설정
	srand(static_cast<unsigned int>(time(0)));

	while (mWindow.isOpen())
	{
		sf::Event event;

		while (mWindow.pollEvent(event))
		{
			// Escape 키 또는 윈도우 닫기가 입력되었을 때 게임 종료
			if ((event.type == sf::Event::Closed) || ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Escape)))
			{
				mWindow.close();
				break;
			}
		}

		// 게임 오버된 상태에서 게임패드 A버튼이 입력되었을 때 게임 재시작
		if (mState == mGameOverState && sf::Joystick::isButtonPressed(0, 0))
		{
			// 게임 상태를 게임오버에서 게임플레이로 전환
			mState = mGamePlayState;

			// Snake 다시 만들기
			Circle* head = new Circle(mRadius);
			head->setFillColor(sf::Color(145, 2, 247));
			head->setPosition(static_cast<float>(GAME_WIDTH) / 2.f, static_cast<float>(GAME_HEIGHT) / 2.f);

			Circle* body1 = new Circle(mRadius);
			body1->setFillColor(sf::Color(0, 255, 0));
			body1->setPosition(head->getPosition().x, head->getPosition().y + 2 * mRadius);

			Circle* body2 = new Circle(mRadius);
			body2->setFillColor(sf::Color(0, 255, 0));
			body2->setPosition(body1->getPosition().x, body1->getPosition().y + 2 * mRadius);

			mSnake.push_back(head);
			mSnake.push_back(body1);
			mSnake.push_back(body2);
		}

		mState->Run(*this);
	}
}

void Game::MoveSnake(float x, float y)
{
	int speed = 4;
	int size = static_cast<int>(mSnake.size());

	for (int i = 0; i < size; i++)
	{
		// 머리 움직임
		if (i == 0)
		{
			if (mSnake[i]->getPosition().x < 0 && x <= 0)
			{
				mSnake[i]->move(0, y * mDeltaTime * speed);
			}
			else if (mSnake[i]->getPosition().x + 2 * mSnake[i]->getRadius() > GAME_WIDTH && x >= 0)
			{
				mSnake[i]->move(0, y * mDeltaTime * speed);
			}
			else if (mSnake[i]->getPosition().y < 0 && y <= 0)
			{
				mSnake[i]->move(x * mDeltaTime * speed, 0);
			}
			else if (mSnake[i]->getPosition().y + 2 * mSnake[i]->getRadius() > GAME_HEIGHT && y >= 0)
			{
				mSnake[i]->move(x * mDeltaTime * speed, 0);
			}
			else
			{
				mSnake[i]->move(x * mDeltaTime * speed, y * mDeltaTime * speed);
			}
		}
		else  // 몸체 움직임
		{
			// (x - 2rcos@, y + 2rsin@)
			float x = mSnake[i - 1]->getPosition().x;
			float y = mSnake[i - 1]->getPosition().y;
			float r = mRadius;
			float cos = GetCOS(mSnake[i - 1]->getPosition(), mSnake[i]->getPosition());
			float sin = GetSIN(mSnake[i - 1]->getPosition(), mSnake[i]->getPosition());

			mSnake[i]->setPosition(x - 2 * r * cos, y + 2 * r * sin);
		}
	}
}

float Game::GetDistance(sf::Vector2f point1, sf::Vector2f point2)
{
	// 피타고라스의 정리 사용
	return sqrt(pow(point1.x - point2.x, 2) + pow(point1.y - point2.y, 2)); 
}

float Game::GetCOS(sf::Vector2f point1, sf::Vector2f point2)
{
	return (point1.x - point2.x) / GetDistance(point1, point2);
}

float Game::GetSIN(sf::Vector2f point1, sf::Vector2f point2)
{
	return (point2.y - point1.y) / GetDistance(point1, point2);
}

void Game::DetectFoodCollision()
{
	for (size_t i = 0; i < mFood.size(); i++)
	{
		if (mFood[i] != nullptr)
		{
			if (GetDistance(mFood[i]->GetCenterPosition(), mSnake[0]->GetCenterPosition()) <= 2 * mRadius)
			{
				// 먹이를 먹었을 때 소리 내기
				mEatingSound.play();

				// 해당 먹이를 삭제
				delete mFood[i];
				mFood[i] = nullptr;

				// 뱀 몸체에 원 하나 추가
				Circle* body = new Circle(mRadius);
				body->setFillColor(sf::Color(0, 255, 0));
				body->setPosition(-10, -10);
				mSnake.push_back(body);
				break;
			}
		}
	}
}

void Game::DetectPoisonCollision()
{
	for (size_t i = 0; i < mPoison.size(); i++)
	{
		if (mPoison[i] != nullptr)
		{
			if (GetDistance(mPoison[i]->GetCenterPosition(), mSnake[0]->GetCenterPosition()) <= 2 * mRadius)
			{
				delete mPoison[i];
				mPoison[i] = nullptr;

				// 게임 오버
				mState = mGameOverState;  

				// 게임 오버되었을 때 게임패드 신호를 false로 전환
				mGamePlayState->SetIsJoystickSignalDetected(false); 

				break;
			}
		}
	}
}