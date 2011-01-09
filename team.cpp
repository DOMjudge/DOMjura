#include "team.h"

#include "category.h"
#include "affiliation.h"
#include "scoreboard.h"

namespace DJ {
namespace Model {
Team::Team(QString id, QString categoryid, QString affiliationid, QObject *parent) : QObject(parent) {
	this->id = id;
	this->categoryid = categoryid;
	this->affiliationid = affiliationid;
}

void Team::updateIds(Scoreboard *scoreboard) {
	this->category = scoreboard->getCategoryById(this->categoryid);
	this->affiliation = scoreboard->getAffiliationById(this->affiliationid);
}

void Team::setName(QString name) {
	this->name = name;
}

QString Team::getName() {
	return this->name;
}

QString Team::getId() {
	return this->id;
}

Category *Team::getCategory() {
	return this->category;
}

QString Team::toString() {
	QString s;
	s += "    id          = " + this->id + "\n";
	s += "    category    = " + this->category->getId() + " (" + this->category->getName() + ")\n";
	if (this->affiliation) {
		s += "    affiliation = " + this->affiliation->getId() + " (" + this->affiliation->getName() + ")\n";
	}
	return s;
}
}
}
