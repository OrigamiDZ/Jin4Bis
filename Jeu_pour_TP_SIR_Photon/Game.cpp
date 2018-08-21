#include <Game.hpp>

#include <StringHelpers.hpp>
#include <iostream>

#include <math.h> 

static const ExitGames::Common::JString appID = L"774e251d-8a12-444d-88c0-96130444aba0"; // set your app id here
static const ExitGames::Common::JString appVersion = L"1.0";
static ExitGames::Common::JString gameName = L"TP_SIR";

const sf::Time Game::TimePerFrame = sf::seconds(1.f / 60.f);
const int Game::Width = 562;
const int Game::Height = 700;
const float StartOffset = 50.f;

Game::Game()
	: mWindow(sf::VideoMode(Game::Width, Game::Height), "Les 1001 Muses", sf::Style::Close)
	, mNetworkLogic(appID, appVersion, this)
{
	mNetworkLogic.connect();
	mNetworkLogic.joinOrCreateRoom(gameName);
	mPlayerNumber = mNetworkLogic.getNumber();

	pageMap[Menus::GoFirstMenu] = std::make_shared<FirstMenu>();
	pageMap[Menus::GoSecondMenu] = std::make_shared<SecondMenu>();

	for (int i = 0; i < 5; i++) {
		vectorOfZeros.push_back("0");
	}

	font.loadFromFile("C:/Dev/JIN4/JIN4/font/DIOGENES.ttf");
	winOrLose = std::make_shared<sf::Text>();
	(*winOrLose).setFont(font);
	(*winOrLose).setCharacterSize(40);
	(*winOrLose).setFillColor(sf::Color::White);
	(*winOrLose).setStyle(sf::Text::Regular);
	(*winOrLose).setPosition(225, 380);

}

void Game::run()
{
	//timeclock
	sf::Clock clock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;
	
	// pointer on the current page
	currentPage = pageMap[Menus::GoFirstMenu];
	
	while (mWindow.isOpen())
	{
		sf::Time elapsedTime = clock.restart();
		timeSinceLastUpdate += elapsedTime;
		while (timeSinceLastUpdate > TimePerFrame)
		{
			timeSinceLastUpdate -= TimePerFrame;
			processEvents();
			
			if (gameEnd == true && opponentScore >= 0) {
				if (currentScore >= opponentScore) {
					//affichage score
					winOrLose->setString("Gagné");
				}
				else {
					//affichage score
					winOrLose->setString("Perdu");
				}
				currentPage->listeTexte.push_back(std::move(winOrLose));
				opponentScore = -1;
				gameEnd = false;
			}
			mNetworkLogic.service();
		}
		render();
	}

	mNetworkLogic.disconnect();
}

void Game::processEvents()
{
	sf::Event event;
	while (mWindow.pollEvent(event))
	{
		switch (event.type)
		{
		case sf::Event::MouseButtonPressed: {
			Menus::Action todo = (*currentPage).handleClick(mWindow, event.mouseButton.x, event.mouseButton.y);

			switch (todo) {

			case Menus::PlaySolo :
			case Menus::PlayMulti :
				currentMode = todo;
				mNetworkLogic.sendPlayerChange(todo);
				currentPage = pageMap[Menus::GoSecondMenu];
				break;

			case Menus::Litterature :
			case Menus::Histoire :
			case Menus::Chant :
			case Menus::Musique :
			case Menus::Tragedie :
			case Menus::Comedie: 
			case Menus::Danse : 
			case Menus::Rhetorique :
			case Menus::Astrologie :
				currentTheme = todo;
				mNetworkLogic.sendPlayerChange(todo);
				if (currentMode == Menus::PlayMulti && opponentMode == Menus::PlayMulti && currentTheme == opponentTheme) {
					pageMap[Menus::QuestionGame] = std::make_shared<Questions>(vectorOfZeros, currentTheme, data);
					currentPage = pageMap[Menus::QuestionGame];
					for (auto i : (dynamic_cast<Questions&>(*currentPage)).choix) {
						mNetworkLogic.sendPlayerChoice(i);
					}
					dynamic_cast<Questions&>(*currentPage).Advance();
				}
				else if (currentMode == Menus::PlaySolo) {
					pageMap[Menus::QuestionGame] = std::make_shared<Questions>(vectorOfZeros, currentTheme, data);
					currentPage = pageMap[Menus::QuestionGame];
					dynamic_cast<Questions&>(*currentPage).Advance();
				}
				break;
			case Menus::GoFirstMenu :
			case Menus::GoSecondMenu:
				currentScore = -1;
				opponentScore = -1;
				currentPage = pageMap[todo];
				break;

			case Menus::Reponse1 :
			case Menus::Reponse2 :
			case Menus::Reponse3: 
				dynamic_cast<Questions&>(*currentPage).UpdateScore(todo);
				currentScore = (dynamic_cast<Questions&>(*currentPage)).score;
				dynamic_cast<Questions&>(*currentPage).Advance();
				if (dynamic_cast<Questions&>(*currentPage).gameEnded == true) {
					gameEnd = true;
					mNetworkLogic.sendPlayerScore(currentScore);
				}
				break;

			case Menus::GameEnd :
				restart();
				currentPage = pageMap[Menus::GoFirstMenu];
				break;

			case Menus::Quitter :
				mWindow.close();
				break;
			}
			break;
		}

		case sf::Event::Closed:
			mWindow.close();
			break;
		}
	}
}


void Game::render()
{
	mWindow.clear();

	(*currentPage).display(mWindow);
}

void Game::restart()
{
	currentMode = Menus::Vide;
	currentTheme = Menus::Vide;
	currentScore = -1;

	gameEnd = false;

	mNetworkLogic.sendPlayerChange(Menus::Vide);
	opponentScore = -1;

	while (!vectorChoice.empty())
	{
		vectorChoice.pop_back();
	}

	winOrLose = std::make_shared<sf::Text>();
	(*winOrLose).setFont(font);
	(*winOrLose).setCharacterSize(40);
	(*winOrLose).setFillColor(sf::Color::White);
	(*winOrLose).setStyle(sf::Text::Regular);
	(*winOrLose).setPosition(225, 380);
	(*winOrLose).setPosition(225, 380);
}

