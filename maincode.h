#ifndef MAINCODE_H
#define MAINCODE_H

#include <QObject>
#include <QDebug>
#include <QQmlContext>      // Для соединения qml и cpp
#include <QtSql>                    // Для работы с SQLite базой данных
#include <QFileInfo>
#define DEBUG



class MainCode : public QObject
{
    Q_OBJECT
public:
    explicit MainCode(QObject *parent = nullptr);


signals:
    void sendErrorMessage (QString errorMesage);
    void openMainMenu();

public slots:
    void logIn(QString, QString);

};

#endif // MAINCODE_H
