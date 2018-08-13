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

}

void Game::run()
{
	// pointer on the current page
	std::shared_ptr<Menus> page;

	//timeclock
	sf::Clock clock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;

	//création unordered map
	//std::map<std::string, Menus> pageMap;
	std::unordered_map<std::string, std::shared_ptr<Menus>> pageMap;

	
	// inserting values by using [] operator
	pageMap["FirstPage"] = std::make_shared<FirstMenu>();
	
	page = pageMap["FirstPage"];
	

	while (mWindow.isOpen())
	{
		sf::Time elapsedTime = clock.restart();
		timeSinceLastUpdate += elapsedTime;
		while (timeSinceLastUpdate > TimePerFrame)
		{
			timeSinceLastUpdate -= TimePerFrame;
			processEvents(*page);
		}
		mNetworkLogic.service();
		render(*page);
	}

	mNetworkLogic.disconnect();
}

void Game::processEvents(Menus & page)
{
	sf::Event event;
	while (mWindow.pollEvent(event))
	{
		switch (event.type)
		{
		case sf::Event::MouseButtonPressed:
			page.handleClick(mWindow, event.mouseButton.x, event.mouseButton.y);
			break;

		case sf::Event::Closed:
			mWindow.close();
			break;
		}
	}
}


void Game::render(Menus & page)
{
	mWindow.clear();

	page.display(mWindow);
}

