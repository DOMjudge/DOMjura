#ifndef GRADIENTCACHE_H
#define GRADIENTCACHE_H

#include <QObject>
#include <QHash>
#include <QPixmap>

#include "defines.h"

namespace DJ {
namespace View {

struct ColorPair {
	ColorPair();
	ColorPair(QColor c1, QColor c2) {
		this->color1 = c1;
		this->color2 = c2;
	}

	QColor color1;
	QColor color2;
};

inline bool operator==(const ColorPair &e1, const ColorPair &e2) {
	return e1.color1 == e2.color1 && e1.color2 == e2.color2;
}

inline uint qHash(const ColorPair &key) {
	QString str;
	str += key.color1.name();
	str += key.color2.name();
	return qHash(str);
}

class GradientCache : public QObject {
	Q_OBJECT
public:
	static GradientCache *getInstance();

	void clearCache();
	void setNumProbs(int n);
	void setHeight(int height);
	QPixmap getColorGradient(QColor color1, QColor color2);
	QPixmap getColorGradientHighlighted(QColor color1, QColor color2);
	QPixmap getMedalGradient(Medal medal);
	QPixmap getOddEvenHighlightedGradient(int oddEvenHighlighted);

protected:
	GradientCache();

private:
	static GradientCache *instance;

	int numProbs;
	int height;
	QHash<ColorPair, QPixmap> problemGradient;
	QHash<ColorPair, QPixmap> problemGradientHighlighted;
	QHash<Medal, QPixmap> medalGradient;
	QHash<int, QPixmap> oddEvenHighlightGradient;

};

} // namespace View
} // namespace DJ

#endif // GRADIENTCACHE_H
