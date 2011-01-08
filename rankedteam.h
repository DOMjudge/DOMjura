#ifndef RANKEDTEAM_H
#define RANKEDTEAM_H

#include <QObject>

namespace DJ {
	namespace Model {
		class RankedTeam : public QObject {
			Q_OBJECT
		public:
			explicit RankedTeam(QObject *parent = 0);
		};
	} // namespace Model
} // namespace DJ

#endif // RANKEDTEAM_H
