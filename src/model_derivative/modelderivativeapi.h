#pragma once

#include <QObject>
#include <QDateTime>
#include <QJsonArray>
#include "../shared/baseapi.h"
#include "../oauth/twoleggedapi.h"


namespace Forge{

	static int refresh_period_in_secs = 360;



	struct SupportedFormats
	{
		QDateTime last_time_updated;
		QHash<QString, QList<QString>> supported_formats;
	};




class ModelDerivativeApi : public BaseApi
{
	Q_OBJECT

public:
	explicit ModelDerivativeApi(QObject *parent = nullptr);
	~ModelDerivativeApi();

	void addTokenManager(TwoLeggedApi* token_requester);

	void getFormats(QString for_source = "");
	void updateFormatsFrom(QString source);


	SupportedFormats get_cached_formats() const;
signals:
	void getFormatsSignal(SupportedFormats* formats, QString error_string);



private:
	SupportedFormats m_formats;
	TwoLeggedApi* m_token_manager;


	//	void getFormatsCallback(HttpRequestWorker* worker);
	//Auxiliary
	SupportedFormats* filterFormatsForSource(QString source);


};
}




/*
 *     void deleteManifest(QString* urn);
    void getDerivativeManifest(QString* urn, QString* derivative_urn, qint32 range);
    void getFormats(QDate* if_modified_since, QString* accept_encoding);
    void getManifest(QString* urn, QString* accept_encoding);
    void getMetadata(QString* urn, QString* accept_encoding);
    void getModelviewMetadata(QString* urn, QString* guid, QString* accept_encoding);
    void getModelviewProperties(QString* urn, QString* guid, QString* accept_encoding);
    void getThumbnail(QString* urn, qint32 width, qint32 height);
    void translate(SWGJobPayload job, bool x_ads_force);
    
private:
    void deleteManifestCallback (HttpRequestWorker * worker);
    void getDerivativeManifestCallback (HttpRequestWorker * worker);
    void getFormatsCallback (HttpRequestWorker * worker);
    void getManifestCallback (HttpRequestWorker * worker);
    void getMetadataCallback (HttpRequestWorker * worker);
    void getModelviewMetadataCallback (HttpRequestWorker * worker);
    void getModelviewPropertiesCallback (HttpRequestWorker * worker);
    void getThumbnailCallback (HttpRequestWorker * worker);
    void translateCallback (HttpRequestWorker * worker);
    
signals:
    void deleteManifestSignal(SWGResult* summary);
    void getDerivativeManifestSignal();
    void getFormatsSignal(SWGFormats* summary);
    void getManifestSignal(SWGManifest* summary);
    void getMetadataSignal(SWGMetadata* summary);
    void getModelviewMetadataSignal(SWGMetadata* summary);
    void getModelviewPropertiesSignal(SWGMetadata* summary);
    void getThumbnailSignal(SWGInputStream* summary);
    void translateSignal(SWGJob* summary);
 */