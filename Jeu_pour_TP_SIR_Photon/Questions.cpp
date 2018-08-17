#include "Questions.h"


Questions::Questions(std::vector<std::string> choices, Menus::Action theme, LoadData & data)
{
	score = 0;
	numTurn = 0;

	currentTheme = theme;

	font.loadFromFile("C:/Dev/JIN4/JIN4/font/DIOGENES.ttf");

	fond.loadFromFile("c:/Dev/JIN4/JIN4/images/the_muses-menu-question.png");

	//Update listeTexte
	(*questionAff).setFont(font);
	(*questionAff).setCharacterSize(20);
	(*questionAff).setFillColor(sf::Color::Black);
	(*questionAff).setStyle(sf::Text::Regular);
	(*questionAff).setPosition(158, 39);

	(*reponse1).setFont(font);
	(*reponse1).setCharacterSize(25);
	(*reponse1).setFillColor(sf::Color::Black);
	(*reponse1).setStyle(sf::Text::Regular);
	(*reponse1).setPosition(187, 241);

	(*reponse2).setFont(font);
	(*reponse2).setCharacterSize(25);
	(*reponse2).setFillColor(sf::Color::Black);
	(*reponse2).setStyle(sf::Text::Regular);
	(*reponse2).setPosition(187, 378);

	(*reponse3).setFont(font);
	(*reponse3).setCharacterSize(25);
	(*reponse3).setFillColor(sf::Color::Black);
	(*reponse3).setStyle(sf::Text::Regular);
	(*reponse3).setPosition(187, 515);

	listeTexte.push_back(std::move(questionAff));
	listeTexte.push_back(std::move(reponse1));
	listeTexte.push_back(std::move(reponse2));
	listeTexte.push_back(std::move(reponse3));

	//Update listeBoutton
	(*reponse1Button).rect.left = 112;
	(*reponse1Button).rect.top = 213;
	(*reponse1Button).rect.width = 333;
	(*reponse1Button).rect.height = 93;
	(*reponse1Button).action = Reponse1;

	(*reponse2Button).rect.left = 112;
	(*reponse2Button).rect.top = 351;
	(*reponse2Button).rect.width = 333;
	(*reponse2Button).rect.height = 93;
	(*reponse2Button).action = Reponse2;

	(*reponse3Button).rect.left = 112;
	(*reponse3Button).rect.top = 492;
	(*reponse3Button).rect.width = 333;
	(*reponse3Button).rect.height = 93;
	(*reponse3Button).action = Reponse3;

	listeBoutton.push_back(std::move(reponse1Button));
	listeBoutton.push_back(std::move(reponse2Button));
	listeBoutton.push_back(std::move(reponse3Button));

	//determination du nombre de questions possible
	//int nombreQuestionsTotalesParCatégorie = NombreQuestions(categorieString);

	//choix aléatoire des questions

	if (choices.size() == 5 && choices[0].compare("0") != 0 && choices[1].compare("0") != 0 && choices[2].compare("0") != 0 && choices[3].compare("0") != 0 && choices[4].compare("0") != 0) {
		choix = choices;
	}
	else {
		if (nombreQuestionsTotalesParCatégorie < 5) {
			for (int i = 0; i < 5; i++) {
				choix.push_back(std::to_string(rand() % nombreQuestionsTotalesParCatégorie + 1));
			}
		}
		else {
			choix.push_back(std::to_string(rand() % nombreQuestionsTotalesParCatégorie + 1));

			while (choix.size() != 5) {
				int pris = 0;
				std::string aleatoir = std::to_string(rand() % nombreQuestionsTotalesParCatégorie + 1);
				for (auto i : choix)
					if (i != aleatoir) {
						pris++;
					}
				if (pris == choix.size()) {
					choix.push_back(aleatoir);
				}
			}
		}
	}


	// GoFetch

	pugi::xml_node questions = data.questions;
	pugi::xml_node categorie = questions.child((traduction[theme]).c_str());


	// recherche des nodes des questions
	for (auto i : choix) {
		for (pugi::xml_node question = categorie.first_child(); question; question = question.next_sibling())
		{
			for (pugi::xml_attribute attr = question.first_attribute(); attr; attr = attr.next_attribute())
			{
				std::string attrName = attr.name();
				std::string attrValue = attr.value();

				if (attrName == "id" && attrValue == i)
				{
					nodes.push_back(question);
				}
			}
		}
	}
}


