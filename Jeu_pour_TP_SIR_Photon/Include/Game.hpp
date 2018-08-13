#ifndef GAME_HPP
#define GAME_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include <map>
#include <unordered_map>
#include <memory>

#include <ResourceIdentifiers.hpp>
#include <NetworkLogic.hpp>

#include "Menus.h"
#include "FirstMenu.h"


class Game : private sf::NonCopyable
{
	public:
								Game();
		void					run();

		enum Action { Rate, Reponse1, Reponse2, Reponse3, Quitter };
		struct MenuItem
		{
		public:
			sf::Rect<int> rect;
			Action action;
		};

	public:
		static const int		Width;
		static const int		Height;

	private:
		void					processEvents(Menus & page);
		void					render();		

	private:
		static const sf::Time	TimePerFrame;

		sf::RenderWindow		mWindow;

		int						mPlayerNumber;

		NetworkLogic            mNetworkLogic;
};

#endif // GAME_HPP
