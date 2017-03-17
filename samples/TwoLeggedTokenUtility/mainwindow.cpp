#include "mainwindow.h"
#include "ui_mainwindow.h"

using namespace Forge;


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
	m_forge_connector(nullptr)
{
    ui->setupUi(this);
	loadConfigFile();
	prepareTokenTable();

	m_forge_connector = new Forge::TwoLeggedApi(this);


	connect(ui->set_secrets_btn, &QPushButton::clicked, this, &MainWindow::setSecretsButtonPressed);
	connect(ui->get_token_btn, &QPushButton::clicked, this, &MainWindow::getTokenButtonPressed);
	connect(m_forge_connector, &TwoLeggedApi::authenticateSignal, this, &MainWindow::processNewToken);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::getTokenButtonPressed() const
{
	QString scope = "";
	scope += ui->data_read_check->isChecked() ? setScopes(DATA::READ) : "";
	scope += ui->data_write_check->isChecked() ? setScopes(DATA::WRITE) : "";
	scope += ui->data_create_check->isChecked() ? setScopes(DATA::CREATE) : "";
	scope += ui->data_search_check->isChecked() ? setScopes(DATA::SEARCH) : "";
	scope += ui->bucket_create_check->isChecked() ? setScopes(BUCKET::CREATE) : "";
	scope += ui->bucket_read_check->isChecked() ? setScopes(BUCKET::READ) : "";
	scope += ui->bucket_update_check->isChecked() ? setScopes(BUCKET::UPDATE) : "";
	scope += ui->bucket_delete_check->isChecked() ? setScopes(BUCKET::DELETE) : "";
	scope += ui->account_write_check->isChecked() ? setScopes(ACCOUNT::WRITE) : "";
	scope += ui->account_read_check->isChecked() ? setScopes(ACCOUNT::READ) : "";
	scope += ui->user_profile_check->isChecked() ? setScopes(USER_PROFILE::READ) : "";
	scope += ui->code_all_check->isChecked() ? setScopes(CODE::ALL) : "";

	m_forge_connector->getTokenWithScope(scope);
}

void MainWindow::processNewToken(Bearer* result)
{

	int current_row = ui->token_table->rowCount();
	ui->token_table->insertRow(current_row);
	ui->token_table->setItem(current_row, 0, new QTableWidgetItem(result->get_access_token()));
	ui->token_table->setItem(current_row, 1, new QTableWidgetItem(QString::number(result->get_expiration_time())));
}




void MainWindow::setSecretsButtonPressed() const
{
	m_forge_connector->set_client_id(ui->client_id_input->text());
	m_forge_connector->set_client_secret(ui->client_secret_input->text());
	ui->set_secrets_btn->setEnabled(false);
	ui->get_token_btn->setEnabled(true);
	ui->client_id_input->setEnabled(false);
	ui->client_secret_input->setEnabled(false);
}



void MainWindow::closeEvent(QCloseEvent* event)
{
	saveConfigFile();
	event->accept();	
}

void MainWindow::prepareTokenTable()
{
	ui->token_table->setColumnCount(1);
	ui->token_table->setHorizontalHeaderLabels({ "Access token" });
	ui->token_table->setColumnWidth(0, 100);
}

void MainWindow::loadConfigFile()
{
	QFile load_file("config.json");
	if(!load_file.open(QIODevice::ReadOnly))
	{
		qWarning("Clould not load settings. Setting defaults.");
		return;
	}
	QByteArray content = load_file.readAll();
	QJsonDocument loadDoc(QJsonDocument::fromJson(content));
	QJsonObject settings = loadDoc.object();

	ui->client_id_input->setText(settings["client_id"].toString());
	ui->client_secret_input->setText(settings["client_secret"].toString());
	load_file.close();
}

void MainWindow::saveConfigFile()
{
	QFile save_file("config.json");
	if (!save_file.open(QIODevice::WriteOnly))
	{
		qWarning("Could not save settings");
		return;
	}

	QJsonObject configs;
	configs["client_id"] = m_forge_connector->get_client_id();
	configs["client_secret"] = m_forge_connector->get_client_secret();

	QJsonDocument saveDoc(configs);
	save_file.write(saveDoc.toJson());
	save_file.close();
}


