#include "mainwindow.h"
#include <QApplication>
#include "../../src/oauth/threeleggedapi.h"
#include <QDebug>
#include <QProcessEnvironment>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;




	auto envs = QProcessEnvironment::systemEnvironment();
	auto key_list = envs.keys();

	auto FORGE_CLIENT_ID = envs.contains("FORGE_CLIENT_ID") ? envs.value("FORGE_CLIENT_ID") : "";
	auto FORGE_CLIENT_SECRET = envs.contains("FORGE_CLIENT_SECRET") ? envs.value("FORGE_CLIENT_SECRET") : "";



	Forge::ThreeLeggedApi access_manager;

	access_manager.set_callback_url("http://localhost:3000/cb");
	access_manager.set_authorization_scopes("data:read");
	access_manager.set_client_id(FORGE_CLIENT_ID);
	access_manager.set_client_secret(FORGE_CLIENT_SECRET);


















	QObject::connect(&access_manager,&Forge::ThreeLeggedApi::authorized, [](QString error)
	{
		qDebug() << "Received: " << error;
	});



	access_manager.authorizeWithBrowser();



    w.show();

    return a.exec();
}
