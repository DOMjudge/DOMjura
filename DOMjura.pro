#-------------------------------------------------
#
# Project created by QtCreator 2011-01-02T13:40:44
#
#-------------------------------------------------

QT       += core gui xml network opengl widgets
CONFIG   += c++11

TARGET = DOMjura
TEMPLATE = app

ICON = resources/AppIcon.icns
RC_FILE = DOMjura.rc

SOURCES += main.cpp \
        problemgraphicsitem.cpp \
        contest.cpp \
        problem.cpp \
        team.cpp \
        maincontroller.cpp \
        standingscontroller.cpp \
        rankedteam.cpp \
        maindialog.cpp \
        aboutdialog.cpp \
        resultswindow.cpp \
        headergraphicsitem.cpp \
        teamgraphicsitem.cpp \
        legendagraphicsitem.cpp \
        settingsdialog.cpp \
        winnergraphicsitem.cpp \
        gradientcache.cpp \
        domjudgeapimanager.cpp \
    submission.cpp \
    judging.cpp \
    group.cpp

HEADERS  += problemgraphicsitem.h \
        defines.h \
        contest.h \
        problem.h \
        team.h \
        maincontroller.h \
        standingscontroller.h \
        rankedteam.h \
        maindialog.h \
        aboutdialog.h \
        resultswindow.h \
        headergraphicsitem.h \
        teamgraphicsitem.h \
        legendagraphicsitem.h \
        settingsdialog.h \
        winnergraphicsitem.h \
        gradientcache.h \
        domjudgeapimanager.h \
    submission.h \
    judging.h \
    group.h

FORMS += \
        maindialog.ui \
        aboutdialog.ui \
        settingsdialog.ui

RESOURCES += \
        resources.qrc
