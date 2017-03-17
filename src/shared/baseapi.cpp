#include "baseapi.h"

Forge::BaseApi::BaseApi(QObject *parent)
	: QObject(parent),
	m_host(QString("https://developer.api.autodesk.com")),
	m_endpoint(QString(""))
{
}

Forge::BaseApi::~BaseApi()
{
}

QString Forge::BaseApi::get_host() const
{
	return m_host;
}

void Forge::BaseApi::set_host(const QString& host)
{
	m_host = host;
}

QString Forge::BaseApi::get_endpoint() const
{
	return m_endpoint;
}

void Forge::BaseApi::set_endpoint(const QString& endpoint)
{
	m_endpoint = endpoint;
}


