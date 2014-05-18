#include <QtGui/QGuiApplication>
#include <QQmlContext>

#include "qtquick2applicationviewer.h"
#include "GameEngine.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

	qmlRegisterType<MoveableItem>("GameEngine", 1, 0, "MoveableItem");
	qmlRegisterType<ElasticItem>("GameEngine", 1, 0, "ElasticItem");
	qmlRegisterType<MoveableElasticItem>("GameEngine", 1, 0, "MoveableElasticItem");

    QtQuick2ApplicationViewer viewer;
	viewer.rootContext()->setContextProperty("engine", new GameEngine(&viewer));
    viewer.setMainQmlFile(QStringLiteral("qml/BreakOut/main.qml"));
    viewer.showExpanded();

    return app.exec();
}
