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
	Q_INIT_RESOURCE(resource);

	QStringList paths = QCoreApplication::libraryPaths();
	paths.append(".");
	paths.append("imageformats");
	paths.append("platforms");
	paths.append("sqldrivers");
	paths.append(QFileInfo(argv[0]).dir().path() + "/plugins");
	paths.append(QFileInfo(argv[0]).dir().path() + "/plugins/platforms");
	QCoreApplication::setLibraryPaths(paths);
	qDebug() << "paths : " << paths;
	qDebug() << QFileInfo(argv[0]).dir().path();

	QString platformPluginPath = QLatin1String(qgetenv("QT_QPA_PLATFORM_PLUGIN_PATH"));
	qDebug() << "platformPluginPath=" << platformPluginPath;


    QGuiApplication app(argc, argv);


	qmlRegisterType<BonusItem>("GameEngine", 1, 0, "BonusItem");
	qmlRegisterType<MoveableItem>("GameEngine", 1, 0, "MoveableItem");
	qmlRegisterType<ElasticItem>("GameEngine", 1, 0, "ElasticItem");
	qmlRegisterType<MoveableElasticItem>("GameEngine", 1, 0, "MoveableElasticItem");

    QtQuick2ApplicationViewer viewer;
	viewer.rootContext()->setContextProperty("engine", new GameEngine(&viewer));
	viewer.setMainQmlFile(QStringLiteral("/qml/BreakOut/main.qml"));
//	viewer.setSource(QUrl("qrc:/qml/BreakOut/main.qml"));
    viewer.showExpanded();

    return app.exec();
}
