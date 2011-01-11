#include "category.h"

namespace DJ {
namespace Model {
Category::Category(QString id, QColor color, QObject *parent) : QObject(parent) {
	this->id = id;
	this->color = color;
}

void Category::setName(QString name) {
	this->name = name;
}

QString Category::getName() {
	return this->name;
}

QString Category::getId() {
	return this->id;
}

QColor Category::getColor() {
	return this->color;
}

QString Category::toString() {
	QString s;
	s += "    id    = " + this->id + "\n";
	s += "    color = " + this->color.name() + "\n";
	s += "    name  = " + this->name + "\n";
	return s;
}

}
}
