#pragma once
#include "Menus.h"

class FirstMenu : public Menus
{
public:
	FirstMenu();
	~FirstMenu();

private:
	std::unique_ptr<MenuItem> playSoloButton = std::make_unique<MenuItem>();
	std::unique_ptr<MenuItem> playMultiButton = std::make_unique<MenuItem>();
	std::unique_ptr<MenuItem> exitButton = std::make_unique<MenuItem>();
};
