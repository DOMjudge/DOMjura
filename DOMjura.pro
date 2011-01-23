#-------------------------------------------------
#
# Project created by QtCreator 2011-01-02T13:40:44
#
#-------------------------------------------------

QT       += core gui xml network opengl

TARGET = DOMjura
TEMPLATE = app


SOURCES += main.cpp \
	problemgraphicsitem.cpp \
	contest.cpp \
	readdatacontroller.cpp \
	scoreboard.cpp \
	problem.cpp \
	language.cpp \
	affiliation.cpp \
	category.cpp \
	team.cpp \
	events.cpp \
	event.cpp \
	submissionevent.cpp \
	judgingevent.cpp \
	statscontroller.cpp \
	maincontroller.cpp \
	standingscontroller.cpp \
	rankedteam.cpp \
	maindialog.cpp \
	aboutdialog.cpp \
	statsdialog.cpp \
	resultswindow.cpp \
	headergraphicsitem.cpp \
	teamgraphicsitem.cpp \
	legendagraphicsitem.cpp \
    settingsdialog.cpp \
    winnergraphicsitem.cpp

HEADERS  += problemgraphicsitem.h \
	defines.h \
	contest.h \
	readdatacontroller.h \
	scoreboard.h \
	problem.h \
	language.h \
	affiliation.h \
	category.h \
	team.h \
	events.h \
	event.h \
	submissionevent.h \
	judgingevent.h \
	statscontroller.h \
	maincontroller.h \
	standingscontroller.h \
	rankedteam.h \
	maindialog.h \
	aboutdialog.h \
	statsdialog.h \
	resultswindow.h \
	headergraphicsitem.h \
	teamgraphicsitem.h \
	legendagraphicsitem.h \
    settingsdialog.h \
    winnergraphicsitem.h

FORMS += \
	maindialog.ui \
	aboutdialog.ui \
	statsdialog.ui \
    settingsdialog.ui
