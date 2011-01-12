#ifndef RESULTSWINDOW_H
#define RESULTSWINDOW_H

#include <QWidget>
#include <QLabel>
#include <QImage>

namespace DJ {
namespace View {

class ResultsWindow : public QWidget {
	Q_OBJECT
public:
	explicit ResultsWindow(QWidget *parent = 0);
	void setBrandingImageFile(QString filename);

protected:
	void keyPressEvent(QKeyEvent *event);

private:
	void updateBrandingRect();

	QLabel *imageLabel;

signals:

public slots:
	void showFullScreen();
};

} // namespace View
} // namespace DJ

#endif // RESULTSWINDOW_H
