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
			this->problemHash[problem->getId()] = this->problems.size() - 1;
		}

		int Scoreboard::getNumProblems() {
			return this->problems.size();
		}

		Problem *Scoreboard::getProblem(int i) {
			return this->problems.at(i);
		}

		Problem *Scoreboard::getProblemById(QString id) {
			if (this->problemHash.contains(id)) {
				return this->problems.at(this->problemHash[id]);
			} else {
				return NULL;
			}
		}

		void Scoreboard::addLanguage(Language *language) {
			this->languages.append(language);
			this->languageHash[language->getId()] = this->languages.size() - 1;
		}

		int Scoreboard::getNumLanguages() {
			return this->languages.size();
		}

		Language *Scoreboard::getLanguage(int i) {
			return this->languages.at(i);
		}

		Language *Scoreboard::getLanguageById(QString id) {
			if (this->languageHash.contains(id)) {
				return this->languages.at(this->languageHash[id]);
			} else {
				return NULL;
			}
		}

		void Scoreboard::addAffiliation(Affiliation *affiliation) {
			this->affiliations.append(affiliation);
			this->affiliationHash[affiliation->getId()] = this->affiliations.size() - 1;
		}

		int Scoreboard::getNumAffiliations() {
			return this->affiliations.size();
		}

		Affiliation *Scoreboard::getAffiliation(int i) {
			return this->affiliations.at(i);
		}

		Affiliation *Scoreboard::getAffiliationById(QString id) {
			if (this->affiliationHash.contains(id)) {
				return this->affiliations.at(this->affiliationHash[id]);
			} else {
				return NULL;
			}
		}

		void Scoreboard::addCategory(Category *category) {
			this->categories.append(category);
			this->categoryHash[category->getId()] = this->categories.size() - 1;
		}

		int Scoreboard::getNumCategories() {
			return this->categories.size();
		}

		Category *Scoreboard::getCategory(int i) {
			return this->categories.at(i);
		}

		Category *Scoreboard::getCategoryById(QString id) {
			if (this->categoryHash.contains(id)) {
				return this->categories.at(this->categoryHash[id]);
			} else {
				return NULL;
			}
		}

		void Scoreboard::addTeam(Team *team) {
			this->teams.append(team);
			this->teamHash[team->getId()] = this->teams.size() - 1;
		}

		void Scoreboard::updateTeamRefs() {
			for (int i = 0; i < this->teams.size(); i++) {
				this->teams.at(i)->updateIds(this);
			}
		}

		int Scoreboard::getNumTeams() {
			return this->teams.size();
		}

		Team *Scoreboard::getTeam(int i) {
			return this->teams.at(i);
		}

		Team *Scoreboard::getTeamById(QString id) {
			if (this->teamHash.contains(id)) {
				return this->teams.at(this->teamHash[id]);
			} else {
				return NULL;
			}
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
			s += "Teams (" + QString::number(this->getNumTeams()) + "):\n";
			for (int i = 0; i < this->getNumTeams(); i++) {
				s += "  Team " + QString::number(i) + ":\n";
				s += this->getTeam(i)->toString();
			}
			return s;
		}
	}
}
