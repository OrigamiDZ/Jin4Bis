#pragma once

#include <pugixml.hpp>

class LoadData
{
public:
	LoadData();
	~LoadData();

	pugi::xml_document doc;
	pugi::xml_node questions;

	pugi::xml_node questionsLitterature;
	pugi::xml_node questionsHistoire;
	pugi::xml_node questionsChant;
	pugi::xml_node questionsMusique;
	pugi::xml_node questionsTragedie;
	pugi::xml_node questionsComedie;
	pugi::xml_node questionsDanse;
	pugi::xml_node questionsRhetorique;
	pugi::xml_node questionsAstrologie;

};

