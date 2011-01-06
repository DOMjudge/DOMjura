#ifndef SCOREBOARD_H
#define SCOREBOARD_H

#include <QObject>
#include <QList>

#include "contest.h"
#include "problem.h"
#include "language.h"
#include "affiliation.h"
#include "category.h"

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
			void addLanguage(Language *language);
			int getNumLanguages();
			Language *getLanguage(int i);
			void addAffiliation(Affiliation *affiliation);
			int getNumAffiliations();
			Affiliation *getAffiliation(int i);
			void addCategory(Category *category);
			int getNumCategories();
			Category *getCategory(int i);

			QString toString();

		private:
			Contest *contest;
			QList<Problem *> problems;
			QList<Language *> languages;
			QList<Affiliation *> affiliations;
			QList<Category *> categories;
		};
	}
}

#endif // SCOREBOARD_H
