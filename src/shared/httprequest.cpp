#include "httprequest.h"

using namespace Forge;

HttpRequestInput::HttpRequestInput()
{
	initialize();
}

HttpRequestInput::HttpRequestInput(QString v_url_str, QString v_http_method)
{
	initialize();
	m_url_str = v_url_str;
	m_http_method = v_http_method;
}

void HttpRequestInput::initialize()
{
	m_var_layout = NOT_SET;
	m_url_str = "";
	m_http_method = "GET";
}

void HttpRequestInput::add_var(QString key, QString value)
{
	m_vars[key] = value;
}

void HttpRequestInput::add_file(QString variable_name, 
								QString local_filename, 
								QString request_filename, 
								QString mime_type)
{
	HttpRequestInputFileElement file;
	file.m_variable_name = variable_name;
	file.m_local_filename = local_filename;
	file.m_request_filename = request_filename;
	file.m_mime_type = mime_type;
	m_files.append(file);
}



// HttpRequestWorker part


HttpRequestWorker::HttpRequestWorker(QObject *parent)
	: QObject(parent)
	, m_manager(nullptr)
{
//	qsrand(QDateTime::currentDateTime().toTime_t());

	m_manager = new QNetworkAccessManager(this);
	connect(m_manager, &QNetworkAccessManager::finished, 
			this, &HttpRequestWorker::on_manager_finished);
}


HttpRequestWorker::~HttpRequestWorker()
{
}

QString HttpRequestWorker::http_attribute_encode(QString attribute_name, QString input)
{
	// result structure follows RFC 5987
	bool need_utf_encoding = false;
	QString result = "";
	QByteArray input_c = input.toLocal8Bit();
	char c;
	for (int i = 0; i < input_c.length(); i++) {
		c = input_c.at(i);
		if (c == '\\' || c == '/' || c == '\0' || c < ' ' || c > '~') {
			// ignore and request utf-8 version
			need_utf_encoding = true;
		}
		else if (c == '"') {
			result += "\\\"";
		}
		else {
			result += c;
		}
	}

	if (result.length() == 0) {
		need_utf_encoding = true;
	}

	if (!need_utf_encoding) {
		// return simple version
		return QString("%1=\"%2\"").arg(attribute_name, result);
	}

	QString result_utf8 = "";
	for (int i = 0; i < input_c.length(); i++) {
		c = input_c.at(i);
		if (
			(c >= '0' && c <= '9')
			|| (c >= 'A' && c <= 'Z')
			|| (c >= 'a' && c <= 'z')
			) {
			result_utf8 += c;
		}
		else {
			result_utf8 += "%" + QString::number(static_cast<unsigned char>(input_c.at(i)), 16).toUpper();
		}
	}

	// return enhanced version with UTF-8 support
	return QString("%1=\"%2\"; %1*=utf-8''%3").arg(attribute_name, result, result_utf8);
}

