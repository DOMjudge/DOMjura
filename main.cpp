#include <QtCore/QCoreApplication>
#include <QPixmap>
#include <QBitmap>
#include <QPainter>
#include <QList>
#include <QFont>
#include <QFontMetrics>

void savePixmapOfSize(int size) {
	QPen pen;
	int penWidth = qMax(1, size / 32);
	int roundedNess = qMax(2, size / 8);
	int shadowSize = qMax(1, size / 128);

	pen.setWidth(penWidth);
	pen.setColor(Qt::black);
	QPixmap pm(size, size);
	QBitmap bm(size, size);
	bm.clear();
	pm.setMask(bm);
	QPainter *painter = new QPainter(&pm);

	painter->setPen(pen);
	QRectF rectsh(shadowSize + penWidth / 2.0, shadowSize + penWidth / 2.0, size-penWidth-shadowSize, size-penWidth-shadowSize);
	painter->drawRoundedRect(rectsh, roundedNess, roundedNess);

	pen.setColor(Qt::white);

	QLinearGradient gradient(0, 0, 0, size);
	gradient.setColorAt(0, QColor(0, 230, 0));
	gradient.setColorAt(1, QColor(0, 128, 0));
	QBrush brush(gradient);
	painter->setPen(pen);
	brush.setStyle(Qt::LinearGradientPattern);
	painter->setBrush(brush);

	QRectF rect(penWidth / 2.0, penWidth / 2.0, size-penWidth-shadowSize, size-penWidth-shadowSize);
	painter->drawRoundedRect(rect, roundedNess, roundedNess);

	if (size >= 64) {
		QFont font("Courier new");
		font.setPixelSize(size / 6);
		font.setBold(true);
		painter->setFont(font);
		painter->setPen(Qt::black);
		painter->drawText(qMax(1, size / 128), qMax(1, size / 128), size, size, Qt::AlignCenter, "DOMjura");
		painter->setPen(Qt::white);
		painter->drawText(0, 0, size, size, Qt::AlignCenter, "DOMjura");
	} else {
		QFont font("Courier new");
		font.setPixelSize(size / 2);
		painter->setFont(font);
		painter->setPen(Qt::white);
		painter->setPen(Qt::black);
		painter->drawText(1, 1, size, size, Qt::AlignCenter, "DJ");
		painter->setPen(Qt::white);
		painter->drawText(0, 0, size, size, Qt::AlignCenter, "DJ");
	}
	delete painter;
	pm.save(QString("image-%1.png").arg(size));
}

int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);

	QList<int> sizes;
	sizes.append(16);
	sizes.append(32);
	sizes.append(64);
	sizes.append(128);
	sizes.append(256);

	foreach (int s, sizes) {
		savePixmapOfSize(s);
	}

	return 0;
}
