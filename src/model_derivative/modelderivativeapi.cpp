#include "modelderivativeapi.h"


using namespace Forge;

ModelDerivativeApi::ModelDerivativeApi(QObject *parent)
	: BaseApi(parent)
	, m_token_manager(new TwoLeggedApi())
{
	m_endpoint = QString("/modelderivative/v2/designdata");
}

ModelDerivativeApi::~ModelDerivativeApi()
{
	if (m_token_manager != nullptr)
	{
		m_token_manager->deleteLater();
	}
}


void ModelDerivativeApi::addTokenManager(TwoLeggedApi* token_requester)
{
	m_token_manager = token_requester;
	if (m_token_manager->parent() == nullptr)
	{
		m_token_manager->setParent(this);
	}
}




SupportedFormats ModelDerivativeApi::get_cached_formats() const
{
	return m_formats;
}




void ModelDerivativeApi::getFormats(QString for_source)
{
	if(!m_formats.supported_formats.isEmpty() 
		&& QDateTime::currentMSecsSinceEpoch() - this->m_formats.last_time_updated.toSecsSinceEpoch() < refresh_period_in_secs)
	{
		SupportedFormats *result = for_source.isEmpty() ? &m_formats : filterFormatsForSource(for_source);
		emit getFormatsSignal(result, nullptr);
		return;
	}

	QString fullPath = m_host + m_endpoint +  "/formats";
	HttpRequestWorker *worker = new HttpRequestWorker();
	QString token_scope = setScopes(DATA::READ);

	connect(m_token_manager, &TwoLeggedApi::authenticateSignal, [=](Bearer* bearer, QString error)
	{
		if (bearer->get_scope() != token_scope) { return; }
		HttpRequestInput input(fullPath, "GET");
		input.m_headers.insert("Authorization", "Bearer " + bearer->get_access_token());

		connect(worker,&HttpRequestWorker::on_execution_finished, [=](HttpRequestWorker*  requester)
		{
			if(requester->m_error_string.isEmpty())
			{
				updateFormatsFrom(requester->m_response);
				SupportedFormats *result = for_source.isEmpty() ? &m_formats : filterFormatsForSource(for_source);
				emit getFormatsSignal(result, nullptr);
			}
			else
			{
				emit getFormatsSignal(&m_formats, requester->m_error_string);
			}

			requester->deleteLater();
			worker->deleteLater();
		});

		worker->execute(&input);
		bearer->deleteLater();
	});

	m_token_manager->getTokenWithScope(token_scope);
}


SupportedFormats* ModelDerivativeApi::filterFormatsForSource(QString source)
{
	SupportedFormats *result = new SupportedFormats();
	result->last_time_updated = m_formats.last_time_updated;
	result->supported_formats.insert(source, m_formats.supported_formats.value(source));

	return result;
}


void ModelDerivativeApi::updateFormatsFrom(QString source)
{
	m_formats.last_time_updated = QDateTime::currentDateTime();
	QHash<QString, QList<QString>> updated_formats;
	QJsonDocument doc = QJsonDocument::fromJson(source.toUtf8());
	QJsonObject content = doc.object();


	QJsonArray formats = content.value("formats").toArray();
		foreach(const QJsonValue &record, formats)
		{
			QJsonObject format = record.toObject();
			QStringList keys = format.keys();
			foreach(const QString &key, keys)
			{
				QJsonArray model_types = format.value(key).toArray();
				foreach(const QJsonValue &t, model_types)
				{
					QString t_object = t.toString();
					qDebug() << t_object;
				}
			}
		}

//	output->set_bucket_key(content.value("bucketKey").toString());
//	output->set_object_id(content.value("objectId").toString());
//	output->set_object_key(content.value("objectKey").toString());
//	output->set_sha1(content.value("sha1").toString());
//	output->set_size(static_cast<uint64_t>(content.value("size").toDouble()));
//	output->set_content_type(content.value("contentType").toString());
//	output->set_location(content.value("location").toString());

		m_formats.supported_formats = updated_formats;
}
