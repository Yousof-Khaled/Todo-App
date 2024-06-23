#include <QGuiApplication>
#include <QQmlApplicationEngine>

// #include "todolistmodel.h"

int main(int argc, char *argv[])
{
	QGuiApplication app(argc, argv);

	// qmlRegisterType<ToDoListModel>("Todo", 1, 0, "ToDoListModel");

	QQmlApplicationEngine engine;
	const QUrl url(QStringLiteral("qrc:/ToDo_app/Main.qml"));
	QObject::connect(&engine, &QQmlApplicationEngine::objectCreationFailed,
					 &app, []() { QCoreApplication::exit(-1); },
	Qt::QueuedConnection);
	engine.load(url);

	return app.exec();
}
