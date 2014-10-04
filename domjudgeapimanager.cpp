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

void DomjudgeApiManager::loadRoleData() {
	DomjudgeApiRequest request("roles");
	this->roleRequests.append(request);

	this->accessManager->get(request);
}

void DomjudgeApiManager::loadContestData() {
	DomjudgeApiRequest request("contest");
	this->contestRequests.append(request);

	this->accessManager->get(request);
}

void DomjudgeApiManager::loadCategoriesData() {
	DomjudgeApiRequest request("categories");
	this->categoriesRequests.append(request);

	this->accessManager->get(request);
}

void DomjudgeApiManager::loadTeamData() {
	DomjudgeApiRequest request("teams");
	this->teamsRequests.append(request);

	this->accessManager->get(request);
}

void DomjudgeApiManager::loadProblemData() {
	DomjudgeApiRequest request("problems");
	this->problemRequests.append(request);

	this->accessManager->get(request);
}

void DomjudgeApiManager::loadSubmissions(int fromId)
{
	QList<QPair<QString, QString>> arguments;
	if (fromId >= 0) {
		arguments.append(QPair<QString, QString>("fromid", QString::number(fromId)));
	}

	DomjudgeApiRequest request("submissions", arguments);
	this->submissionRequests.append(request);

	this->accessManager->get(request);
}

void DomjudgeApiManager::loadJudgings(int fromId)
{
	QList<QPair<QString, QString>> arguments;
	if (fromId >= 0) {
		arguments.append(QPair<QString, QString>("fromid", QString::number(fromId)));
	}

	DomjudgeApiRequest request("judgings", arguments);
	this->judgingRequests.append(request);

	this->accessManager->get(request);
}

DomjudgeApiManager::DomjudgeApiRequest::DomjudgeApiRequest(QString method, QList<QPair<QString, QString>> arguments) {
	DomjudgeApiManager *apiManager = DomjudgeApiManager::sharedApiManager();
	QUrl url(apiManager->protocol + apiManager->url + "/api/" + method);

	QUrlQuery urlQuery;
	foreach (auto argument, arguments) {
		urlQuery.addQueryItem(argument.first, argument.second);
	}
	url.setQuery(urlQuery);

	this->setUrl(url);

	if (!apiManager->username.isEmpty()) {
		QString concatenated = apiManager->username + ":" + apiManager->password;
		QByteArray data = concatenated.toLocal8Bit().toBase64();
		QString headerData = "Basic " + data;
		this->setRawHeader("Authorization", headerData.toLocal8Bit());
	}
}

DomjudgeApiManager::DomjudgeApiManager() {
	this->accessManager = new QNetworkAccessManager(this);
	connect(this->accessManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));
}

void DomjudgeApiManager::replyFinished(QNetworkReply *reply) {
	DomjudgeApiManager *apiManager = DomjudgeApiManager::sharedApiManager();

	// Process all types of replies
	if (this->processReply(reply, &apiManager->roleRequests,
						   &DomjudgeApiManager::roleDataFailedLoading,
						   &DomjudgeApiManager::rolesLoaded)) {
		return;
	}
	if (this->processReply(reply, &apiManager->contestRequests,
						   &DomjudgeApiManager::contestDataFailedLoading,
						   &DomjudgeApiManager::contestDataLoaded)) {
		return;
	}
	if (this->processReply(reply, &apiManager->categoriesRequests,
						   &DomjudgeApiManager::categoriesDataFailedLoading,
						   &DomjudgeApiManager::categoriesDataLoaded)) {
		return;
	}
	if (this->processReply(reply, &apiManager->teamsRequests,
						   &DomjudgeApiManager::teamsDataFailedLoading,
						   &DomjudgeApiManager::teamsDataLoaded)) {
		return;
	}
	if (this->processReply(reply, &apiManager->problemRequests,
						   &DomjudgeApiManager::problemsDataFailedLoading,
						   &DomjudgeApiManager::problemsDataLoaded)) {
		return;
	}
	if (this->processReply(reply, &apiManager->submissionRequests,
						   &DomjudgeApiManager::submissionsDataFailedLoading,
						   &DomjudgeApiManager::submissionsDataLoaded)) {
		return;
	}
	if (this->processReply(reply, &apiManager->judgingRequests,
						   &DomjudgeApiManager::judgingsDataFailedLoading,
						   &DomjudgeApiManager::judgingsDataLoaded)) {
		return;
	}
}

template<typename... ArgsError, typename... ArgsSuccess>
bool DomjudgeApiManager::processReply(QNetworkReply *reply,
									  QList<QNetworkRequest> *requests,
									  void(DomjudgeApiManager::*errorFunc)(ArgsError...args),
									  void(DomjudgeApiManager::*successFunc)(ArgsSuccess...args)) {
	if (requests->contains(reply->request())) {
		DomjudgeApiManager *apiManager = DomjudgeApiManager::sharedApiManager();

		// See if we must do a redirect and if so, do one
		auto possibleRedirectURL = reply->attribute(QNetworkRequest::RedirectionTargetAttribute).toUrl();
		if (!possibleRedirectURL.isEmpty()) {
			auto newRequest = QNetworkRequest(possibleRedirectURL);
			if (requests->contains(reply->request())) {
				requests->append(newRequest);
				requests->removeOne(reply->request());
			}
			apiManager->accessManager->get(newRequest);
			return true;
		}

		int status_code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();

		QString error = "General error";
		if (status_code == 401) {
			error = "Authentication required. Please enter correct username and password";
		} else if (status_code == 0) {
			error = "Can not find server. Please enter correct URL";
		} else if (status_code == 404) {
			error = "Can not find this URL; please enter the correct one";
		}

		if (status_code != 200) {
			std::function<void(DomjudgeApiManager *, ArgsError...)> func = errorFunc;
			emit func(this, error);
		} else {
			QJsonParseError error;
			QJsonDocument document = QJsonDocument::fromJson(reply->readAll(), &error);
			if (error.error != QJsonParseError::NoError) {
				std::function<void(DomjudgeApiManager *, ArgsError...)> func = errorFunc;
				emit func(this, "Error parsing JSON");
			} else {
				std::function<void(DomjudgeApiManager *, ArgsSuccess...)> func = successFunc;
				emit func(this, document);
			}
		}

		requests->removeOne(reply->request());

		return true;
	}

	return false;
}

}
}
