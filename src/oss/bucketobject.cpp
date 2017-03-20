#include "bucketobject.h"

using namespace Forge;

BucketObject::BucketObject(QObject *parent)
	: QObject(parent)
	, bucket_key(QString())
	, object_id(QString())
	, object_key(QString())
	, sha1(QString())
	, size(0)
	, content_type(QString())
	, location(QString())
//	, block_sizes(QVector<quint64>())
{
}


//BucketObject::BucketObject(const BucketObject&)
//{
//	//TODO: implement copy contructor to deal with
//	// block_sizes QVector
//}

QString BucketObject::get_bucket_key() const
{
	return bucket_key;
}

void BucketObject::set_bucket_key(const QString& bucket_key)
{
	this->bucket_key = bucket_key;
}

QString BucketObject::get_object_id() const
{
	return object_id;
}

void BucketObject::set_object_id(const QString& object_id)
{
	this->object_id = object_id;
}

QString BucketObject::get_object_key() const
{
	return object_key;
}

void BucketObject::set_object_key(const QString& object_key)
{
	this->object_key = object_key;
}

QString BucketObject::get_sha1() const
{
	return sha1;
}

void BucketObject::set_sha1(const QString& sha1)
{
	this->sha1 = sha1;
}

quint64 BucketObject::get_size() const
{
	return size;
}

void BucketObject::set_size(const quint64 size)
{
	this->size = size;
}

QString BucketObject::get_content_type() const
{
	return content_type;
}

void BucketObject::set_content_type(const QString& content_type)
{
	this->content_type = content_type;
}

QString BucketObject::get_location() const
{
	return location;
}

void BucketObject::set_location(const QString& location)
{
	this->location = location;
}

//QVector<quint64> BucketObject::get_block_sizes() const
//{
//	return block_sizes;
//}
//
//void BucketObject::set_block_sizes(const QVector<quint64>& block_sizes)
//{
//	this->block_sizes = block_sizes;
//}
