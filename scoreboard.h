/** \file scoreboard.h
  * \brief Contains the scoreboard class.
  */

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
/** The scoreboard.
  */
class Scoreboard : public QObject {
	Q_OBJECT
public:
	/** Constructs a new scoreboard.
	  */
	explicit Scoreboard(QObject *parent = 0);
	/** Sets the contest.
	  * \param contest The contest to set.
	  */
	void setContest(Contest *contest);
	/** Returns the contest.
	  * \return The contest.
	  */
	Contest *getContest();

	/** Adds a new problem.
	  * \param problem The problem to add.
	  */
	void addProblem(Problem *problem);
	/** Returns the number of problems.
	  * \return The number of problems.
	  */
	int getNumProblems();
	/** Returns the i-th problem.
	  * \param i The index to use.
	  * \return The i-th problem.
	  */
	Problem *getProblem(int i);
	/** Returns the problem with the given ID.
	  * \param id The ID to search for.
	  * \return The problem with the given ID, or NULL if not found.
	  */
	Problem *getProblemById(QString id);

	/** Adds a new language.
	  * \param language The language to add.
	  */
	void addLanguage(Language *language);
	/** Returns the number of langauges.
	  * \return The number of langauges.
	  */
	int getNumLanguages();
	/** Returns the i-th langauge.
	  * \param i The index to use.
	  * \return The i-th language.
	  */
	Language *getLanguage(int i);
	/** Returns the language with the given ID.
	  * \param id The ID to search for.
	  * \return The language with the given ID, or NULL if not found.
	  */
	Language *getLanguageById(QString id);

	/** Adds a new affiliation.
	  * \param affiliation The affiliation to add.
	  */
	void addAffiliation(Affiliation *affiliation);
	/** Returns the number of affiliations.
	  * \return The number of affiliations.
	  */
	int getNumAffiliations();
	/** Returns the i-th affiliation.
	  * \param i The index to use.
	  * \return The i-th affiliation.
	  */
	Affiliation *getAffiliation(int i);
	/** Returns the affiliation with the given ID.
	  * \param id The ID to search for.
	  * \return The affiliation with the given ID, or NULL if not found.
	  */
	Affiliation *getAffiliationById(QString id);

	/** Adds a new category.
	  * \param category The category to add.
	  */
	void addCategory(Category *category);
	/** Returns the number of categories.
	  * \return The number of categories.
	  */
	int getNumCategories();
	/** Returns the i-th cagetory.
	  * \param i The index to use.
	  * \return The i-th cagetory.
	  */
	Category *getCategory(int i);
	/** Returns the category with the given ID.
	  * \param id The ID to search for.
	  * \return The category with the given ID, or NULL if not found.
	  */
	Category *getCategoryById(QString id);

	/** Adds a new team.
	  * \param team The team to add.
	  */
	void addTeam(Team *team);
	/** Update the team references (i.e. category and affiliation).
	  */
	void updateTeamRefs();
	/** Returns the number of teams.
	  * \return The number of teams.
	  */
	int getNumTeams();
	/** Returns the i-th team.
	  * \param i The index to use.
	  * \return The i-th team.
	  */
	Team *getTeam(int i);
	/** Returns the team with the given ID.
	  * \param id The ID to search for.
	  * \return The team with the given ID, or NULL if not found.
	  */
	Team *getTeamById(QString id);

	/** Returns a string representing this scoreboard.
	  * \return A string representation of this scoreboard.
	  * Useful for debug printing.
	  */
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
