#include "team.h"

#include "category.h"
#include "affiliation.h"

namespace DJ {
	namespace Model {
		Team::Team(QString id, Category *category, Affiliation *affiliation, QObject *parent) : QObject(parent) {
			this->id = id;
			this->category = category;
			this->affiliation = affiliation;
		}

		void Team::setName(QString name) {
			this->name = name;
		}

		QString Team::getId() {
			return this->id;
		}

		QString Team::toString() {
			QString s;
			s += "    id          = " + this->id + "\n";
			s += "    category    = " + this->category->getId() + "\n";
			if (this->affiliation) {
				s += "    affiliation = " + this->affiliation->getId() + "\n";
			}
			return s;
		}
	}
}
