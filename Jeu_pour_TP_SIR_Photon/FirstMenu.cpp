#include "FirstMenu.h"

FirstMenu::FirstMenu()
{
	fond.loadFromFile("./images/the_muses-menu.png");

	//FirstMenu menu item coordinates
	playSoloButton->rect.left = 190;
	playSoloButton->rect.top = 167;
	playSoloButton->rect.width = 178;
	playSoloButton->rect.height = 56;
	playSoloButton->action = PlaySolo;

	//FirstMenu menu item coordinates
	playMultiButton->rect.left = 80;
	playMultiButton->rect.top = 345;
	playMultiButton->rect.width = 404;
	playMultiButton->rect.height = 51;
	playMultiButton->action = PlayMulti;

	//Exit menu item coordinates
	exitButton->rect.left = 162;
	exitButton->rect.top = 520;
	exitButton->rect.width = 231;
	exitButton->rect.height = 51;
	exitButton->action = Quitter;
	
	listeBoutton.push_back(std::move(playSoloButton));
	listeBoutton.push_back(std::move(playMultiButton));
	listeBoutton.push_back(std::move(exitButton));

}


FirstMenu::~FirstMenu()
{
}