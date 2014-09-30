#ifndef DOMJUDGEAPIMANAGER_H
#define DOMJUDGEAPIMANAGER_H

#include <QObject>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <QJsonObject>

#include <QDebug>

namespace DJ
{
namespace Shared
{

class DomjudgeApiManager : public QObject
{
	Q_OBJECT
public:
	static DomjudgeApiManager *sharedApiManager() {
		static DomjudgeApiManager *_instance = NULL;

		if (!_instance) {
			_instance = new DomjudgeApiManager();
		}

		return _instance;
	}

	void setConnectionInfo(QString protocol, QString url, QString username, QString password);
	void loadContestData();

protected:
	class DomjudgeApiRequest : public QNetworkRequest {
		friend class DomjudgeApiManager;
		explicit DomjudgeApiRequest(QString method, QList<QPair<QString, QString> > arguments = QList<QPair<QString, QString> >());
	};

private:
	explicit DomjudgeApiManager();
	QString protocol, url, username, password;
	QNetworkAccessManager *accessManager;

	QList<QNetworkRequest> contestRequests;

signals:
	void contestDataLoaded(QJsonObject contestData);
	void contestDataFailedLoading();

private slots:
	void replyFinished(QNetworkReply *reply);

};

}
}

#endif // DOMJUDGEAPIMANAGER_H
