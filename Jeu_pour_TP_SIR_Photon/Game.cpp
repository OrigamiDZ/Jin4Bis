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
		}
		mNetworkLogic.service();
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
				currentModeG = todo;
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
				currentThemeG = todo;
				currentPage = std::make_shared<Questions>(true, currentThemeG, data);
				break;
				
			case Menus::GoFirstMenu :
			case Menus::GoSecondMenu:
				currentPage = pageMap[todo];
				break;

			case Menus::Reponse1 :
			case Menus::Reponse2 :
			case Menus::Reponse3 :
				(*currentPage).Questions::Advance();
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

