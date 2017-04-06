#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QProcessEnvironment>
#include <QListView>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
	m_bucket_manager(new Forge::BucketApi(this)),
	m_object_manager(new Forge::BucketObjectApi(this)),
	m_bucket_list(new QStringListModel(this)),
	m_object_list(new QStandardItemModel(this)),
	m_cashed_objects(new QMap<QString, QList<QList<QStandardItem*>>>())
{
    ui->setupUi(this);

	this->setupKeysAndAPIs();
	this->setupObjectTable();

	connect(m_bucket_manager, &Forge::BucketApi::getBucketsSignal, this, &MainWindow::populateBucketView);
	connect(m_object_manager, &Forge::BucketObjectApi::listObjectsSignal, this, &MainWindow::updateObjectTable);
	connect(ui->bucketView, &QListView::clicked, this, &MainWindow::selectionChanged);
//	connect(ui->objectView, &QTableView::dropEvent, this, &MainWindow::uploadDroppedFile);

	m_bucket_manager->getBuckets();
}

MainWindow::~MainWindow()
{
    delete ui;
	//TODO: properly clean the cacheed_objects
	if (m_cashed_objects != nullptr)
	{
		delete m_cashed_objects;
	}
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


	m_bucket_manager->addTokenManager(m_token_manager);
	m_object_manager->addTokenManager(m_token_manager);
}

void MainWindow::setupObjectTable() const
{
	m_object_list->clear();
    m_object_list->setColumnCount(5);
    m_object_list->setHorizontalHeaderLabels({ "Object Key", "Size", "Object ID", "SHA1", "Location" });
	ui->objectView->setModel(m_object_list);
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
		m_object_manager->listObjectsInBucket(bucket.get_bucket_key());
	}
	m_bucket_list->setStringList(buckets);
	ui->bucketView->setModel(m_bucket_list);
	ui->bucketView->reset();
}

void MainWindow::selectionChanged(const QModelIndex& index) const
{
	QString selected_bucket = index.data().toString();
	writeStatusMessage("Bucket \"" + selected_bucket + "\" has " 
						+ QString::number(m_cashed_objects->value(selected_bucket).length()) 
						+ " object(s)");
	m_object_list->setRowCount(0);
	auto object_list = m_cashed_objects->value(selected_bucket);

	for (QList<QStandardItem*> item_description : object_list)
	{
		QList<QStandardItem*> displayed_values;
		for (QStandardItem* item : item_description)
		{
			displayed_values.append(item->clone());
		}
		m_object_list->appendRow(displayed_values);
	}

}

void MainWindow::updateObjectTable(QList<Forge::BucketObject*> object_list, QString error_string) const
{

	if (!error_string.isEmpty())
	{
		writeStatusMessage("Could not list objects: " + error_string, false);
	}
	else
	{
		writeStatusMessage("Received object list with " + QString::number(object_list.length()) + " element(s)", true, 3000);

		QList<QList<QStandardItem*>> items;
		foreach(Forge::BucketObject* object, object_list)
		{
            items.append({	new QStandardItem(object->get_object_key()),
							new QStandardItem(QString::number(object->get_size())),
							new QStandardItem(object->get_object_id()),
							new QStandardItem(object->get_sha1()),
							new QStandardItem(object->get_location()),
			});
		}
		if (!object_list.isEmpty())
		{
			m_cashed_objects->insert(object_list.first()->get_bucket_key(), items);
		}
	}
}

void MainWindow::uploadDroppedFile(QDropEvent* event) const
{
	if(event->mimeData()->hasUrls())
	{
		QUrl url = event->mimeData()->urls().first();
		QString filepath = url.toLocalFile();
		writeStatusMessage("Droped file " + filepath);
	}
	event->acceptProposedAction();
}

void MainWindow::writeStatusMessage(QString text_content, bool positive, qint16 millsec_timeout) const
{
	QPalette palette;
	positive ? palette.setColor(QPalette::WindowText, Qt::darkGreen) 
			 : palette.setColor(QPalette::WindowText, Qt::red);
	ui->statusBar->setPalette(palette);
	ui->statusBar->showMessage(text_content, millsec_timeout);
}

