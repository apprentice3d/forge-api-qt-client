#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStringListModel>
#include "../../src/oss/bucketapi.h"
#include "../../src/oss/bucketobjectapi.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

	
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

	void setupKeysAndAPIs();


private slots:
	void populateBucketView(QList<Forge::Bucket> result, QString error_string) const;
	void selectionChanged(const QModelIndex& index) const;

private:
    Ui::MainWindow *ui;
	Forge::BucketApi m_bucket_manager;
	Forge::BucketObjectApi m_object_manager;
	QStringListModel* m_bucket_list;

	void writeStatusMessage(QString text_content, bool positive = true, qint16 millsec_timeout = 0) const;
};

#endif // MAINWINDOW_H
