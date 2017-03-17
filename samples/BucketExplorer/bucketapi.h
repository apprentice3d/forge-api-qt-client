#ifndef BUCKETAPI_H
#define BUCKETAPI_H

#include "../../src/oauth/baseapi.h"
namespace Forge {

enum class REGION {
    US,
    EMEA
};

enum class POLICY {
    TRANSIENT,
    TEMPORARY,
    PERSISTENT
};


class BucketApi : public BaseApi
{
    Q_OBJECT
public:
    BucketApi(QObject *parent = nullptr);
    ~BucketApi();


private:
    void createBucket(QString bucket_key = "", REGION region, POLICY);
    void getBuckets(REGION region, qint32 limit, QString start_at);
//    void deleteBucket(QString* bucket_key);
//    void getBucketDetails(QString* bucket_key);

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
