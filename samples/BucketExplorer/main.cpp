#include <QCoreApplication>
#include <QDebug>
#include "../../src/oss/bucketapi.h"


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    qDebug() << "App started ...";

	Forge::BucketApi bucketer;
//	bucketer.set_host(QString("http://localhost:9001"));
	Forge::TwoLeggedApi token_requester;


	bucketer.addTokenRequester(&token_requester);
	token_requester.set_client_id("F5M2IA7AAQadzLKpS4vEGmSNQYAFAg5T");
	token_requester.set_client_secret("none");




	QObject::connect(&bucketer, &Forge::BucketApi::getBucketsSignal, [&](QList<Forge::Bucket> result, QString error_string)
	{
		if (!error_string.isEmpty())
		{
			qDebug() << "Could not get buckets: " << error_string;
		}
		else
		{
			qDebug() << "Received bucket list: " << result.length() << " elements";
			foreach(Forge::Bucket bucket, result)
			{

				qDebug() << bucket.get_bucket_key()
					<< " : "
					<< QDateTime::fromMSecsSinceEpoch(bucket.get_creation_date()).toString()
					<< " : "
					<< bucket.get_policy_key();

			}
		}
		
		
	});

	




	QObject::connect(&bucketer, &Forge::BucketApi::getBucketDetailsSignal, [](Forge::Bucket bucket, QString error_string)
	{
		if (!error_string.isEmpty())
		{
			qDebug() << "Could not get bucket details: " << error_string;
		}
		else
		{
			qDebug() << "Bucket Details:";
			qDebug() << bucket.get_bucket_key();
			qDebug() << bucket.get_bucket_owner();
			qDebug() << bucket.get_creation_date();
			qDebug() << bucket.get_policy_key();
			qDebug() << "=====Permisions=====";
			foreach(Forge::BucketPermissions permission, bucket.getPermissions())
			{
				qDebug() << permission.get_auth_id();
				qDebug() << permission.get_access();
			}
			qDebug() << "====================";

		}

	});




	QObject::connect(&bucketer, &Forge::BucketApi::createBucketSignal, [](Forge::Bucket bucket, QString error_string)
	{
		if (!error_string.isEmpty())
		{
			qDebug() << "Could not create bucket: " << error_string;
		}
		else
		{
			qDebug() << "Bucket Details:";
			qDebug() << bucket.get_bucket_key();
			qDebug() << bucket.get_bucket_owner();
			qDebug() << QDateTime::fromMSecsSinceEpoch(bucket.get_creation_date()).toString();
			qDebug() << bucket.get_policy_key();
			qDebug() << "=====Permisions=====";
			foreach(Forge::BucketPermissions permission, bucket.getPermissions())
			{
				qDebug() << permission.get_auth_id();
				qDebug() << permission.get_access();
			}
			qDebug() << "====================";

		}

	});

	QObject::connect(&token_requester, &Forge::TwoLeggedApi::authenticateSignal,
		[=](Forge::Bearer *result,QString error_string)
	{
		if (!error_string.isEmpty())
		{
			qDebug() << "Could not aquire token: " << error_string;
		}
		else
		{
			qDebug() << "Got token: ";
			qDebug() << result->get_token_type();
			qDebug() << result->get_access_token();
			qDebug() << result->get_expiration_time();
			qDebug() << result->get_refresh_token();
			//		result->deleteLater();
		}
	});

	for(int i = 0 ; i < 1; ++i)
	{
		bucketer.getBuckets();
		bucketer.getBucketDetails("f9755023-388c-4b29-814b-9a6760b6");
		//	bucketer.createBucket();
//		bucketer.getBuckets();
//		token_requester.getTokenWithScope(Forge::setScopes(Forge::DATA::READ));

	}

	



    return a.exec();
}
