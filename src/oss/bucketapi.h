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

	void addTokenManager(TwoLeggedApi* token_requester);

	void createBucket(QString bucket_key = "", POLICY policy = POLICY::TRANSIENT, REGION region = REGION::US);
	void getBuckets(REGION region = REGION::US, qint32 limit = 10, QString start_at = "");
	void getBucketDetails(QString bucket_key);
	void getBucketDetails(Bucket bucket);

	//TODO: Check if deleting buckets is possible
	//void deleteBucket(QString bucket_key);



signals:
	void getBucketsSignal(QList<Bucket> bucket_list, QString error_string);
	void getBucketDetailsSignal(Bucket bucket, QString error_string);
	void createBucketSignal(Bucket bucket, QString error_string);

private:
	TwoLeggedApi *m_token_manager;

	void getBucketsCallback(HttpRequestWorker* worker);
	void getBucketDetailsCallback(HttpRequestWorker* worker);
	void createBucketCallback(HttpRequestWorker* worker);


	//Auxiliary methods
	static QList<Bucket> convertResponseToBucketList(QString result);
	static Bucket readBucketFromJson(QString input);



};
}
#endif // BUCKETAPI_H
