#pragma once
#include <QString>
#include <QList>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>


namespace Forge{




class JobOutput
{

public:
	JobOutput(){};
	virtual ~JobOutput(){};

	virtual QJsonObject getAttributes() = 0;
};

class JobPayload
{
public:
	JobPayload(QString input_urn, QList<JobOutput*>, bool compressed_urn = false, QString input_root_filename = "");
	~JobPayload();

	QString m_input_urn;
	QString m_input_root_filename;
	bool m_compressed_urn;

	QList<JobOutput*> m_outputs;



	QJsonDocument createContentAsJson();

};



class SvfJobOutput: public JobOutput
{
public:
	SvfJobOutput(bool make3D = true, bool make2D = false);
	virtual ~SvfJobOutput() = default;


	QJsonObject getAttributes() override;

private:
	bool m_create3D;
	bool m_create2D;
};


}



