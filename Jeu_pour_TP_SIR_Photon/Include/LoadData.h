#pragma once

#include <pugixml.hpp>

class LoadData
{
public:
	LoadData();
	~LoadData();

	pugi::xml_node questions;

private:
	pugi::xml_document doc;


};

