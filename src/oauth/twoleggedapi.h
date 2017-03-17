#pragma once

#include "../shared/baseapi.h"
#include "../shared/httprequest.h"
#include "bearer.h"

namespace Forge {
class TwoLeggedApi : public BaseApi
{
	Q_OBJECT

public:
	
	TwoLeggedApi(QObject *parent = nullptr);
	~TwoLeggedApi() = default;


	void getTokenWithScope(QString scope);
	void set_client_id(const QString& client_id);
	void set_client_secret(const QString& client_secret);
	QString get_client_id() const;
	QString get_client_secret() const;

signals:
	void authenticateSignal(Bearer* bearer, QString error);


private:
	QString m_client_id;
	QString m_client_secret;


};
}