Questions::~Questions()
{
}

void Questions::Advance()
{
	if (numTurn < 5) {

		//récupération de la question
		for (pugi::xml_attribute attr = nodes[numTurn].first_attribute(); attr; attr = attr.next_attribute())
		{
			std::string attrName = attr.name();
			std::string attrValue = attr.value();

			if (attrName == "enonce") {
				(*listeTexte[0]).setString(attrValue);
			}
		}


		//changement de l'affichage de la question pour quelle rentre dans le cadre
		for (int i = 0; i < int((*listeTexte[0]).getString().getSize()); i++)
		{
			//Si le caractère dépasse la boundingbox définit et que le caractère n'est pas égal à un retour à la ligne
			if (((*listeTexte[0]).findCharacterPos(i).x >(155 + 246)) && ((*listeTexte[0]).getString()[i] != '\n'))
			{
				//On va chercher le dernier caractère espace afin de faire des retour à la ligne par mot et non par caractère
				while ((*listeTexte[0]).getString()[i] != ' ')--i;
				//On ajoute le caractère de retour à la ligne en remplacent l'espace entre les deux mots par \n
				sf::String str1 = (*listeTexte[0]).getString().toWideString().substr(0, i);
				str1 += "\n";
				str1 += (*listeTexte[0]).getString().toWideString().substr(i + 1);
				(*listeTexte[0]).setString(str1);
			}
		}

		//recupération réponses
		int rep = 0;
		for (pugi::xml_node reponse = nodes[numTurn].first_child(); reponse; reponse = reponse.next_sibling())
		{
			for (pugi::xml_attribute attr = reponse.first_attribute(); attr; attr = attr.next_attribute())
			{
				std::string attrName = attr.name();
				std::string attrValue = attr.value();

				if (attrName == "valeur") {
					(*ensemble[rep]).setString(attrValue);

				}
				if ((attrName == "bool") && (attrValue == "true")) {
					pointeurBonneRep = ensemble[rep];
				}
			}
			rep++;
		}

		//
		numTurn++;
	}
	else if (numTurn == 5) {


		fond.loadFromFile("c:/Dev/JIN4/JIN4/images/the_muses-menu-fin.png");

		while (!listeBoutton.empty())
		{
			listeBoutton.pop_back();
		}

		while (!listeTexte.empty())
		{
			listeTexte.pop_back();
		}
		
		//Retour menu item coordinates
		std::unique_ptr<MenuItem> AutreButton = std::make_unique<MenuItem>();

		(*AutreButton).rect.left = 93;
		(*AutreButton).rect.top = 546;
		(*AutreButton).rect.width = 356;
		(*AutreButton).rect.height = 33;
		(*AutreButton).action = GameEnd;

		listeBoutton.push_back(std::move(AutreButton));

		//affichage score
		std::unique_ptr<sf::Text> scoreAff = std::make_unique<sf::Text>();
		(*scoreAff).setFont(font);
		(*scoreAff).setCharacterSize(80);
		(*scoreAff).setFillColor(sf::Color::White);
		(*scoreAff).setStyle(sf::Text::Regular);
		(*scoreAff).setPosition(245, 272);
		(*scoreAff).setString(std::to_string(score));

		listeTexte.push_back(std::move(scoreAff));

		gameEnded = true;
	}
	else {
		this->~Questions();
	}
}

void Questions::UpdateScore(Menus::Action answer)
{
	switch (answer)
	{
	case Menus::Reponse1:
		if (ensemble[0] == pointeurBonneRep) { score++; }
		break;
	case Menus::Reponse2:
		if (ensemble[1] == pointeurBonneRep) { score++; }
		break;
	case Menus::Reponse3:
		if (ensemble[2] == pointeurBonneRep) { score++; }
		break;
	default:
		break;
	}
}


