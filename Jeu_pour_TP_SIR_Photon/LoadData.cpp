#include "LoadData.h"



LoadData::LoadData()
{

	doc.load_file("C:/Dev/JIN4/JIN4/Questions.xml");

	questions = doc.child("document");
	questionsLitterature = doc.child("Litterature");
	questionsHistoire = doc.child("Histoire");
	questionsChant = doc.child("Chant");
	questionsMusique = doc.child("Musique");
	questionsTragedie = doc.child("Tragedie");
	questionsComedie = questions.child("Comedie");
	questionsDanse = doc.child("Danse");
	questionsRhetorique = doc.child("Rhetorque");
	questionsAstrologie = doc.child("Astrologie");

}


LoadData::~LoadData()
{
}
