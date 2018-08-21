#include "LoadData.h"



LoadData::LoadData()
{

	doc.load_file("C:/Dev/JIN4/JIN4/Questions.xml");

	questions = doc.child("document");
	
}


LoadData::~LoadData()
{
}
