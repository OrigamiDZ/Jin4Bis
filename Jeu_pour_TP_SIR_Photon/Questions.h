#pragma once
#include "Menus.h"
#include "LoadData.h"
#include "SFML\Window.hpp"
#include "SFML\Graphics.hpp"
#include <list>
#include <pugixml.hpp>
#include "unordered_map"

class Questions : public Menus
{
public:
	Questions(bool solo, Menus::Action theme, LoadData & data);
	~Questions();

	void Questions::Advance();
	void Questions::UpdateScore(Menus::Action answer);

	int nombreQuestionsTotalesParCatégorie = 5; //cheating because voilà

	std::vector<std::string> choix; //vector of the numbers of the questions chosen

	std::vector<pugi::xml_node> nodes; // vector of the nodes, each node composed of a question and of its possible answers

	int					numTurn;
	int					score = 0;

	bool				playingSolo;

	std::shared_ptr<sf::Text> pointeurBonneRep = std::make_shared<sf::Text>();

	std::unique_ptr<sf::Text> questionAff = std::make_unique<sf::Text>();

	std::vector<std::shared_ptr<sf::Text>> ensemble = { reponse1, reponse2, reponse3 };

	std::shared_ptr<sf::Text> reponse1 = std::make_shared<sf::Text>();
	std::shared_ptr<sf::Text> reponse2 = std::make_shared<sf::Text>();
	std::shared_ptr<sf::Text> reponse3 = std::make_shared<sf::Text>();

	std::unique_ptr<MenuItem> reponse1Button = std::make_unique<MenuItem>();
	std::unique_ptr<MenuItem> reponse2Button = std::make_unique<MenuItem>();
	std::unique_ptr<MenuItem> reponse3Button = std::make_unique<MenuItem>();


private:
	std::unordered_map<Menus::Action, std::string>	traduction = {
	{ Menus::Astrologie, "Astrologie" },
	{ Menus::Chant, "Chant" },
	{ Menus::Comedie, "Comedie" },
	{ Menus::Danse, "Danse" },
	{ Menus::Rhetorique, "Rhetorique" },
	{ Menus::Tragedie, "Tragedie" },
	{ Menus::Histoire, "Histoire" } };;

};

