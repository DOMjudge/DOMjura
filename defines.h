#ifndef DEFINES_H
#define DEFINES_H

#define PENALTY_TIME 20

#define HEADER_HEIGHT 30
#define TEAMITEM_HEIGHT 80

#define RANK_WIDTH 100
#define SOLVED_WIDTH 100
#define TIME_WIDTH 70
#define LEFT_MARGIN 10
#define RIGHT_MARGIN 10
#define NAME_WIDTH (QApplication::desktop()->screenGeometry().width() - LEFT_MARGIN - RIGHT_MARGIN - RANK_WIDTH - SOLVED_WIDTH - TIME_WIDTH)

#define LEGENDA_RIGHT_OFFSET 90
#define LEGENDA_BOTTOM_OFFSET 90

#define PROB_MARGIN 15
#define NAME_PROBS_MARGIN 0
#define PROBS_BELOW_MARGIN 5

#include <QString>
#include <QList>
#include <QApplication>
#include <QDesktopWidget>

namespace DJ {
enum ProblemState {
	NOTSUBMITTED,
	SOLVED,
	FAILED,
	PENDING_SOLVED,
	PENDING_FAILED
};

struct ResultProblem {
	ProblemState state;
	int numTries;
	int time;
	QString problemId;
};

struct ResultTeam {
	QString name;
	int rank;
	int solved;
	int time;
	QList<ResultProblem> problems;
};
}

#endif // DEFINES_H
