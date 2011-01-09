#include "language.h"

namespace DJ {
namespace Model {
Language::Language(QString id, QObject *parent) : QObject(parent) {
	this->id = id;
}

void Language::setName(QString name) {
	this->name = name;
}

QString Language::getId() {
	return this->id;
}

QString Language::toString() {
	QString s;
	s += "    id   = " + this->id + "\n";
	s += "    name = " + this->name + "\n";
	return s;
}
}
}
