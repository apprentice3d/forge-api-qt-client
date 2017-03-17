#include "bucketapi.h"


using namespace Forge;

BucketApi::BucketApi(QObject *parent)
    : BaseApi(parent)
	, m_token_requester(nullptr)
{
	m_endpoint = QString("/oss/v2/buckets");
}


BucketApi::~BucketApi()
{
	if (m_token_requester != nullptr) 
	{ m_token_requester->deleteLater(); }
}


void BucketApi::addTokenRequester(TwoLeggedApi* token_requester)
{
	m_token_requester = token_requester;
	if (m_token_requester->parent() == nullptr) 
	{ m_token_requester->setParent(this); }
}


/*
*	================= BUCKET MANIPULATION PART ===========================
*/


/*
 *	CREATEBUCKET
 */
void BucketApi::createBucket(QString bucket_key, POLICY policy, REGION region)
{
    QString fullPath = m_host + m_endpoint;
    
    HttpRequestWorker *worker = new HttpRequestWorker();
    

	QString bucket_name = bucket_key.isEmpty() ? QUuid::createUuid().toString().mid(1, 32) : bucket_key;

	QString token_scope = setScopes(BUCKET::CREATE);

	connect(worker, &HttpRequestWorker::on_execution_finished, this, &BucketApi::createBucketCallback);
	connect(m_token_requester, &TwoLeggedApi::authenticateSignal, [=](Bearer* bearer, QString error)
	{
		if (bearer->get_scope() != token_scope) { return; }
		HttpRequestInput input(fullPath, "POST");
		input.m_headers.insert("Authorization", "Bearer " + bearer->get_access_token());
		input.m_headers.insert("Content-Type", "application/json");
		input.m_headers.insert("x-ads-region", regions.at(static_cast<int>(region)));


		QJsonObject body_content;
		body_content["bucketKey"] = bucket_name;
		body_content["policyKey"] = policy_categories.at(static_cast<int>(policy));
		QJsonDocument content(body_content);
		input.m_request_body.append(content.toJson());

		worker->execute(&input);
//		bearer->deleteLater();
	});

	m_token_requester->getTokenWithScope(token_scope);

}


void BucketApi::createBucketCallback(HttpRequestWorker* worker)
{
	Bucket output = readBucketFromJson(worker->m_response);

	emit createBucketSignal(output, worker->m_error_string);
	worker->deleteLater();
}


/*
 *	GETBUCKETDETAILS
 */


void BucketApi::getBucketDetails(Bucket bucket)
{
	getBucketDetails(bucket.get_bucket_key());
}

void BucketApi::getBucketDetails(QString bucket_key)
{
	QString fullPath = m_host + m_endpoint + "/" + bucket_key + "/details";

	HttpRequestWorker *worker = new HttpRequestWorker();

	QString token_scope = setScopes(BUCKET::READ);

	connect(worker, &HttpRequestWorker::on_execution_finished, this, &BucketApi::getBucketDetailsCallback);
	connect(m_token_requester, &TwoLeggedApi::authenticateSignal, [=](Bearer* bearer, QString error)
	{
		if (bearer->get_scope() != token_scope) { return; }
		HttpRequestInput input(fullPath, "GET");
		input.m_headers.insert("Authorization", "Bearer " + bearer->get_access_token());
		input.m_headers.insert("Content-Type", "application/json");
		worker->execute(&input);
//		bearer->deleteLater();
	});

	m_token_requester->getTokenWithScope(token_scope);
}


void BucketApi::getBucketDetailsCallback(HttpRequestWorker* worker)
{

	Bucket output = readBucketFromJson(worker->m_response);
	emit getBucketDetailsSignal(output, worker->m_error_string);

	worker->deleteLater();
}


/*
 *	GETBUCKETS
 */
