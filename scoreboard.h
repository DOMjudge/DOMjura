#ifndef SCOREBOARD_H
#define SCOREBOARD_H

#include <QObject>
#include <QList>
#include <QHash>

#include "contest.h"
#include "problem.h"
#include "language.h"
#include "affiliation.h"
#include "category.h"
#include "team.h"

namespace DJ {
	namespace Model {
		class Scoreboard : public QObject {
			Q_OBJECT
		public:
			explicit Scoreboard(QObject *parent = 0);

			void setContest(Contest *contest);
			Contest *getContest();

			void addProblem(Problem *problem);
			int getNumProblems();
			Problem *getProblem(int i);
			Problem *getProblemById(QString id);

			void addLanguage(Language *language);
			int getNumLanguages();
			Language *getLanguage(int i);
			Language *getLanguageById(QString id);

			void addAffiliation(Affiliation *affiliation);
			int getNumAffiliations();
			Affiliation *getAffiliation(int i);
			Affiliation *getAffiliationById(QString id);

			void addCategory(Category *category);
			int getNumCategories();
			Category *getCategory(int i);
			Category *getCategoryById(QString id);

			void addTeam(Team *team);
			void updateTeamRefs();
			int getNumTeams();
			Team *getTeam(int i);
			Team *getTeamById(QString id);

			QString toString();

		private:
			Contest *contest;

			QList<Problem *> problems;
			QHash<QString, int> problemHash;

			QList<Language *> languages;
			QHash<QString, int> languageHash;

			QList<Affiliation *> affiliations;
			QHash<QString, int> affiliationHash;

			QList<Category *> categories;
			QHash<QString, int> categoryHash;

			QList<Team *> teams;
			QHash<QString, int> teamHash;
		};
	}
}

#endif // SCOREBOARD_H
