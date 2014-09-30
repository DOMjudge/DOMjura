#include "domjudgeapimanager.h"

#include <QDebug>
#include <QUrlQuery>
#include <QJsonParseError>

namespace DJ
{
namespace Shared
{

void DomjudgeApiManager::setConnectionInfo(QString protocol, QString url, QString username, QString password) {
	this->protocol = protocol;
	this->url = url;
	this->username = username;
	this->password = password;
}

void DomjudgeApiManager::loadContestData() {
	DomjudgeApiRequest request("contest");
	this->contestRequests.append(request);

	this->accessManager->get(request);
}

DomjudgeApiManager::DomjudgeApiRequest::DomjudgeApiRequest(QString method, QList<QPair<QString, QString> > arguments) {
	DomjudgeApiManager *apiManager = DomjudgeApiManager::sharedApiManager();
	QUrl url(apiManager->protocol + apiManager->url + "/api/" + method);

	QPair<QString, QString> argument;
	QUrlQuery urlQuery;
	foreach (argument, arguments) {
		urlQuery.addQueryItem(argument.first, argument.second);
	}
	url.setQuery(urlQuery);

	this->setUrl(url);
}

DomjudgeApiManager::DomjudgeApiManager() {
	this->accessManager = new QNetworkAccessManager(this);
	connect(this->accessManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));
}

void DomjudgeApiManager::replyFinished(QNetworkReply *reply) {
	DomjudgeApiManager *apiManager = DomjudgeApiManager::sharedApiManager();
	// See if it is a contest request
	if (apiManager->contestRequests.contains(reply->request())) {
		// Yes, process as contest request
		if (reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt() != 200) {
			emit contestDataFailedLoading();
		} else {
			QJsonParseError error;
			QJsonDocument document = QJsonDocument::fromJson(reply->readAll(), &error);
			if (error.error != QJsonParseError::NoError) {
				emit contestDataFailedLoading();
			} else {
				emit contestDataLoaded(document.object());
			}
		}

		apiManager->contestRequests.removeOne(reply->request());
	}
}

}
}
