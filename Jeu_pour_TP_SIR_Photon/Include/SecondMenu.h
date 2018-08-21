#pragma once
#include "Menus.h"

class SecondMenu : public Menus
{
public:
	SecondMenu();
	~SecondMenu();

private:
	std::unique_ptr<MenuItem> litteratureButton = std::make_unique<MenuItem>();
	std::unique_ptr<MenuItem> histoireButton = std::make_unique<MenuItem>();
	std::unique_ptr<MenuItem> chantButton = std::make_unique<MenuItem>();
	std::unique_ptr<MenuItem> musiqueButton = std::make_unique<MenuItem>();
	std::unique_ptr<MenuItem> tragedieButton = std::make_unique<MenuItem>();
	std::unique_ptr<MenuItem> comedieButton = std::make_unique<MenuItem>();
	std::unique_ptr<MenuItem> danseButton = std::make_unique<MenuItem>();
	std::unique_ptr<MenuItem> rhetoriqueButton = std::make_unique<MenuItem>();
	std::unique_ptr<MenuItem> astrologieButton = std::make_unique<MenuItem>();
	std::unique_ptr<MenuItem> backButton = std::make_unique<MenuItem>();

};

