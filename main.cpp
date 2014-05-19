#include <QtGui/QGuiApplication>
#include <QQmlContext>
#include <QFileInfo>
#include <QDir>

#include "qtquick2applicationviewer.h"
#include "GameEngine.h"
#include <MoveableItem.h>
#include <ElasticItem.h>
#include <BonusItem.h>
#include <MoveableElasticItem.h>

int main(int argc, char *argv[])
{
	QCoreApplication::addLibraryPath(QFileInfo(argv[0]).dir().path());

    QGuiApplication app(argc, argv);

	Q_INIT_RESOURCE(resource);

	qmlRegisterType<BonusItem>("GameEngine", 1, 0, "BonusItem");
	qmlRegisterType<MoveableItem>("GameEngine", 1, 0, "MoveableItem");
	qmlRegisterType<ElasticItem>("GameEngine", 1, 0, "ElasticItem");
	qmlRegisterType<MoveableElasticItem>("GameEngine", 1, 0, "MoveableElasticItem");

    QtQuick2ApplicationViewer viewer;
	viewer.rootContext()->setContextProperty("engine", new GameEngine(&viewer));
	viewer.setSource(QUrl("qrc:/qml/BreakOut/main.qml"));
    viewer.showExpanded();

    return app.exec();
}
