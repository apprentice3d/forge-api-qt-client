#pragma once

#include <QObject>

namespace Forge {

class BaseApi : public QObject
{
	Q_OBJECT

public:
    BaseApi(QObject *parent = nullptr);
	virtual ~BaseApi() = 0;

	QString get_host() const;
	void set_host(const QString& host);
	QString get_endpoint() const;
	void set_endpoint(const QString& endpoint);
protected:
	QString m_host;
	QString m_endpoint;

};




}
