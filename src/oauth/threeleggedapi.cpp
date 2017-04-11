#include "threeleggedapi.h"


using namespace Forge;

ThreeLeggedApi::ThreeLeggedApi(QObject *parent)
	: BaseApi(parent)
	, m_callback_server(new QTcpServer(this))
{
	connect(m_callback_server, &QTcpServer::newConnection, this, &ThreeLeggedApi::getAuthorizationScopes);
}

ThreeLeggedApi::~ThreeLeggedApi()
{
}

void ThreeLeggedApi::authorizeWithScopes(QString scope, QString state)
{
	QString fullPath = m_host + m_endpoint;

	HttpRequestWorker *worker = new HttpRequestWorker();
	HttpRequestInput input(fullPath, "GET");
	input.add_var("client_id", m_client_id);
	input.add_var("response_type", "code");
	input.add_var("redirect_uri", m_callback_url);
	input.add_var("scope", scope);
	if(!state.isEmpty())
	{
		input.add_var("state", state);
	}


	if(startCallbackServer())
	{
		connect(worker, &HttpRequestWorker::on_execution_finished,
			[=](HttpRequestWorker* done)
		{
			QString response = done->m_response;
			emit authorized(done->m_error_string);
			done->deleteLater();
			stopCallbackServer();
		});

		worker->execute(&input);
	}
	else
	{
		emit authorized(QString("unable to start the callback server: %1").arg(m_callback_server->errorString()));
	}

	

}

void ThreeLeggedApi::authorizeWithBrowser()
{
	authorizeWithScopes(m_authorization_scopes);
}


bool ThreeLeggedApi::startCallbackServer() const
{
	bool started = m_callback_server->listen(QHostAddress::LocalHost, 3000);

	return started;
}

void ThreeLeggedApi::getAuthorizationScopes()
{

}

void ThreeLeggedApi::stopCallbackServer() const
{
	m_callback_server->close();
}















void ThreeLeggedApi::set_client_id(const QString& char_)
{
	m_client_id = char_;
}


void ThreeLeggedApi::set_client_secret(const QString& char_)
{
	m_client_secret = char_;
}

QString ThreeLeggedApi::get_client_id() const
{
	return m_client_id;
}

QString ThreeLeggedApi::get_client_secret() const
{
	return m_client_secret;
}

QString ThreeLeggedApi::get_callback_url() const
{
	return m_callback_url;
}

void ThreeLeggedApi::set_callback_url(const QString& char_)
{
	m_callback_url = char_;
}

QString ThreeLeggedApi::get_authorization_scopes() const
{
	return m_authorization_scopes;
}

void ThreeLeggedApi::set_authorization_scopes(const QString& char_)
{
	m_authorization_scopes = char_;
}

