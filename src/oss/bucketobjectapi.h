#pragma once

#include <QObject>
#include "../oauth/twoleggedapi.h"
#include "bucketapi.h"
#include "bucketobject.h"

namespace Forge{

class BucketObjectApi : public BaseApi
{
	Q_OBJECT

public:
	explicit BucketObjectApi(QObject *parent = nullptr);
	~BucketObjectApi();

	/**
	 * \brief adds a token manager responsible for getting access tokens
	 * \param token_manager - a two-legged token requester
	 */
	void addTokenManager(TwoLeggedApi* token_manager);


	/**
	 * \brief List objects in a bucket. It is only available to the bucket creator.
	 * \param bucket_key is an URL-encoded bucket key for which to get details
	 * \param limit to the response size
	 */
	void listObjectsInBucket(QString bucket_key, qint32 limit = 10);
	
	
	/**
	 * \brief List objects in a bucket. It is only available to the bucket creator.
	 * \param bucket object from which to extract the bucket key
	 * \param limit to the response size
	 */
	void listObjectsInBucket(Bucket bucket, qint32 limit = 10);



	void uploadObject(QString bucket_key, QString object_name, QString file_path, QString if_match = "");
	void downloadObject(QString bucket_key, QString object_name, QString file_name_path);


//	void copyTo(QString* bucket_key, QString* object_name, QString* new_obj_name);
//	void createSignedResource(QString* bucket_key, QString* object_name, SWGPost_buckets_signed post_buckets_signed, QString* access);
//	void deleteObject(QString* bucket_key, QString* object_name);
//	void deleteSignedResource(QString* id, QString* region);
//	void getObject(QString* bucket_key, QString* object_name, QString* range, QString* if_none_match, QDate* if_modified_since, QString* accept_encoding);
//	void getObjectDetails(QString* bucket_key, QString* object_name, QDate* if_modified_since, QString* with);
//	void getObjects(QString* bucket_key, qint32 limit, QString* begins_with, QString* start_at);
//	void getSessionid(QString* bucket_key, QString* object_name, QString* session_id);
//	void getSignedResource(QString* id, QString* range, QString* if_none_match, QDate* if_modified_since, QString* accept_encoding, QString* region);
//	void uploadChunk(QString* bucket_key, QString* object_name, qint32 content_length, QString* content_range, QString* session_id, SWGInputStream body, QString* content_disposition, QString* if_match);
//	void uploadObject(QString* bucket_key, QString* object_name, qint32 content_length, SWGInputStream body, QString* content_disposition, QString* if_match);
//	void uploadSignedResource(QString* id, qint32 content_length, SWGInputStream body, QString* content_disposition, QString* x_ads_region, QString* if_match);
//	void uploadSignedResourcesChunk(QString* id, QString* content_range, QString* session_id, SWGInputStream body, QString* content_disposition, QString* x_ads_region);
//


signals:
	void listObjectsSignal(QList<BucketObject*> object_list, QString error_string);
	void uploadObjectSignal(BucketObject* object, QString error_string);
	void downloadObjectSignal(QString filename_path, QString error_string);



private:
	TwoLeggedApi *m_token_manager;

	void listObjectsCallback(HttpRequestWorker* worker);
	void uploadObjectCallback(HttpRequestWorker* worker);
	void downloadObjectCallback(HttpRequestWorker* worker);
	
	//Auxiliary methods
	static QList<BucketObject*> convertResponseToObjectList(QString input);
	static BucketObject* readBucketObjectFromJson(QString input);
	static QString saveDownloadToDisk(QString& filename, QByteArray source);
};
}
