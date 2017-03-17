#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QCloseEvent>
#include "../../src/oauth/twoleggedapi.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
	Forge::TwoLeggedApi *m_forge_connector;
	void loadConfigFile();
	void saveConfigFile();
	void prepareTokenTable();

private slots:
	void setSecretsButtonPressed() const;
	void getTokenButtonPressed() const;
	void processNewToken(Forge::Bearer* new_token);

protected:
	void closeEvent(QCloseEvent* event) override;
};

#endif // MAINWINDOW_H
