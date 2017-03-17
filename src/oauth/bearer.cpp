#include "bearer.h"
#include <qobject.h>


/**
 * \brief Holds data on received token
 */
Forge::Bearer::Bearer(QString json_data, QObject* parent)
	: QObject(parent)
{
	this->init();
	if (json_data != nullptr)
	{
		QJsonDocument data = QJsonDocument::fromJson(json_data.toUtf8());
		QJsonObject json_object = data.object();
		this->m_token_type = QString(json_object["token_type"].toString());
		this->m_expires_in = json_object["expires_in"].toInt();
		this->m_access_token = QString(json_object["access_token"].toString());
		this->m_refresh_token = QString(json_object["refresh_token"].toString());
	}
}

void Forge::Bearer::init()
{
	m_token_type = QString();
	m_expires_in = 0;
	m_creation_time = 0;
	m_access_token = QString();
	m_refresh_token = QString();
	m_token_context = TOKEN_CONTEXT::TWO_LEGGED;
	m_scope = QString();
}





QString Forge::Bearer::get_token_type() const
{
	return m_token_type;
}

void Forge::Bearer::set_token_type(const QString& token_type)
{
	this->m_token_type = token_type;
}

qint32 Forge::Bearer::get_expiration_time() const
{
	return m_expires_in;
}

void Forge::Bearer::set_expiration_time(const qint32 expires_in)
{
	this->m_expires_in = expires_in;
}

QString Forge::Bearer::get_access_token() const
{
	return m_access_token;
}

void Forge::Bearer::set_access_token(const QString& access_token)
{
	this->m_access_token = access_token;
}

QString Forge::Bearer::get_refresh_token() const
{
	return m_refresh_token;
}

void Forge::Bearer::set_refresh_token(const QString& refresh_token)
{
	this->m_refresh_token = refresh_token;
}

Forge::TOKEN_CONTEXT Forge::Bearer::get_token_context() const
{
	return m_token_context;
}

void Forge::Bearer::set_token_context(const TOKEN_CONTEXT token_context)
{
	m_token_context = token_context;
}

QString Forge::Bearer::get_scope() const
{
	return m_scope;
}

void Forge::Bearer::set_scope(const QString& char_)
{
	m_scope = char_;
}

quint64 Forge::Bearer::get_creation_time() const
{
	return m_creation_time;
}

void Forge::Bearer::set_creation_time(quint64 quint64)
{
	m_creation_time = quint64;
}



