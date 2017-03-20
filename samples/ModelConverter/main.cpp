
#include <QCoreApplication>
#include <QDebug>
#include <QThread>
#include <QProcessEnvironment>
#include "../../src/oss/bucketobjectapi.h"
#include "../../src/oss/bucketapi.h"
#include "../../src/model_derivative/modelderivativeapi.h"

QString FORGE_CLIENT_ID = "FORGE_CLIENT_ID";
QString FORGE_CLIENT_SECRET = "FORGE_CLIENT_SECRET";

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    qDebug() << "App started ...";

    Forge::TwoLeggedApi token_manager;
    Forge::BucketApi bucket_manager;
    Forge::BucketObjectApi object_manager;
	Forge::ModelDerivativeApi md_manager;




    auto envs = QProcessEnvironment::systemEnvironment();
    auto key_list = envs.keys();

    FORGE_CLIENT_ID = envs.contains(FORGE_CLIENT_ID) ? envs.value(FORGE_CLIENT_ID): FORGE_CLIENT_ID;
    FORGE_CLIENT_SECRET = envs.contains(FORGE_CLIENT_SECRET) ? envs.value(FORGE_CLIENT_SECRET) : FORGE_CLIENT_SECRET;


    token_manager.set_client_id(FORGE_CLIENT_ID);
    token_manager.set_client_secret(FORGE_CLIENT_SECRET);

    bucket_manager.addTokenManager(&token_manager);
    object_manager.addTokenManager(&token_manager);
	md_manager.addTokenManager(&token_manager);




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


    QObject::connect(&object_manager, &Forge::BucketObjectApi::listObjectsSignal, [](QList<Forge::BucketObject*> object_list, QString error_string)
    {
        if (!error_string.isEmpty())
        {
            qDebug() << "Could not list objects: " << error_string;
        }
        else
        {
            qDebug() << "Received object list: " << object_list.length() << " elements";
            foreach(Forge::BucketObject* object, object_list)
            {

                qDebug() << "Bucket Key = " << object->get_bucket_key()
                    << " : \n"
                    << "Object key = " << object->get_object_key()
                    << " : \n"
                    << "Object id = " << object->get_object_id()
                    << " : \n"
                    << "SHA1 = " << object->get_sha1()
                    << " : \n"
                    << "size = " << object->get_size()
                    << " : \n"
                    << "location = " << object->get_location();

            }
        }

    });

    QObject::connect(&object_manager, &Forge::BucketObjectApi::uploadObjectSignal, [](Forge::BucketObject* object, QString error_string)
    {
        if (!error_string.isEmpty())
        {
            qDebug() << "Could not upload file: " << error_string;
        }
        else
        {

                qDebug() << object->get_bucket_key()
                    << " : "
                    << object->get_object_key()
                    << " : "
                    << object->get_object_id()
                    << " : "
                    << object->get_sha1()
                    << " : "
                    << object->get_size()
                    << " : "
                    << object->get_location();

        }

    });




    QObject::connect(&object_manager, &Forge::BucketObjectApi::downloadObjectSignal, [](QString filename_path, QString error_string)
    {
        if (!error_string.isEmpty())
        {
            qDebug() << "Could not download file: " << error_string;
        }
        else
        {

            qDebug() << "The needed file can be found at: " << filename_path;

        }

    });





//	object_manager.listObjectsInBucket("denix-temp-bucket");

//	object_manager.uploadObject("denix-temp-bucket","TestMaxSceneFile", QFileInfo("C:\\Temp\\test_scene.max").absoluteFilePath());
//	object_manager.downloadObject("denix-temp-bucket", "TestMaxSceneFile", "C:\\Temp\\ForDenix.max");


//			bucket_manager.getBuckets();
//			bucket_manager.getBucketDetails("denix-temp-bucket");
//			bucket_manager.createBucket();
    //		token_requester.getTokenWithScope(Forge::setScopes(Forge::DATA::READ));

//	for(int i = 0 ; i < 1; ++i)
//	{
//
//
//	}


//	auto reference_time = QDateTime::currentDateTime();
//
//	QThread::sleep(10);
//	int interval = QDateTime::currentDateTime().toSecsSinceEpoch() - reference_time.toSecsSinceEpoch();
//
//	if( interval > 9)
//	{
//		qDebug() << "time is up: " << interval << " secs => " << reference_time;
//	}
//	else
//	{
//		qDebug() << "Less than 120 secs";
//	}
//
//

//
//	QHash<QString, QList<QString>> checker_hash;
//	checker_hash.insert("fist", { "one", "primul","pervij" });
//
////	QHash<QString, QList<QString>> result = 
//
//	qDebug() << checker_hash.value("fist");


//
//	QString test_string = "{'formats':{'svf':['3ds','dae'],'thumbnail':['3dm','3ds','asm']}}";
//	QByteArray tester = test_string.replace(QString("\\"), QString()).toUtf8();
//	QJsonDocument doc = QJsonDocument::fromJson(tester);
//	qDebug() << tester;
//	QJsonObject content = doc.object();
//
//	
//
//	qDebug() << tester;
//	QJsonArray formats = content.value("formats").toArray();
//	qDebug() << "Identified " << formats.count() << " formats:";
//	foreach(const QJsonValue &bucket_info, formats)
//	{
//		QJsonObject bucket_object = bucket_info.toObject();
//		QStringList keys = bucket_object.keys();
//		qDebug() << keys;
//	}
//	

	QObject::connect(&md_manager, &Forge::ModelDerivativeApi::getFormatsSignal,[](Forge::SupportedFormats* formats, QString error)
	{
		qDebug() << error;
		qDebug() << "Supported formats: " << formats->supported_formats;
	});


	md_manager.getFormats();



    return a.exec();
}
