#pragma once

#include <QObject>
#include <QVector>

namespace Forge {

class BucketObject : public QObject
{
	Q_OBJECT

public:
	explicit BucketObject(QObject *parent = nullptr);
	~BucketObject() = default;

	BucketObject(const BucketObject &);

	QString get_bucket_key() const;
	void set_bucket_key(const QString& bucket_key);
	QString get_object_id() const;
	void set_object_id(const QString& object_id);
	QString get_object_key() const;
	void set_object_key(const QString& object_key);
	QString get_sha1() const;
	void set_sha1(const QString& sha1);
	quint64 get_size() const;
	void set_size(const quint64 size);
	QString get_content_type() const;
	void set_content_type(const QString& content_type);
	QString get_location() const;
	void set_location(const QString& location);
//	QVector<quint64> get_block_sizes() const;
//	void set_block_sizes(const QVector<quint64>& block_sizes);


private:
	
	QString bucket_key;
	QString object_id;
	QString object_key;
	QString sha1;
	quint64 size;
	QString content_type;
	QString location;
//	QVector<quint64> block_sizes;
	
	//TODO: Review the output and switch to a QMap
	//QVector<BucketObject> deltas;
};
}