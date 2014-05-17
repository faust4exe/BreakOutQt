#include <QtGui/QGuiApplication>
#include <QQmlContext>

#include "qtquick2applicationviewer.h"
#include "GameEngine.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QtQuick2ApplicationViewer viewer;
	viewer.rootContext()->setContextProperty("engine", new GameEngine(&viewer));
    viewer.setMainQmlFile(QStringLiteral("qml/BreakOut/main.qml"));
    viewer.showExpanded();

    return app.exec();
}
