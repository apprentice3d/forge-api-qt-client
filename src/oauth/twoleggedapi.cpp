#include "twoleggedapi.h"

using namespace Forge;

TwoLeggedApi::TwoLeggedApi(QObject *parent)
	: BaseApi(parent)
	, m_client_id(QString(""))
	, m_client_secret(QString(""))
{
	m_endpoint = QString("/authentication/v1/authenticate");
}




void TwoLeggedApi::getTokenWithScope(QString scope)
{
	QString fullPath = m_host + m_endpoint;

	HttpRequestWorker *worker = new HttpRequestWorker();
	HttpRequestInput input(fullPath, "POST");
	input.add_var("client_id", m_client_id);
	input.add_var("client_secret", m_client_secret);
	input.add_var("grant_type", "client_credentials");
	input.add_var("scope", scope);


	connect(worker, &HttpRequestWorker::on_execution_finished,
		[=](HttpRequestWorker* done)
	{
//		Bearer* output = new Bearer(done->m_response, this);
		Bearer* output = new Bearer(done->m_response);
		output->set_scope(scope);
		output->set_creation_time(QDateTime::currentMSecsSinceEpoch());
		emit authenticateSignal(output, done->m_error_string);
		done->deleteLater();
	});

	worker->execute(&input);
}


void TwoLeggedApi::set_client_id(const QString& char_)
{
	m_client_id = char_;
}


void TwoLeggedApi::set_client_secret(const QString& char_)
{
	m_client_secret = char_;
}

QString TwoLeggedApi::get_client_id() const
{
	return m_client_id;
}

QString TwoLeggedApi::get_client_secret() const
{
	return m_client_secret;
}

