#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QProcessEnvironment>



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
	m_bucket_list(new QStringListModel(this))
{
    ui->setupUi(this);

	this->setupKeysAndAPIs();

	connect(&m_bucket_manager, &Forge::BucketApi::getBucketsSignal, this, &MainWindow::populateBucketView);
	connect(ui->bucketView, &QListView::clicked, this, &MainWindow::selectionChanged);


	m_bucket_manager.getBuckets();
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::setupKeysAndAPIs()
{
	auto envs = QProcessEnvironment::systemEnvironment();
	auto key_list = envs.keys();

	auto FORGE_CLIENT_ID = envs.contains("FORGE_CLIENT_ID") ? envs.value("FORGE_CLIENT_ID") : "";
	auto FORGE_CLIENT_SECRET = envs.contains("FORGE_CLIENT_SECRET") ? envs.value("FORGE_CLIENT_SECRET") : "";

	if (FORGE_CLIENT_ID.isEmpty() || FORGE_CLIENT_SECRET.isEmpty())
	{
		writeStatusMessage("WARNING: could not properly set the Client_ID and Client_Secret", false);
	}
	else
	{
		writeStatusMessage("Client_ID and Client_Secret properly set", true, 3000);
	}
	auto m_token_manager = new Forge::TwoLeggedApi(this);

	m_token_manager->set_client_id(FORGE_CLIENT_ID);
	m_token_manager->set_client_secret(FORGE_CLIENT_SECRET);


	m_bucket_manager.addTokenManager(m_token_manager);
	m_object_manager.addTokenManager(m_token_manager);
}

void MainWindow::populateBucketView(QList<Forge::Bucket> result, QString error_string) const
{
	if(!error_string.isEmpty())
	{
		this->writeStatusMessage("ERROR: " + error_string, false);
		return;
	}

	writeStatusMessage("Received a list of " + QString::number(result.length()) + " bucket(s)", true, 5000);

	QStringList buckets;
	for (auto bucket : result)
	{
		buckets << bucket.get_bucket_key();
	}
	m_bucket_list->setStringList(buckets);
	ui->bucketView->setModel(m_bucket_list);
	ui->bucketView->reset();
}

void MainWindow::selectionChanged(const QModelIndex& index) const
{
	//TODO: singnal here for bucket content fetch
	writeStatusMessage("Index moved ..." + QString::number(index.row()) + " = " + index.data().toString());
}

void MainWindow::writeStatusMessage(QString text_content, bool positive, qint16 millsec_timeout) const
{
	QPalette palette;
	positive ? palette.setColor(QPalette::WindowText, Qt::darkGreen) 
			 : palette.setColor(QPalette::WindowText, Qt::red);
	ui->statusBar->setPalette(palette);
	ui->statusBar->showMessage(text_content, millsec_timeout);
}

