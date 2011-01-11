#ifndef LANGUAGE_H
#define LANGUAGE_H

#include <QObject>

namespace DJ {
namespace Model {
class Language : public QObject {
	Q_OBJECT
public:
	explicit Language(QString id, QObject *parent = 0);
	void setName(QString name);
	QString getId();
	QString getName();

	QString toString();

private:
	QString id;
	QString name;
};
}
}

#endif // LANGUAGE_H
