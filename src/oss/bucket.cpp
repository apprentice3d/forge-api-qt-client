#include "bucket.h"

using namespace Forge;

Bucket::Bucket(QString bucket_key, QString bucket_owner, qint64 created_date, QString policy_key)
	: m_bucket_key(bucket_key)
	, m_bucket_owner(bucket_owner)
	, m_created_date(created_date)
	, m_permissions(QList<BucketPermissions>())
	, m_policy_key(policy_key)
{
}

QString Bucket::get_bucket_key() const
{
	return m_bucket_key;
}

QString Bucket::get_bucket_owner() const
{
	return m_bucket_owner;
}

qint64 Bucket::get_creation_date() const
{
	return m_created_date;
}

QString Bucket::get_policy_key() const
{
	return m_policy_key;
}

QList<BucketPermissions> Bucket::getPermissions() const
{
	return m_permissions;
}

void Bucket::set_bucket_key(const QString& bucket_key)
{
	this->m_bucket_key = bucket_key;
}

void Bucket::set_bucket_owner(const QString& bucket_owner)
{
	this->m_bucket_owner = bucket_owner;
}

void Bucket::set_creation_date(qint64 created_date)
{
	this->m_created_date = created_date;
}

void Bucket::set_policy_key(QString policy_key)
{
	this->m_policy_key = policy_key;
}

void Bucket::set_permissions(QList<BucketPermissions> permisions)
{
	this->m_permissions = permisions;
}

