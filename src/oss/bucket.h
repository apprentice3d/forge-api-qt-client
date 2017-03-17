#ifndef BUCKET_H
#define BUCKET_H

#include <QStringList>


namespace Forge {

static QStringList policy_categories = {
		"transient",
		"temporary",
		"persistent",
	};

enum class POLICY {
	TRANSIENT,
	TEMPORARY,
	PERSISTENT
};


static QStringList permission_categories = {
	"full",
	"read"
};

enum class ACCESS
{
	FULL,
	READ
};

struct BucketPermissions
{
	QString authId;
	ACCESS access;

	QString get_auth_id() const
	{
		return authId;
	}

	QString get_access() const
	{
		return permission_categories.at(static_cast<int>(access));
	}





};




class Bucket
{
public:
	explicit Bucket(QString bucket_key = "", 
					QString bucket_owner = "", 
					qint64 created_date = 0, 
					QString policy_key = "transient");

	~Bucket() = default;

	QString get_bucket_key() const;
	QString get_bucket_owner() const;
	qint64 get_creation_date() const;
	QString get_policy_key() const;
	QList<BucketPermissions> getPermissions() const;


	void set_bucket_key(const QString& bucket_key);
	void set_bucket_owner(const QString& bucket_owner);
	void set_creation_date(qint64 created_date);
	void set_policy_key(QString policy_key);
	void set_permissions(QList<BucketPermissions> perm);
private:
	QString m_bucket_key;
	QString m_bucket_owner;
	qint64 m_created_date;
	QList<BucketPermissions> m_permissions;
	QString m_policy_key;
};

	



}

#endif // BUCKET_H
