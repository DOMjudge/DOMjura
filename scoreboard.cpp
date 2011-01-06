#include "scoreboard.h"

namespace DJ {
	namespace Model {
		Scoreboard::Scoreboard(QObject *parent) : QObject(parent) {
			this->contest = NULL;
		}

		void Scoreboard::setContest(Contest *contest) {
			this->contest = contest;
		}

		Contest *Scoreboard::getContest() {
			return this->contest;
		}

		void Scoreboard::addProblem(Problem *problem) {
			this->problems.append(problem);
		}

		int Scoreboard::getNumProblems() {
			return this->problems.size();
		}

		Problem *Scoreboard::getProblem(int i) {
			return this->problems.at(i);
		}

		void Scoreboard::addLanguage(Language *language) {
			this->languages.append(language);
		}

		int Scoreboard::getNumLanguages() {
			return this->languages.size();
		}

		Language *Scoreboard::getLanguage(int i) {
			return this->languages.at(i);
		}

		void Scoreboard::addAffiliation(Affiliation *affiliation) {
			this->affiliations.append(affiliation);
		}

		int Scoreboard::getNumAffiliations() {
			return this->affiliations.size();
		}

		Affiliation *Scoreboard::getAffiliation(int i) {
			return this->affiliations.at(i);
		}

		void Scoreboard::addCategory(Category *category) {
			this->categories.append(category);
		}

		int Scoreboard::getNumCategories() {
			return this->categories.size();
		}

		Category *Scoreboard::getCategory(int i) {
			return this->categories.at(i);
		}

		QString Scoreboard::toString() {
			QString s;
			s += "-- Scoreboard --\n";
			s += "Contest:\n";
			s += this->contest->toString();
			s += "Problems (" + QString::number(this->getNumProblems()) + "):\n";
			for (int i = 0; i < this->getNumProblems(); i++) {
				s += "  Problem " + QString::number(i) + ":\n";
				s += this->getProblem(i)->toString();
			}
			s += "Languages (" + QString::number(this->getNumLanguages()) + "):\n";
			for (int i = 0; i < this->getNumLanguages(); i++) {
				s += "  Language " + QString::number(i) + ":\n";
				s += this->getLanguage(i)->toString();
			}
			s += "Affiliations (" + QString::number(this->getNumAffiliations()) + "):\n";
			for (int i = 0; i < this->getNumAffiliations(); i++) {
				s += "  Affiliation " + QString::number(i) + ":\n";
				s += this->getAffiliation(i)->toString();
			}
			s += "Categories (" + QString::number(this->getNumCategories()) + "):\n";
			for (int i = 0; i < this->getNumCategories(); i++) {
				s += "  Category " + QString::number(i) + ":\n";
				s += this->getCategory(i)->toString();
			}
			return s;
		}
	}
}
