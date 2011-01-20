#include "resultswindow.h"

#include <QKeyEvent>
#include <QApplication>
#include <QDesktopWidget>
#include <QPropertyAnimation>
#include <QDebug>
#include <QGLWidget>
#include <math.h>

#include "defines.h"

namespace DJ {
namespace View {

ResultsWindow::ResultsWindow(QWidget *parent) : QWidget(parent) {
	this->scrollToBottomAnim = NULL;
	this->legendaAnim = NULL;

	this->started = false;

	this->view = new QGraphicsView(this);

	this->scene = new QGraphicsScene(this);
	this->scene->setSceneRect(QApplication::desktop()->screenGeometry());
	this->scene->setBackgroundBrush(Qt::black);

	this->view->setScene(this->scene);
	this->view->setGeometry(QApplication::desktop()->screenGeometry());

	this->view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	this->view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	this->view->setStyleSheet("background-color: black; border: none;");
	this->view->centerOn(0, 0);

	this->view->setViewport(new QGLWidget(QGLFormat(QGL::SampleBuffers)));

	this->headerItem = new HeaderGraphicsItem(QApplication::desktop()->screenGeometry().width());
	this->headerItem->setPos(0, 0);

	this->legendaItem = new LegendaGraphicsItem();
	QRectF legendaRect = this->legendaItem->boundingRect();
	this->legendaItem->setPos(QApplication::desktop()->screenGeometry().width() - legendaRect.width() - LEGENDA_RIGHT_OFFSET,
							  QApplication::desktop()->screenGeometry().height() - legendaRect.height() - LEGENDA_BOTTOM_OFFSET);
	this->legendaItem->setZValue(1);

	this->pixmap = new QGraphicsPixmapItem;
	this->pixmap->setZValue(1);

	this->scene->addItem(this->pixmap);
	this->scene->addItem(this->headerItem);
	this->scene->addItem(this->legendaItem);

	this->legendaTimer = new QTimer(this);
	this->legendaTimer->setSingleShot(true);
	connect(this->legendaTimer, SIGNAL(timeout()), this, SLOT(hideLegenda()));
}

void ResultsWindow::setBrandingImageFile(QString filename) {
	if (!filename.isEmpty()) {
		QPixmap pixmap(filename);
		this->pixmap->setPixmap(pixmap);
	} else {
		this->pixmap->setPixmap(QPixmap());
	}
	resizeImage();
}

void ResultsWindow::setTeams(QList<ResultTeam> teams) {
	for (int i = 0; i < this->teamItems.size(); i++) {
		this->scene->removeItem(this->teamItems.at(i));
		delete this->teamItems.at(i);
	}
	this->teamItems.clear();

	if (teams.size() == 0) {
		return;
	}
	this->teams = teams;
	int numprobs = teams.at(0).problems.size();
	int probWidth = (NAME_WIDTH - (numprobs - 1) * PROB_MARGIN) / numprobs;

	for (int i = 0; i < teams.size(); i++) {
		QList<ResultProblem> problems = teams.at(i).problems;
		if (problems.size() == 0) {
			return;
		}
		QList<ProblemGraphicsItem *> problemItems;
		for (int j = 0; j < numprobs; j++) {
			ProblemGraphicsItem *probItem = new ProblemGraphicsItem(0, probWidth);
			probItem->setProblemId(problems.at(j).problemId);
			probItem->setState(problems.at(j).state);
			probItem->setNumTries(problems.at(j).numTries);
			probItem->setTime(problems.at(j).time);
			problemItems.append(probItem);
		}

		TeamGraphicsItem *teamItem = new TeamGraphicsItem(problemItems);
		teamItem->setRank(teams.at(i).rank);
		teamItem->setName(teams.at(i).name);
		teamItem->setSolved(teams.at(i).solved);
		teamItem->setTime(teams.at(i).time);
		teamItem->setPos(0, HEADER_HEIGHT + i * TEAMITEM_HEIGHT);
		teamItem->setEven(i % 2 == 0);

		this->teamItems.append(teamItem);
		this->scene->addItem(teamItem);
	}
}

void ResultsWindow::keyPressEvent(QKeyEvent *event) {
	switch (event->key()) {
	case Qt::Key_Escape:
	case Qt::Key_Q:
	case Qt::Key_X:
		close();
		break;
	case Qt::Key_Enter:
	case Qt::Key_Return:
	case Qt::Key_Space:
		doNextStep();
	}
}

void ResultsWindow::mousePressEvent(QMouseEvent *event) {
	if (event->button() == Qt::LeftButton) {
		doNextStep();
	}
}

void ResultsWindow::reload() {
	this->started = false;

	if (this->scrollToBottomAnim) {
		this->scrollToBottomAnim->clear();
		delete this->scrollToBottomAnim;
		this->scrollToBottomAnim = NULL;
	}

	if (this->legendaAnim) {
		delete this->legendaAnim;
		this->legendaAnim = NULL;
	}
	this->headerItem->setPos(0, 0);
	this->hideLegendAfterTimeout();
}

void ResultsWindow::hideLegendAfterTimeout() {
	this->legendaItem->setOpacity(1);
	this->legendaTimer->start(5000);
}

void ResultsWindow::doNextStep() {
	if (!this->started) {
		if (this->scrollToBottomAnim) {
			this->scrollToBottomAnim->clear();
			delete this->scrollToBottomAnim;
			this->scrollToBottomAnim = NULL;
		}
		this->scrollToBottomAnim = new QParallelAnimationGroup;
		connect(this->scrollToBottomAnim, SIGNAL(finished()), this, SLOT(animationDone()));

		int screenHeight = QApplication::desktop()->screenGeometry().height();
		int totalItemsHeight = HEADER_HEIGHT + this->teamItems.size() * TEAMITEM_HEIGHT + SCROLL_BELOW_OFFSET;
		int toScroll = qMax(0, totalItemsHeight - screenHeight);
		QPointF toScrollPoint(0, toScroll);

		QPropertyAnimation *animHeader = new QPropertyAnimation(this->headerItem, "pos");
		animHeader->setDuration(1000 + 1200 * log(this->teamItems.size()));
		animHeader->setEasingCurve(QEasingCurve::OutBack);
		animHeader->setStartValue(QPointF(0, 0));
		animHeader->setEndValue(QPointF(0, 0) - toScrollPoint);
		this->scrollToBottomAnim->addAnimation(animHeader);

		for (int i = 0; i < this->teamItems.size(); i++) {
			QPropertyAnimation *animItem = new QPropertyAnimation(this->teamItems.at(i), "pos");
			animItem->setDuration(1000 + 1200 * log(this->teamItems.size()));
			animItem->setEasingCurve(QEasingCurve::OutBack);
			QPointF startPoint;
			startPoint.setX(0);
			startPoint.setY(HEADER_HEIGHT + i * TEAMITEM_HEIGHT);
			QPointF newPoint = startPoint;
			newPoint -= toScrollPoint;
			animItem->setStartValue(startPoint);
			animItem->setEndValue(newPoint);
			this->scrollToBottomAnim->addAnimation(animItem);
		}

		this->scrollToBottomAnim->start();
	}
}

void ResultsWindow::animationDone() {
	if (this->sender() == this->scrollToBottomAnim) {
		this->started = true;
		TeamGraphicsItem *item = this->teamItems.at(this->teamItems.size() - 1);
		item->setHighlighted(true);
	}
}

void ResultsWindow::resizeImage() {
	QSize size;
	if (!this->pixmap->pixmap().isNull()) {
		size = this->pixmap->pixmap().size();
	} else {
		size = QSize(0, 0);
	}
	QRect screenSize = QApplication::desktop()->screenGeometry();
	QPointF labelPos;
	labelPos.setX(screenSize.width() - size.width());
	labelPos.setY(screenSize.height() - size.height());
	this->pixmap->setPos(labelPos);
}

void ResultsWindow::hideLegenda() {
	if (this->legendaAnim) {
		delete this->legendaAnim;
		this->legendaAnim = NULL;
	}
	this->legendaAnim = new QPropertyAnimation(this->legendaItem, "opacity");
	this->legendaAnim->setDuration(10000);
	this->legendaAnim->setEasingCurve(QEasingCurve::InOutExpo);
	this->legendaAnim->setStartValue(1);
	this->legendaAnim->setEndValue(0);

	this->legendaAnim->start();
}

} // namespace View
} // namespace DJ