void HttpRequestWorker::execute(HttpRequestInput* input)
{
	// reset variables

	QByteArray request_content = "";
	m_response = "";
	m_error_type = QNetworkReply::NoError;
	m_error_string = "";
	bool isFormData = false;


	// decide on the variable layout

	if (input->m_files.length() > 0) {
		input->m_var_layout = MULTIPART;
	}
	if (input->m_var_layout == NOT_SET) {
		input->m_var_layout = input->m_http_method == "GET" || input->m_http_method == "HEAD" ? ADDRESS : URL_ENCODED;
	}


	// prepare request content

	QString boundary = "";

	if (input->m_var_layout == ADDRESS || input->m_var_layout == URL_ENCODED) {
		// variable layout is ADDRESS or URL_ENCODED

		if (input->m_vars.count() > 0) {
			bool first = true;
			isFormData = true;
			foreach(QString key, input->m_vars.keys()) {
				if (!first) {
					request_content.append("&");
				}
				first = false;

				request_content.append(QUrl::toPercentEncoding(key));
				request_content.append("=");
				request_content.append(QUrl::toPercentEncoding(input->m_vars.value(key)));
			}

			if (input->m_var_layout == ADDRESS) {
				input->m_url_str += "?" + request_content;
				request_content = "";
			}
		}
	}
	else {
		// variable layout is MULTIPART



		//TODO: Check if the boundary and the delimiter is necessary
		/*
		boundary = "__-----------------------"
			+ QString::number(QDateTime::currentDateTime().toTime_t())
			+ QString::number(qrand());
		QString boundary_delimiter = "--";
		*/

		boundary = "";
		QString boundary_delimiter = "";


		//End of above TODO


		QString new_line = "\r\n";

		// add variables
		foreach(QString key, input->m_vars.keys()) {
			// add boundary
			request_content.append(boundary_delimiter);
			request_content.append(boundary);
			request_content.append(new_line);

			// add header
			request_content.append("Content-Disposition: form-data; ");
			request_content.append(http_attribute_encode("name", key));
			request_content.append(new_line);
			request_content.append("Content-Type: text/plain");
			request_content.append(new_line);

			// add header to body splitter
			request_content.append(new_line);

			// add variable content
			request_content.append(input->m_vars.value(key));
			request_content.append(new_line);
		}

		// add files
		for (QList<HttpRequestInputFileElement>::iterator file_info = input->m_files.begin(); file_info != input->m_files.end(); file_info++) {
			QFileInfo fi(file_info->m_local_filename);

			// ensure necessary variables are available
			if (
				file_info->m_local_filename == nullptr || file_info->m_local_filename.isEmpty()
				|| file_info->m_variable_name == nullptr || file_info->m_variable_name.isEmpty()
				|| !fi.exists() || !fi.isFile() || !fi.isReadable()
				) {
				// silent abort for the current file
				continue;
			}

			QFile file(file_info->m_local_filename);
			if (!file.open(QIODevice::ReadOnly)) {
				// silent abort for the current file
				continue;
			}

			// ensure filename for the request
			if (file_info->m_request_filename == nullptr || file_info->m_request_filename.isEmpty()) {
				file_info->m_request_filename = fi.fileName();
				if (file_info->m_request_filename.isEmpty()) {
					file_info->m_request_filename = "file";
				}
			}

//			// add boundary
//			request_content.append(boundary_delimiter);
//			request_content.append(boundary);
//			request_content.append(new_line);

//			// add header
//			request_content.append(QString("Content-Disposition: form-data; %1; %2").arg(
//				http_attribute_encode("name", file_info->m_variable_name),
//				http_attribute_encode("filename", file_info->m_request_filename)
//			));
//			request_content.append(new_line);
//
//			if (file_info->m_mime_type != nullptr && !file_info->m_mime_type.isEmpty()) {
//				request_content.append("Content-Type: ");
//				request_content.append(file_info->m_mime_type);
//				request_content.append(new_line);
//			}

//			request_content.append("Content-Transfer-Encoding: binary");
//			request_content.append(new_line);
//
//			// add header to body splitter
//			request_content.append(new_line);

			// add file content
			request_content.append(file.readAll());
//			request_content.append(new_line);

			file.close();
		}
//
//		// add end of body
//		request_content.append(boundary_delimiter);
//		request_content.append(boundary);
//		request_content.append(boundary_delimiter);
	}











	if (input->m_request_body.size() > 0) {
		qDebug() << "got a request body";
		request_content.clear();
		request_content.append(input->m_request_body);
	}
	// prepare connection

	QNetworkRequest request = QNetworkRequest(QUrl(input->m_url_str));
	request.setRawHeader("User-Agent", "Forge-Qt-SDK");
	foreach(QString key, input->m_headers.keys()) {
		request.setRawHeader(key.toStdString().c_str(), input->m_headers.value(key).toStdString().c_str());
	}

	if (request_content.size() > 0 && !isFormData) {
		request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
//		request.setHeader(QNetworkRequest::ContentTypeHeader, "application/octet-stream");
	}
	else if (input->m_var_layout == URL_ENCODED) {
		request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
	}
	else if (input->m_var_layout == MULTIPART) {
		request.setHeader(QNetworkRequest::ContentTypeHeader, "multipart/form-data; boundary=" + boundary);
	}

	if (input->m_http_method == "GET") {
		m_manager->get(request);
	}
	else if (input->m_http_method == "POST") {
		m_manager->post(request, request_content);
	}
	else if (input->m_http_method == "PUT") {
		m_manager->put(request, request_content);
	}
	else if (input->m_http_method == "HEAD") {
		m_manager->head(request);
	}
	else if (input->m_http_method == "DELETE") {
		m_manager->deleteResource(request);
	}
	else {
		QBuffer buff(&request_content);
		m_manager->sendCustomRequest(request, input->m_http_method.toLatin1(), &buff);
	}
}

void HttpRequestWorker::on_manager_finished(QNetworkReply* reply)
{
	m_error_type = reply->error();
	if (m_error_type == QNetworkReply::NoError)
	{
		m_response = reply->readAll();
	}
	else
	{
		m_error_string = reply->errorString();
	}

	reply->deleteLater();

	emit on_execution_finished(this);
}
