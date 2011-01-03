#ifndef SCOREBOARD_H
#define SCOREBOARD_H

#include <QObject>
#include <QList>

#include "contest.h"
#include "problem.h"
#include "language.h"

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

			QString toString();

		private:
			Contest *contest;
			QList<Problem *> problems;
			QList<Language *> languages;
		};
	}
}

#endif // SCOREBOARD_H
