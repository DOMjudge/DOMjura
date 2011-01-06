#ifndef AFFILIATION_H
#define AFFILIATION_H

#include <QObject>

namespace DJ {
	namespace Model {
		class Affiliation : public QObject {
			Q_OBJECT
		public:
			explicit Affiliation(QString id, QString country, QObject *parent = 0);
			void setName(QString name);
			QString getId();

			QString toString();

		private:
			QString id;
			QString country;
			QString name;
		};
	}
}


#endif // AFFILIATION_H
