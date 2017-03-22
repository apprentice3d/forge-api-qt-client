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
        && QDateTime::currentMSecsSinceEpoch() - this->m_formats.last_time_updated.toTime_t() < refresh_period_in_secs)
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
    QJsonObject formats = doc.object().value("formats").toObject();

    QStringList format_keys = formats.keys();
    foreach(const QString &key, format_keys)
    {
        QJsonArray model_types = formats.value(key).toArray();
        foreach(const QJsonValue &t, model_types)
        {
            QString t_object = t.toString();
            if (updated_formats.contains(t_object))
            {
                updated_formats[t_object].append(key);
            }
            else
            {
                QList<QString> convertible_to;
                convertible_to.append(key);
                updated_formats.insert(t_object, convertible_to);
            }

        }
    }

		m_formats.supported_formats = updated_formats;
}
