/** \file defines.h
  * \brief Global definitions and structs
  */

#ifndef DEFINES_H
#define DEFINES_H

/** Use OpenGL for the rendering or not.
  */
#define USE_OPENGL QSettings().value("useOpenGL", true).toBool()

/** Defines how high the header item is.
  */
#define HEADER_HEIGHT QSettings().value("headerHeight", 30).toInt()
/** Defines how high each team item is.
  */
#define TEAMITEM_HEIGHT QSettings().value("teamItemHeight", 80).toInt()

/** Defines how many pixels to leave below the last team when scrolling down.
  */
#define SCROLL_BELOW_OFFSET QSettings().value("scrollBelowOffset", 40).toInt()
/** Defines how many pixels to leave below the team that is resolved to currently.
  */
#define RESOLV_BELOW_OFFSET QSettings().value("resolvBelowOffset", 80).toInt()
/** Defines the filename of the branding image.
  */
#define BRANDING_IMAGE QSettings().value("brandingImage", "").toString()

/** Defines until which row resolving will be done automatically. For the
  * top NEED_TO_CLICK rows, the user needs to click to resolve
  */
#define NEED_TO_CLICK QSettings().value("needToClick", 10).toInt()

/** Every team in the places 1 -- GOLD will earn a gold medal.
  */
#define GOLD QSettings().value("gold", 1).toInt()
/** Every team in the places GOLD+1 -- SILVER will earn a silver medal.
  */
#define SILVER QSettings().value("silver", 2).toInt()
/** Every team in the places SILVER+1 -- BRONZE will earn a bronse medal.
  */
#define BRONZE QSettings().value("bronze", 3).toInt()

/** How wide the rank colum is.
  */
#define RANK_WIDTH QSettings().value("rankWidth", 100).toInt()
/** How wide the solved column is.
  */
#define SOLVED_WIDTH QSettings().value("solvedWidth", 100).toInt()
/** How wide the time column is.
  */
#define TIME_WIDTH QSettings().value("timeWidth", 70).toInt()
/** How many margin there is to the left.
  */
#define LEFT_MARGIN QSettings().value("leftMargin", 10).toInt()
/** How many margin there is to the right.
  */
#define RIGHT_MARGIN QSettings().value("rightMargin", 10).toInt()
/** Helper define to compute the width of the name column.
  */
#define NAME_WIDTH (QApplication::desktop()->screenGeometry().width() - LEFT_MARGIN - RIGHT_MARGIN - RANK_WIDTH - SOLVED_WIDTH - TIME_WIDTH)

/** How many pixels from the right the legenda will be drawn.
  */
#define LEGENDA_RIGHT_OFFSET QSettings().value("legendaRightOffset", 90).toInt()
/** How many pixels from the bottom the legenda will be drawn.
  */
#define LEGENDA_BOTTOM_OFFSET QSettings().value("legendaBottomOffset", 90).toInt()

/** The margin between the problems.
  */
#define PROB_MARGIN QSettings().value("probMargin", 15).toInt()
/** The margin between the team name and the problem list.
  */
#define NAME_PROBS_MARGIN QSettings().value("nameProbsMargin", 0).toInt()
/** The margin below the problem list.
  */
#define PROBS_BELOW_MARGIN QSettings().value("probsBelowMargin", 5).toInt()
/** The time to wait
  */
#define TIME_TO_WAIT QSettings().value("timeToWait", 1000).toInt()
/** The time to scroll
  */
#define TIME_TO_SCROLL QSettings().value("timeToScroll", 1000).toInt()
/** The time before the legend is hidden
  */
#define LEGEND_WAIT_TIME QSettings().value("legendWaitTime", 5000).toInt()
/** The time for the legend to hide
  */
#define LEGEND_HIDE_TIME QSettings().value("legendHideTime", 2000).toInt()
/** The time per item to scroll down
  */
#define TIME_PER_ITEM QSettings().value("timePerItem", 1200).toInt()
/** The time for the winner to show
  */
#define TIME_FOR_WINNER QSettings().value("timeForWinner", 2000).toInt()
/** The time to move
  */
#define TIME_TO_MOVE QSettings().value("timeToMove", 200).toInt()
/** The initial time to move
  */
#define TIME_TO_MOVE_INIT QSettings().value("timeToMoveInit", 500).toInt()
/** The time to blink
  */
#define TIME_TO_BLINK QSettings().value("timeToBlink", 2000).toInt()
/** The X offset of the branding image
  */
#define BRANDING_IMAGE_OFFSET_X QSettings().value("brandingImageOffsetX", 5).toInt()
/** The Y offset of the branding image
  */
#define BRANDING_IMAGE_OFFSET_Y QSettings().value("brandingImageOffsetY", 5).toInt()

#include <QString>
#include <QList>
#include <QApplication>
#include <QDesktopWidget>
#include <QSettings>

namespace DJ {
/** State of a problem.
  */
enum ProblemState {
    NOTSUBMITTED, /**< Problem is not submitted. */
    SOLVED, /**< Problem is solved. */
    FAILED, /**< Problem is submitted, but not solved. */
    PENDING_SOLVED, /**< Problem is solved, but after the freeze time. */
    PENDING_FAILED /**< Problem is submitted after the freeze time and not solved. */
};

/** What medal this team earned.
  */
enum Medal {
    NO_MEDAL, /**< This team earned no medal at al. */
    GOLD_MEDAL, /**< This team earned a gold medal. */
    SILVER_MEDAL, /**< This team earned a silver medal. */
    BRONZE_MEDAL /**< This team earned a bronze medal. */
};

/** Struct for keeping track of info for a problem.
  * This struct is used in the standings controller.
  */
struct ResultProblem {
    ProblemState state; /**< State for this problem. */
    int numTries; /**< Number of tries for this problem. */
    int time; /**< Time of the last try. */
    QString problemId; /**< ID for this problem. */
};

/** Struct for keeping track of info for a team.
  * This struct is used in the standings controller.
  */
struct ResultTeam {
    QString name; /**< Name of this team. */
    QString id; /**< ID for this team. */
    int rank; /**< Current rank of this team. */
    int solved; /**< How many problems this team solved. */
    int time; /**< The total time for this team. */
    QList<ResultProblem> problems; /**< The list of problems for this team. */
};
}

#endif // DEFINES_H
