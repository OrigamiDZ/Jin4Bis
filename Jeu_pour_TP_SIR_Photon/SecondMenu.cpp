#include "SecondMenu.h"



SecondMenu::SecondMenu()
{

	fond.loadFromFile("c:/Dev/JIN4/JIN4/images/the_muses-menu-second.png");

	//Litterature menu item coordinates
	(*litteratureButton).rect.left = 158;
	(*litteratureButton).rect.top = 76;
	(*litteratureButton).rect.width = 243;
	(*litteratureButton).rect.height = 33;
	(*litteratureButton).action = Litterature;

	//Histoire menu item coordinates
	(*histoireButton).rect.left = 189;
	(*histoireButton).rect.top = 136;
	(*histoireButton).rect.width = 184;
	(*histoireButton).rect.height = 33;
	(*histoireButton).action = Histoire;

	//Chant menu item coordinates
	(*chantButton).rect.left = 206;
	(*chantButton).rect.top = 196;
	(*chantButton).rect.width = 140;
	(*chantButton).rect.height = 33;
	(*chantButton).action = Chant;

	//Musique menu item coordinates
	(*musiqueButton).rect.left = 188;
	(*musiqueButton).rect.top = 258;
	(*musiqueButton).rect.width = 179;
	(*musiqueButton).rect.height = 36;
	(*musiqueButton).action = Musique;

	//Tragedie menu item coordinates
	(*tragedieButton).rect.left = 179;
	(*tragedieButton).rect.top = 317;
	(*tragedieButton).rect.width = 193;
	(*tragedieButton).rect.height = 33;
	(*tragedieButton).action = Tragedie;

	//Comedie menu item coordinates
	(*comedieButton).rect.left = 185;
	(*comedieButton).rect.top = 376;
	(*comedieButton).rect.width = 183;
	(*comedieButton).rect.height = 33;
	(*comedieButton).action = Comedie;

	//Danse menu item coordinates
	(*danseButton).rect.left = 212;
	(*danseButton).rect.top = 436;
	(*danseButton).rect.width = 127;
	(*danseButton).rect.height = 33;
	(*danseButton).action = Danse;

	//Rhetorique menu item coordinates
	(*rhetoriqueButton).rect.left = 149;
	(*rhetoriqueButton).rect.top = 496;
	(*rhetoriqueButton).rect.width = 254;
	(*rhetoriqueButton).rect.height = 38;
	(*rhetoriqueButton).action = Rhetorique;

	//Astrologie menu item coordinates
	(*astrologieButton).rect.left = 150;
	(*astrologieButton).rect.top = 555;
	(*astrologieButton).rect.width = 248;
	(*astrologieButton).rect.height = 33;
	(*astrologieButton).action = Astrologie;

	//Back menu item coordinates
	(*backButton).rect.left = 222;
	(*backButton).rect.top = 618;
	(*backButton).rect.width = 120;
	(*backButton).rect.height = 26;
	(*backButton).action = GoFirstMenu;

	listeBoutton.push_back(std::move(litteratureButton));
	listeBoutton.push_back(std::move(histoireButton));
	listeBoutton.push_back(std::move(chantButton));
	listeBoutton.push_back(std::move(musiqueButton));
	listeBoutton.push_back(std::move(tragedieButton));
	listeBoutton.push_back(std::move(comedieButton));
	listeBoutton.push_back(std::move(danseButton));
	listeBoutton.push_back(std::move(rhetoriqueButton));
	listeBoutton.push_back(std::move(astrologieButton));
	listeBoutton.push_back(std::move(backButton));
}


SecondMenu::~SecondMenu()
{
}
