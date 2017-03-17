#include "bucketobjectapi.h"

using namespace Forge;

BucketObjectApi::BucketObjectApi(QObject *parent)
	: BaseApi(parent)
	, m_token_manager(new TwoLeggedApi())
{
	m_endpoint = QString("/oss/v2/buckets");
}


BucketObjectApi::~BucketObjectApi()
{
	if (m_token_manager != nullptr)
	{
		m_token_manager->deleteLater();
	}
}


void BucketObjectApi::addTokenManager(TwoLeggedApi* token_requester)
{
	m_token_manager = token_requester;
	if (m_token_manager->parent() == nullptr)
	{
		m_token_manager->setParent(this);
	}
}




/*
*	List Objects
*/


void BucketObjectApi::listObjectsInBucket(Bucket bucket, qint32 limit)
{
	listObjectsInBucket(bucket.get_bucket_key(), limit);
}

void BucketObjectApi::listObjectsInBucket(QString bucket_key, qint32 limit)
{
	QString fullPath = m_host + m_endpoint + "/" + bucket_key + "/objects";

	fullPath.append("?");
	fullPath.append(QUrl::toPercentEncoding("limit"))
		.append("=")
		.append(QUrl::toPercentEncoding(QString::number(limit)));


	HttpRequestWorker *worker = new HttpRequestWorker();
	QString token_scope = setScopes(DATA::READ);

	connect(worker, &HttpRequestWorker::on_execution_finished, this, &BucketObjectApi::listObjectsCallback);
	connect(m_token_manager, &TwoLeggedApi::authenticateSignal, [=](Bearer* bearer, QString error)
	{
		if (bearer->get_scope() != token_scope) { return; }
		HttpRequestInput input(fullPath, "GET");
		input.m_headers.insert("Authorization", "Bearer " + bearer->get_access_token());
		input.m_headers.insert("Content-Type", "application/json");
		worker->execute(&input);
		//		bearer->deleteLater();
	});

	m_token_manager->getTokenWithScope(token_scope);

}


void BucketObjectApi::listObjectsCallback(HttpRequestWorker* worker)
{
	QList<BucketObject> output = convertResponseToObjectList(worker->m_response);

	emit listObjectsSignal(output, worker->m_error_string);
	worker->deleteLater();
}


QList<BucketObject> BucketObjectApi::convertResponseToObjectList(QString input)
{

	QList<BucketObject> output;
	QJsonDocument data = QJsonDocument::fromJson(input.toUtf8());
	QJsonObject json_object = data.object();

	if (json_object.contains("items") != true)
	{
		return output;
	}

	QJsonArray bucket_array = json_object["items"].toArray();
	foreach(const QJsonValue &bucket_info, bucket_array)
	{
		QJsonObject bucket_object = bucket_info.toObject();
		BucketObject item;
		item.set_bucket_key(bucket_object["bucketKey"].toString());
		item.set_object_key(bucket_object["objectKey"].toString());
		item.set_object_id(bucket_object["objectId"].toString());
		item.set_sha1(bucket_object["sha1"].toString());
		item.set_size(static_cast<uint64_t>(bucket_object["size"].toDouble()));
		item.set_location(bucket_object["location"].toString());

		//TODO: check if the "next" reference is needed 
		output.append(item);
	}


	return output;




}