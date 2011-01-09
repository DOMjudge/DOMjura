#ifndef MAINCONTROLLER_H
#define MAINCONTROLLER_H

#include <QObject>

#include "readdatacontroller.h"
#include "statscontroller.h"
#include "standingscontroller.h"

namespace DJ {
	namespace Controller {
		class MainController : public QObject {
			Q_OBJECT
		public:
			explicit MainController(QObject *parent = 0);
			void go();

		signals:

		public slots:
			void dataRead();

		private:
			ReadDataController *readDataController;
			StatsController *statsController;
			StandingsController *standingsController;
		};
	} // namespace Controller
} // namespace DJ

#endif // MAINCONTROLLER_H
