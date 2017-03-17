#include <QCoreApplication>
#include <QDebug>
#include <QProcessEnvironment>
#include "../../src/oss/bucketapi.h"

QString FORGE_CLIENT_ID = "FORGE_CLIENT_ID";
QString FORGE_CLIENT_SECRET = "FORGE_CLIENT_SECRET";

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    qDebug() << "App started ...";

	Forge::BucketApi bucket_manager;
	Forge::TwoLeggedApi token_manager;


	auto envs = QProcessEnvironment::systemEnvironment();
	auto key_list = envs.keys();

	FORGE_CLIENT_ID = envs.contains(FORGE_CLIENT_ID) ? envs.value(FORGE_CLIENT_ID): FORGE_CLIENT_ID;
	FORGE_CLIENT_SECRET = envs.contains(FORGE_CLIENT_SECRET) ? envs.value(FORGE_CLIENT_SECRET) : FORGE_CLIENT_SECRET;

	
	bucket_manager.addTokenRequester(&token_manager);
	token_manager.set_client_id(FORGE_CLIENT_ID);
	token_manager.set_client_secret(FORGE_CLIENT_SECRET);


	QObject::connect(&bucket_manager, &Forge::BucketApi::getBucketsSignal, [&](QList<Forge::Bucket> result, QString error_string)
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


	QObject::connect(&bucket_manager, &Forge::BucketApi::getBucketDetailsSignal, [](Forge::Bucket bucket, QString error_string)
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


	QObject::connect(&bucket_manager, &Forge::BucketApi::createBucketSignal, [](Forge::Bucket bucket, QString error_string)
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

	QObject::connect(&token_manager, &Forge::TwoLeggedApi::authenticateSignal,
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


	QObject::connect(&bucket_manager, &Forge::BucketApi::listObjectsSignal, [](QList<Forge::BucketObject> object_list, QString error_string)
	{
		if (!error_string.isEmpty())
		{
			qDebug() << "Could not list objects: " << error_string;
		}
		else
		{
			qDebug() << "Received object list: " << object_list.length() << " elements";
			foreach(Forge::BucketObject object, object_list)
			{

				qDebug() << object.get_bucket_key()
					<< " : "
					<< object.get_object_key()
					<< " : "
					<< object.get_object_id()
					<< " : "
					<< object.get_sha1()
					<< " : "
					<< object.get_size()
					<< " : "
					<< object.get_location();

			}
		}
		
	});



	bucket_manager.listObjectsInBucket("model2016-05-02-17-26-51-7wr53d9174tfiichybahdxalon24");
	//		bucket_manager.getBuckets();
	//		bucket_manager.getBucketDetails("f9755023-388c-4b29-814b-9a6760b6");
	//		bucket_manager.createBucket();
	//		token_requester.getTokenWithScope(Forge::setScopes(Forge::DATA::READ));

//	for(int i = 0 ; i < 1; ++i)
//	{
//
//
//	}

	



    return a.exec();
}
