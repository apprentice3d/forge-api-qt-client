#include "jobpayload.h"


using namespace Forge;


JobPayload::JobPayload(QString input_urn, QList<JobOutput*> outputs, bool compressed_urn, QString input_root_filename)
	: m_input_urn(input_urn)
	, m_input_root_filename(input_root_filename)
	, m_compressed_urn(compressed_urn)
	, m_outputs(outputs)
{
	
}

JobPayload::~JobPayload()
{
	foreach(JobOutput* output, m_outputs)
	{
		delete output;
	}
}


QJsonDocument JobPayload::createContentAsJson()
{
	QJsonObject body_content;
//	body_content["bucketKey"] = bucket_name;
//	body_content["policyKey"] = policy_categories.at(static_cast<int>(policy));
//	QJsonDocument content(body_content);
//	input.m_request_body.append(content.toJson());

	return QJsonDocument(body_content);

}


SvfJobOutput::SvfJobOutput(bool make3D, bool make2D)
	: m_create3D(make3D)
	, m_create2D(make2D)
{
}


QJsonObject SvfJobOutput::getAttributes()
{
	QJsonObject result;
	QJsonArray views;
	if(m_create2D)
	{
		views << "2d";
	}
	if(m_create3D)
	{
		views << "3d";
	}
	result.insert("type", "svf");
	result.insert("views", views);

	return result;

}
