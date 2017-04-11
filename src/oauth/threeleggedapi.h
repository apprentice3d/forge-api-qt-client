#pragma once

#include "../shared/baseapi.h"
#include "bearer.h"
#include "../shared/httprequest.h"
#include <QTcpServer>
#include <QTcpSocket>
#include <QMessageBox>

namespace Forge{

class ThreeLeggedApi : public BaseApi
{
	Q_OBJECT

public:
	ThreeLeggedApi(QObject *parent = nullptr);
	~ThreeLeggedApi();

	void authorizeWithScopes(QString scopes, QString state="");
	
	void authorizeWithBrowser();
	void authorizeWithWidget(QWidget* auth_window);

	void set_client_id(const QString& client_id);
	void set_client_secret(const QString& client_secret);
	QString get_client_id() const;
	QString get_client_secret() const;
	QString get_callback_url() const;
	void set_callback_url(const QString& char_);
	QString get_authorization_scopes() const;
	void set_authorization_scopes(const QString& char_);

signals:
	void authorized(QString error);

private:
	QString m_client_id;
	QString m_client_secret;
	QString m_callback_url;
	QString m_authorization_scopes;
	QTcpServer* m_callback_server;

	bool startCallbackServer() const;
	void getAuthorizationScopes();
	void stopCallbackServer() const;

};
}
