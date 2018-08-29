#include "LoadData.h"



LoadData::LoadData()
{

	doc.load_file("./Questions.xml");

	questions = doc.child("document");
	
}


LoadData::~LoadData()
{
}
