#ifndef DOMJUDGEAPIMANAGER_H
#define DOMJUDGEAPIMANAGER_H

#include <QObject>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <QJsonDocument>

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
	void loadUserData();
    void loadContestData(QString contestId);
    void loadTeamData(QString cid);
    void loadProblemData(QString cid);
    void loadSubmissions(QString cid);
    void loadJudgings(QString cid);

protected:
	class DomjudgeApiRequest : public QNetworkRequest {
		friend class DomjudgeApiManager;
		explicit DomjudgeApiRequest(QString method, QList<QPair<QString, QString>> arguments = QList<QPair<QString, QString> >());
	};

private:
	explicit DomjudgeApiManager();
	QString protocol, url, username, password;
	QNetworkAccessManager *accessManager;

	QList<QNetworkRequest> userRequests;
    QList<QNetworkRequest> contestRequests;
	QList<QNetworkRequest> teamsRequests;
	QList<QNetworkRequest> problemRequests;
	QList<QNetworkRequest> submissionRequests;
	QList<QNetworkRequest> judgingRequests;

	template<typename... ArgsError, typename... ArgsSuccess>
	bool processReply(QNetworkReply *reply,
					  QList<QNetworkRequest> *requests,
					  void(DomjudgeApiManager::*errorFunc)(ArgsError...args),
					  void(DomjudgeApiManager::*successFunc)(ArgsSuccess...args));

signals:
	void userLoaded(QJsonDocument user);
	void userDataFailedLoading(QString error);

	void contestDataLoaded(QJsonDocument contestData);
	void contestDataFailedLoading(QString error);

	void categoriesDataLoaded(QJsonDocument data);
	void categoriesDataFailedLoading(QString error);

	void teamsDataLoaded(QJsonDocument data);
	void teamsDataFailedLoading(QString error);

	void problemsDataLoaded(QJsonDocument data);
	void problemsDataFailedLoading(QString error);

	void submissionsDataLoaded(QJsonDocument data);
	void submissionsDataFailedLoading(QString error);

	void judgingsDataLoaded(QJsonDocument data);
	void judgingsDataFailedLoading(QString error);

private slots:
	void replyFinished(QNetworkReply *reply);

};

}
}

#endif // DOMJUDGEAPIMANAGER_H
