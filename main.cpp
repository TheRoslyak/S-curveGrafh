#include <QtWidgets/QApplication>
#include <QQmlApplicationEngine>
#include <SCurveTp.h>



int main(int argc, char *argv[])
{

    QApplication  app(argc, argv);
    QQmlApplicationEngine engine;

    qmlRegisterType<SCurveTp>("com.company.scurvetp", 1, 0, "SCurveTp");
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
