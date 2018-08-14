#pragma once
#include "Menus.h"
#include "LoadData.h"

class Questions : public Menus
{
public:
	Questions(bool solo, Menus::Action mode, LoadData & data);
	~Questions();
};

