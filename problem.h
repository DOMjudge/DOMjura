#ifndef PROBLEM_H
#define PROBLEM_H

#include <QObject>
#include <QColor>

namespace DJ {
	namespace Model {
		class Problem : public QObject {
			Q_OBJECT
		public:
			explicit Problem(QString id, QColor color, QObject *parent = 0);
			void setName(QString name);
			QString toString();

		private:
			QString id;
			QColor color;
			QString name;
		};
	}
}

#endif // PROBLEM_H
