#pragma once
#include "SFML\Window.hpp"
#include "SFML\Graphics.hpp"
#include <list>
//#include <pugixml.hpp>
#include "NetworkLogic.hpp"

class Menus
{
public:
	Menus();
	~Menus();

	enum Id {
		FirstMenu, SecondMenu,
		Game, Ending
	};
	enum Action {
		PlaySolo, PlayMulti,
		Litterature, Histoire, Chant, Musique, Tragedie, Comedie, Danse, Rhetorique, Astrologie,
		GoFirstMenu, GoSecondMenu,
		Reponse1, Reponse2, Reponse3,
		Quitter,
		Vide
	};

	struct MenuItem
	{
	public:
		sf::Rect<int> rect;
		Action action;
	};

	void Menus::display(sf::RenderWindow & renderWindow);
	Action Menus::handleClick(sf::RenderWindow & renderWindow, int xMouse, int yMouse);

	Id id;
	sf::Texture fond;
	
	std::vector<std::unique_ptr<MenuItem>> listeBoutton;
	std::vector<std::unique_ptr<sf::Text>> listeTexte;
	
	Action currentModeM = Vide;
	Action currentThemeM = Vide;

};



