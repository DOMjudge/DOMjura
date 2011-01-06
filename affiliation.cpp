#include "affiliation.h"


namespace DJ {
	namespace Model {
		Affiliation::Affiliation(QString id, QString country, QObject *parent) : QObject(parent) {
			this->id = id;
			this->country = country;
		}

		void Affiliation::setName(QString name) {
			this->name = name;
		}

		QString Affiliation::toString() {
			QString s;
			s += "    id      = " + this->id + "\n";
			s += "    country = " + this->country + "\n";
			s += "    name    = " + this->name + "\n";
			return s;
		}
	}
}
