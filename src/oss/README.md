# Bucket and Object Management

This SDK comes with a <a href="https://developer.autodesk.com/en/docs/data/v2/overview/"
target="_blank">OSS client</a> client that allows you to manage storage in terms of buckets and storage content in form of objects.
 <!--This tutorial uses both 2-legged and 3-legged tokens for calling
different Data Management endpoints.
-->

This part of the SDK is represented by two major clases:
* ***Forge::BucketApi*** - responsible for bucket management:
    * *[Get buckets](#get_buckets)*
    * *[Get bucket details](#get_bucket_details)*
    * *[Create bucket](#create_bucket)*
* ***BucketObjectApi*** - responsible for managing objects within buckets:
    * *[List objects in a bucket](#list_objects)*
    * *[Upload an object to the bucket](#upload_object)*
    * *[Download an object from the bucket](#download_object)*

## Tutorials
Follow this tutorial to see a step-by-step bucket and object management guide, and examples of how to use the Forge APIs.

### Create an App

Create an app on the Forge Developer portal. Note the client ID and
client secret.

Every call to OSS requires a token access with different scopes, depending on performed operation. Thus, both classes have to add an instance of ***Forge::TwoLeggedApi***, that will be responsible for token management and use of tokens with appropriate scope:

```cpp
// create an instance of TwoLeggedApi and set the secrets
Forge::TwoLeggedApi token_manager;
token_manager.set_client_id(FORGE_CLIENT_ID);
token_manager.set_client_secret(FORGE_CLIENT_SECRET);

Forge::BucketApi bucket_manager;
Forge::BucketObjectApi object_manager;
   
bucket_manager.addTokenManager(&token_manager);
object_manager.addTokenManager(&token_manager);
```

#### <a name="get_buckets"></a> Get buckets
```cpp 
// call
void BucketApi::getBuckets(REGION region = REGION::US, qint32 limit = 10, QString start_at = "");
// and stay tuned for 
void BucketApi::getBucketsSignal(QList<Bucket> bucket_list, QString error_string);
```
***Example***:To get the list of available buckets, run the following code:

```cpp
...
#include "oss/bucketapi.h"
...

// create an instance of TwoLeggedAPI, set secrets 
// and add it to the instance of BucketApi
Forge::TwoLeggedApi token_manager;
token_manager.set_client_id(FORGE_CLIENT_ID);
token_manager.set_client_secret(FORGE_CLIENT_SECRET);
    
Forge::BucketApi bucket_manager;
bucket_manager.addTokenManager(&token_manager);


// prepare to receive the list of buckets, 
// by connecting to BucketApi::getBucketsSignal with a needed slot 
// or a lambda function as in this example 
QObject::connect(&bucket_manager, 
                 &Forge::BucketApi::getBucketsSignal, 
                 [&](QList<Forge::Bucket> result, 
                     QString error_string)
{
    if (!error_string.isEmpty())
    {
    	qDebug() << "Could not get buckets: " << error_string;
    }
    else
    {
    	qDebug() << "Received bucket list: " 
                 << result.length() 
                 << " elements";
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

...

// Request the list of buckets, with default US region
bucket_manager.getBuckets();

...

```
In your terminal, you should get a result like this (containing your own data):
```
Received bucket list:  5  elements
"457c728c-1234-4439-9fd3-d3d12158"  :  "Mon Mar 20 11:07:55 2017"  :  "transient"
"c269110b-70d8-1234-9959-94ce8d60"  :  "Fri Mar 3 00:43:24 2017"  :  "transient"
"cd157def-b756-4925-1234-34fd4209"  :  "Fri Mar 3 00:38:46 2017"  :  "transient"
"e5d4e960-d3e6-1234-b8ef-9bfe2e2d"  :  "Fri Mar 3 00:42:07 2017"  :  "transient"
"f9755023-1234-4b29-814b-9a6760b6"  :  "Fri Mar 3 10:12:40 2017"  :  "transient"
```
-------------------------

#### <a name="get_bucket_details"></a>Get bucket details
```cpp
// call
void BucketApi::getBucketDetails(QString bucket_key);
void BucketApi::getBucketDetails(Bucket bucket);
// and stay tuned for 
void BucketApi::getBucketDetailsSignal(Bucket bucket, QString error_string);
```
***Example***:To get the details of a given bucket, run the following code:
```cpp
...
#include "oss/bucketapi.h"
...

// prepare to receive the details of needed bucket, 
// by connecting to BucketApi::getBucketDetailsSignal with a needed slot 
// or a lambda function as in this example 
QObject::connect(&bucket_manager, 
                 &Forge::BucketApi::getBucketDetailsSignal, 
                 [](Forge::Bucket bucket, 
                    QString error_string)
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

...

// Request the details for a given bucket
bucket_manager.getBucketDetails("some-temp-bucket");;

...

```

In your terminal, you should get a result like this (containing your own data):
```
Bucket Details:
"some-temp-bucket"
"F5M2IA7AAQsomerandomSNQYAFAg5T"
1487979947692
"transient"
=====Permisions=====
====================
```

-----------------
#### <a name="create_bucket"></a>Create bucket
```cpp
// call
void BucketApi::createBucket(QString bucket_key = "", 
                             POLICY policy = POLICY::TRANSIENT, 
                             REGION region = REGION::US);
// and stay tuned for 
void BucketApi::createBucketSignal(Bucket bucket, QString error_string);
```

***Example***:To create a bucket, run the following code:

```cpp
...
#include "oss/bucketapi.h"
...


// prepare to receive the confirmation for bucket creation, 
// by connecting to BucketApi::createBucketSignal with a needed slot 
// or a lambda function as in this example 
QObject::connect(&bucket_manager, 
                 &Forge::BucketApi::createBucketSignal, 
                 [](Forge::Bucket bucket, 
                    QString error_string)
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

...

// create bucket with a given key, policy and region.
// when the bucket_key is not provided, an UUID will be generated
// for the name (g.e. 457c728c-1234-4439-9fd3-d3d12158)
bucket_manager.createBucket()

...

```
In your terminal, you should get a result like this (containing your own data):
```
Bucket Details:
"ab1c03a5-d2d6-4592-ae1b-fabfce62"
"F5M2IA7AAQadzLKpS4vEGmSNQYAFAg5T"
"Mon Apr 3 11:35:01 2017"
"transient"
=====Permisions=====
====================
```

---------
#### <a name="list_objects"></a> List objects in a bucket
```cpp
// call
void BucketObjectApi::listObjectsInBucket(QString bucket_key, qint32 limit = 10);
void BucketObjectApi::listObjectsInBucket(Bucket bucket, qint32 limit = 10);
// and stay tuned for 
void BucketObjectApi::listObjectsSignal(QList<BucketObject*> object_list, QString error_string);
```
***Example***:To get the list of object within a bucket, run the following code:

```cpp
...
#include "oss/bucketobjectapi.h"
...

// create an instance of TwoLeggedAPI, set secrets 
// and add it to the instance of BucketApi
Forge::TwoLeggedApi token_manager;
token_manager.set_client_id(FORGE_CLIENT_ID);
token_manager.set_client_secret(FORGE_CLIENT_SECRET);
    
Forge::BucketObjectApi object_manager;
object_manager.addTokenManager(&token_manager);


// prepare to receive the list of objects from given bucket, 
// by connecting to BucketObjectApi::listObjectsSignal with suitable slot 
// or a lambda function as in this example 
QObject::connect(&object_manager, 
                 &Forge::BucketObjectApi::listObjectsSignal, 
                 [](QList<Forge::BucketObject*> object_list, 
                    QString error_string)
{
	if (!error_string.isEmpty())
	{
		qDebug() << "Could not list objects: " << error_string;
	}
	else
	{
		qDebug() << "Received object list: " << object_list.length() << " element(s)";
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

...

// Request the list of object with needed bucket
object_manager.listObjectsInBucket("some-temp-bucket");

...

```
In your terminal, you should get a result like this (containing your own data):
```
Received object list:  1  element(s)
Bucket Key =  "some-temp-bucket"  :
 Object key =  "TestMaxSceneFile"  :
 Object id =  "urn:adsk.objects:os.object:some-temp-bucket/TestMaxSceneFile"  :
 SHA1 =  "0ce58fa8d840f8d1002244bfb447777ae99c94c5"  :
 size =  245760  :
 location =  "https://developer.api.autodesk.com/oss/v2/buckets/some-temp-bucket/objects/TestMaxSceneFile"
```
-------------------------


#### <a name="upload_object"></a> Upload an object into a bucket
```cpp
// call
void BucketObjectApi::uploadObject(QString bucket_key, QString object_name, QString file_path, QString if_match = "");
// and stay tuned for 
void BucketObjectApi::uploadObjectSignal(BucketObject* object, QString error_string);
```
***Example***:To upload an object into a bucket, run the following code:

```cpp
...
#include "oss/bucketobjectapi.h"
...


// prepare to receive the upload results, 
// by connecting to BucketObjectApi::uploadObjectSignal with suitable slot 
// or a lambda function as in this example 
QObject::connect(&object_manager, &Forge::BucketObjectApi::uploadObjectSignal, [](Forge::BucketObject* object, QString error_string)
{
	if (!error_string.isEmpty())
	{
		qDebug() << "Could not upload file: " << error_string;
	}
	else
	{
		qDebug() << "The given object was successfully uploaded:" 
                         << "\nBucket_key: "    << object->get_bucket_key()
                         << "\nObject_key: "    << object->get_object_key()
                         << "\nObject_id: "     << object->get_object_id()
                         << "\nSHA1: "          << object->get_sha1()
                         << "\nSize: "          << object->get_size()
                         << "\nLocation: "      << object->get_location();
	}
});

...

// upload the object to the needed bucket
object_manager.uploadObject("some-temp-bucket",
                            "TestMaxSceneFile", 
                            QFileInfo("C:\\Temp\\test_scene.max").absoluteFilePath());

...

```
In your terminal, you should get a result like this (containing your own data):
```
The given object was successfully uploaded:
Bucket_key:  "denix-temp-bucket"
Object_key:  "TestMaxSceneFile"
Object_id:  "urn:adsk.objects:os.object:denix-temp-bucket/TestMaxSceneFile"
SHA1:  "0ce58fa8d840f8d1002245bfb448779ae98c94c5"
Size:  245760
Location:  "https://developer.api.autodesk.com/oss/v2/buckets/denix-temp-bucket/objects/TestMaxSceneFile"
```
-------------------------


#### <a name="download_object"></a> Download an object from the bucket
```cpp
// call
void BucketObjectApi::downloadObject(QString bucket_key, QString object_name, QString file_name_path);
// and stay tuned for 
void BucketObjectApi::downloadObjectSignal(QString filename_path, QString error_string);
```
***Example***:To download an object from a bucket, run the following code:

```cpp
...
#include "oss/bucketobjectapi.h"
...


// prepare to receive and channel the download results, 
// by connecting to BucketObjectApi::downloadObjectSignal with suitable slot 
// or a lambda function as in this example 
QObject::connect(&object_manager, 
                 &Forge::BucketObjectApi::downloadObjectSignal, 
                 [](QString filename_path, 
                    QString error_string)
{
    if (!error_string.isEmpty())
    {
        qDebug() << "Could not download file: " << error_string;
    }
    else
    {

        qDebug() << "Download completed!\n"
                 << "The needed file can be found at: " 
                 << filename_path;
    }

});

...

// upload the object to the needed bucket
object_manager.downloadObject("denix-temp-bucket", 
                              "TestMaxSceneFile", 
                              "C:\\Temp\\ForExample.max");
...

```
In your terminal, you should get a result like this (containing your own data):
```
Download completed!
The needed file can be found at:  "C:\\Temp\\ForExample.max"
```

-----------------

For an illustration of integrating ***BucketApi*** and ***BucketObjectApi*** in you application,
 please see the [BucketExplorer](../../samples/BucketExplorer) sample.

