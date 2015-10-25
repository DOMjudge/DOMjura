#include "team.h"

#include <QJsonObject>

#include "category.h"

namespace DJ {
namespace Model {
Team::Team(QJsonObject team, QHash<int, Category *> categories, QObject *parent) : QObject(parent) {
	this->id = team.value("id").toInt();
	this->name = team.value("name").toString("UNKNOWN");
	int categoryId = team.value("category").toInt();
	if (categories.contains(categoryId)) {
		this->category = categories[categoryId];
		this->category->addTeam(this);
	} else {
		this->category = NULL;
	}
}

QString Team::getName() {
	return this->name;
}

int Team::getId() {
	return this->id;
}

Category *Team::getCategory() {
	return this->category;
}

QString Team::toString() {
	QString s;
	s += "    id          = " + QString::number(this->id) + "\n";
	s += "    name        = " + this->name + "\n";
	s += "    category    = " + QString::number(this->category->getId()) + " (" + this->category->getName() + ")\n";
	return s;
}
}
}
