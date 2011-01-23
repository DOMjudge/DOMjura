#ifndef DEFINES_H
#define DEFINES_H

#define PENALTY_TIME QSettings().value("penaltyTime", 20).toInt()

#define HEADER_HEIGHT QSettings().value("headerHeight", 30).toInt()
#define TEAMITEM_HEIGHT QSettings().value("teamItemHeight", 80).toInt()

#define SCROLL_BELOW_OFFSET QSettings().value("scrollBelowOffset", 40).toInt()
#define RESOLV_BELOW_OFFSET QSettings().value("resolvBelowOffset", 80).toInt()

#define WINNER_TEXT QSettings().value("winnerText", "Winner programming contest").toString()
#define BRANDING_IMAGE QSettings().value("brandingImage", "").toString()

// We need to click to resolve the NEED_TO_CLICK top rows
// The other rows resolve automatically
#define NEED_TO_CLICK QSettings().value("needToClick", 10).toInt()

#define GOLD QSettings().value("gold", 1).toInt()
#define SILVER QSettings().value("silver", 2).toInt()
#define BRONZE QSettings().value("bronze", 3).toInt()

#define RANK_WIDTH QSettings().value("rankWidth", 100).toInt()
#define SOLVED_WIDTH QSettings().value("solvedWidth", 100).toInt()
#define TIME_WIDTH QSettings().value("timeWidth", 70).toInt()
#define LEFT_MARGIN QSettings().value("leftMargin", 10).toInt()
#define RIGHT_MARGIN QSettings().value("rightMargin", 10).toInt()
#define NAME_WIDTH (QApplication::desktop()->screenGeometry().width() - LEFT_MARGIN - RIGHT_MARGIN - RANK_WIDTH - SOLVED_WIDTH - TIME_WIDTH)

#define LEGENDA_RIGHT_OFFSET QSettings().value("legendaRightOffset", 90).toInt()
#define LEGENDA_BOTTOM_OFFSET QSettings().value("legendaBottomOffset", 90).toInt()

#define PROB_MARGIN QSettings().value("probMargin", 15).toInt()
#define NAME_PROBS_MARGIN QSettings().value("nameProbsMargin", 0).toInt()
#define PROBS_BELOW_MARGIN QSettings().value("probsBelowMargin", 5).toInt()

#include <QString>
#include <QList>
#include <QApplication>
#include <QDesktopWidget>
#include <QSettings>

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
	QString id;
	int rank;
	int solved;
	int time;
	QList<ResultProblem> problems;
};
}

#endif // DEFINES_H
