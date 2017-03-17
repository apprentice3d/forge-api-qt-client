#ifndef BUCKETAPI_H
#define BUCKETAPI_H


#include "../shared/baseapi.h"
#include "../oauth/twoleggedapi.h"
#include "bucket.h"
#include <QJsonArray>
#include <QUuid>

namespace Forge {

static QStringList regions = {
	"US",
	"EMEA",
};

enum class REGION {
	US,
	EMEA
};

class BucketApi : public BaseApi
{
	Q_OBJECT
public:
	explicit BucketApi(QObject *parent = nullptr);
	~BucketApi();
	void createBucket(QString bucket_key = "", POLICY policy = POLICY::TRANSIENT, REGION region = REGION::US);
	void getBuckets(REGION region = REGION::US, qint32 limit = 10, QString start_at = "");
	//    void deleteBucket(QString bucket_key);
	void getBucketDetails(QString bucket_key);

	void getBucketDetails(Bucket bucket){ getBucketDetails(bucket.get_bucket_key()); };

	void addTokenRequester(TwoLeggedApi* token_requester);

signals:
	//    void createBucketSignal(SWGBucket* summary);
	//    void deleteBucketSignal();
	//    void getBucketDetailsSignal(SWGBucket* summary);
	void getBucketsSignal(QList<Bucket> bucket_list, QString error_string);
	void getBucketDetailsSignal(Bucket bucket, QString error_string);
	void createBucketSignal(Bucket bucket, QString error_string);


private:
	TwoLeggedApi *m_token_requester;

	void getBucketsCallback(HttpRequestWorker* worker);
	void getBucketDetailsCallback(HttpRequestWorker* worker);
	void createBucketCallback(HttpRequestWorker* worker);

	//Auxiliary methods
	static QList<Bucket> convertResponseToBucketList(QString result);
	static Bucket readBucketFromJson(QString input);



};
}
#endif // BUCKETAPI_H




//#ifndef BUCKETAPI_H
//#define BUCKETAPI_H

//#include <QObject>

//namespace Forge {
//class BucketApi : public QObject
//{
//    Q_OBJECT
//public:
//    explicit BucketApi(QObject *parent = nullptr);

//    QString get_host() const;
//    void set_host(const QString& host);
//    QString get_oss_endpoint() const;
//    void set_oss_endpoint(const QString& endpoint);

//signals:

//public slots:



//protected:
//    QString m_host;
//    QString m_endpoint;


//};
//}


//#endif // BUCKETAPI_H


//class SWGBucketsApi: public QObject {
//    Q_OBJECT

//public:
//    SWGBucketsApi();
//    SWGBucketsApi(QString host, QString basePath);
//    ~SWGBucketsApi();

//    QString host;
//    QString basePath;

//    void createBucket(SWGPost_buckets_payload post_buckets, QString* x_ads_region);
//    void deleteBucket(QString* bucket_key);
//    void getBucketDetails(QString* bucket_key);
//    void getBuckets(QString* region, qint32 limit, QString* start_at);

//private:
//    void createBucketCallback (HttpRequestWorker * worker);
//    void deleteBucketCallback (HttpRequestWorker * worker);
//    void getBucketDetailsCallback (HttpRequestWorker * worker);
//    void getBucketsCallback (HttpRequestWorker * worker);

//signals:
//    void createBucketSignal(SWGBucket* summary);
//    void deleteBucketSignal();
//    void getBucketDetailsSignal(SWGBucket* summary);
//    void getBucketsSignal(SWGInline_response_200* summary);

//};