void BucketApi::getBuckets(REGION region, qint32 limit, QString start_at)
{
	QString fullPath = m_host + m_endpoint;

	if (fullPath.indexOf("?") > 0)
		fullPath.append("&");
	else
		fullPath.append("?");
	fullPath.append(QUrl::toPercentEncoding("region"))
		.append("=")
		.append(QUrl::toPercentEncoding(regions.at(int(region))));

	if (fullPath.indexOf("?") > 0)
		fullPath.append("&");
	else
		fullPath.append("?");
	fullPath.append(QUrl::toPercentEncoding("limit"))
		.append("=")
		.append(QUrl::toPercentEncoding(QString::number(limit)));

	if (start_at.length() > 0)
	{
		if (fullPath.indexOf("?") > 0)
			fullPath.append("&");
		else
			fullPath.append("?");
		fullPath.append(QUrl::toPercentEncoding("startAt"))
			.append("=")
			.append(QUrl::toPercentEncoding(start_at));
	}
	

	HttpRequestWorker *worker = new HttpRequestWorker();
	

	connect(worker, &HttpRequestWorker::on_execution_finished, this, &BucketApi::getBucketsCallback);
	
	QString token_scope = setScopes(BUCKET::READ);
	connect(m_token_requester, &TwoLeggedApi::authenticateSignal, [=](Bearer* bearer, QString error)
	{
		if (bearer->get_scope() != token_scope) { return; }
		HttpRequestInput input(fullPath, "GET");
		input.m_headers.insert("Authorization", "Bearer " + bearer->get_access_token());
		worker->execute(&input);
//		bearer->deleteLater();
	});
	
	m_token_requester->getTokenWithScope(token_scope);
	
}


void BucketApi::getBucketsCallback(HttpRequestWorker* worker)
{
	QList<Bucket> output = convertResponseToBucketList(worker->m_response);

	
	emit getBucketsSignal(output, worker->m_error_string);
	worker->deleteLater();
}


QList<Bucket> BucketApi::convertResponseToBucketList(QString input)
{
	QList<Bucket> output;
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
		Bucket item;
		item.set_bucket_key(bucket_object["bucketKey"].toString());
		item.set_creation_date(static_cast<uint64_t>(bucket_object["createdDate"].toDouble()));
		item.set_policy_key(bucket_object["policyKey"].toString());
		output.append(item);
	}


	return output;

}

Bucket BucketApi::readBucketFromJson(QString input)
{
	Bucket output;

	QJsonDocument doc = QJsonDocument::fromJson(input.toUtf8());
	QJsonObject content = doc.object();

	output.set_bucket_key(content.value("bucketKey").toString());
	output.set_bucket_owner(content.value("bucketOwner").toString());
	output.set_creation_date(static_cast<uint64_t>(content.value("createdDate").toDouble()));
	output.set_policy_key(content.value("policyKey").toString());

	QList<BucketPermissions> app_permissions;
	QJsonArray permission_container = content.value("permissions").toArray();
	for (QJsonValue json_value : permission_container)
	{
		BucketPermissions permision_record;
		QJsonObject record = json_value.toObject();

		permision_record.authId = record.value("authId").toString();
		QString access = record.value("access").toString();

		if (access == permission_categories.at(static_cast<int>(ACCESS::READ)))
		{
			permision_record.access = ACCESS::READ;
		}
		if (access == permission_categories.at(static_cast<int>(ACCESS::FULL)))
		{
			permision_record.access = ACCESS::FULL;
		}
		app_permissions.append(permision_record);
	}

	return output;
}



/*
 *	================= OBJECT MANIPULATION PART ===========================
 */




/*
 *	List Objects
 */


void BucketApi::listObjectsInBucket(Bucket bucket, qint32 limit)
{
	listObjectsInBucket(bucket.get_bucket_key());
}

void BucketApi::listObjectsInBucket(QString bucket_key, qint32 limit)
{
	QString fullPath = m_host + m_endpoint + "/" + bucket_key + "/objects";

	fullPath.append("?");
	fullPath.append(QUrl::toPercentEncoding("limit"))
		.append("=")
		.append(QUrl::toPercentEncoding(QString::number(limit)));


	HttpRequestWorker *worker = new HttpRequestWorker();
	QString token_scope = setScopes(DATA::READ);

	connect(worker, &HttpRequestWorker::on_execution_finished, this, &BucketApi::listObjectsCallback);
	connect(m_token_requester, &TwoLeggedApi::authenticateSignal, [=](Bearer* bearer, QString error)
	{
		if (bearer->get_scope() != token_scope) { return; }
		HttpRequestInput input(fullPath, "GET");
		input.m_headers.insert("Authorization", "Bearer " + bearer->get_access_token());
		input.m_headers.insert("Content-Type", "application/json");
		worker->execute(&input);
		//		bearer->deleteLater();
	});

	m_token_requester->getTokenWithScope(token_scope);

}


void BucketApi::listObjectsCallback(HttpRequestWorker* worker)
{
	QList<BucketObject> output = convertResponseToObjectList(worker->m_response);

	emit listObjectsSignal(output, worker->m_error_string);
	worker->deleteLater();
}


QList<BucketObject> BucketApi::convertResponseToObjectList(QString input)
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
