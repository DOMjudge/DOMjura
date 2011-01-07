#ifndef CATEGORY_H
#define CATEGORY_H

#include <QObject>
#include <QColor>

namespace DJ {
	namespace Model {
		class Category : public QObject
		{
			Q_OBJECT
		public:
			explicit Category(QString id, QColor color, QObject *parent = 0);
			void setName(QString name);
			QString getName();
			QString getId();

			QString toString();

		private:
			QString id;
			QColor color;
			QString name;
		};
	}
}

#endif // CATEGORY_H
