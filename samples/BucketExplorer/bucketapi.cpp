#include "bucketapi.h"
using namespace Forge;

BucketApi::BucketApi(QObject *parent)
    : BaseApi(parent)
{
    m_endpoint = QString("/oss/v2/buckets")
}

BucketApi::~BucketApi()
{

}

void BucketApi::createBucket(QString bucket_key, REGION region, POLICY)
{
    QString fullPath = m_host + m_endpoint;

    HttpRequestWorker *worker = new HttpRequestWorker();
    HttpRequestInput input(fullPath, "POST");

}

void BucketApi::getBuckets(REGION region, qint32 limit, QString start_at)
{

}
