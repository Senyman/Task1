#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "maincode.h"

int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;                               // Создание qml движка


    MainCode maincode;
    QQmlContext *context = engine.rootContext();                    // Для соединения main.qml и maincode.h/cpp
    context->setContextProperty("MainCode", & maincode);





    const QUrl url(QStringLiteral("qrc:/main.qml"));    // В движок загружается корневой элемент main.qml
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated, &app, [url](QObject *obj, const QUrl &objUrl) {  if (!obj && url == objUrl) QCoreApplication::exit(-1); }, Qt::QueuedConnection);
    engine.load(url);
    return app.exec();
}
