#include <QGuiApplication>
#include <QtQml/QtQml>
#include <QtQuick/QQuickView>
#include <QtQuickControls2>

#include "BaggyGlobal.h"
#include "components/PluginLoader.h"
#include "core/PluginLibrary.h"

int main(int argc, char **argv)
{
    using namespace chip;

//    qRegisterMetaType<PluginInfoList>();

    QGuiApplication app(argc, argv);

    BaggyGlobal baggy;

    qmlRegisterType<PluginLoader>("chip.Baggy", 1, 0, "PluginLoader");

    QQuickStyle::setStyle("Fusion");
    QQmlApplicationEngine eng;
    auto ctx = eng.rootContext();
    ctx->setContextProperty("baggy", &baggy);
    eng.load(QUrl("qrc:/qml/Main.qml"));

    if (eng.rootObjects().isEmpty())
        return -1;

    return QGuiApplication::exec();
}
