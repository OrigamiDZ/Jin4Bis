#include "LoadData.h"



LoadData::LoadData()
{

	doc.load_file("C:/Dev/JIN4/JIN4/Questions.xml");

	questions = doc.child("document");
	
	/* 
	questionsLitterature = questions.child("Litterature");
	questionsHistoire = questions.child("Histoire");
	questionsChant = questions.child("Chant");
	questionsMusique = questions.child("Musique");
	questionsTragedie = questions.child("Tragedie");
	questionsComedie = questions.child("Comedie");
	questionsDanse = questions.child("Danse");
	questionsRhetorique = questions.child("Rhetorque");
	questionsAstrologie = questions.child("Astrologie");
	*/

}


LoadData::~LoadData()
{
}
