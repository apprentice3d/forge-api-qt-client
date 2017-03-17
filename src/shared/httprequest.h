#pragma once

#include <QObject>
#include <QMap>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QFileInfo>
#include <QBuffer>

namespace Forge {


enum HttpRequestVarLayout { NOT_SET, ADDRESS, URL_ENCODED, MULTIPART };


// Used to keep structured metadata for file
class HttpRequestInputFileElement {
public:
	QString m_variable_name;
	QString m_local_filename;
	QString m_request_filename;
	QString m_mime_type;

};


// Used to form the content of requests
class HttpRequestInput {

public:
	QString m_url_str;
	QString m_http_method;
	HttpRequestVarLayout m_var_layout;
	QMap<QString, QString> m_vars;
	QMap<QString, QString> m_headers;
	QList<HttpRequestInputFileElement> m_files;
	QByteArray m_request_body;

	HttpRequestInput();
	HttpRequestInput(QString v_url_str, QString v_http_method);
	void initialize();
	void add_var(QString key, QString value);
	void add_file(QString variable_name, QString local_filename, QString request_filename, QString mime_type);

};



class HttpRequestWorker : public QObject
{
	Q_OBJECT

public:
	explicit HttpRequestWorker(QObject *parent = nullptr);
	virtual ~HttpRequestWorker();



	QByteArray m_response;
	QNetworkReply::NetworkError m_error_type;
	QString m_error_string;

	QString http_attribute_encode(QString attribute_name, QString input);
	void execute(HttpRequestInput *input);

signals:
	void on_execution_finished(HttpRequestWorker *worker);

private:
	QNetworkAccessManager *m_manager;

private slots:
	void on_manager_finished(QNetworkReply *reply);


};
}
