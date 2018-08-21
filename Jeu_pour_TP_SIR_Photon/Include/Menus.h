#pragma once
#include "SFML\Window.hpp"
#include "SFML\Graphics.hpp"

class Menus
{
public:
	Menus();
	virtual ~Menus();

	enum Id {
		FirstMenu, SecondMenu,
		Game, Ending
	};
	enum Action {
		PlaySolo, PlayMulti,
		Litterature, Histoire, Chant, Musique, Tragedie, Comedie, Danse, Rhetorique, Astrologie,
		GoFirstMenu, GoSecondMenu, QuestionGame, GameEnd,
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

	virtual void Menus::Advance();
	virtual void Menus::UpdateScore(Action answer);


	Id id;
	sf::Texture fond;
	sf::Font font;

	
	std::vector<std::unique_ptr<MenuItem>> listeBoutton;
	std::vector<std::shared_ptr<sf::Text>> listeTexte;
	

};



