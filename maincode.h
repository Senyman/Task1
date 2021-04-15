#ifndef MAINCODE_H
#define MAINCODE_H


#include <QObject>
#include <QDebug>
#include <QQmlContext>      // Для соединения qml и cpp
#include <QtSql>                    // Для работы с SQLite базой данных
#include <QFileInfo>
#include <iostream>
#include <QString>
#define DEBUG

using namespace std;



// #include <time.h>
// #include <iomanip>
// #include <conio.h>
//#include <QQmlApplicationEngine>
// #include <windows.h>

 int CounterWorkDays(QString beginDate, QString endDate);

class MainCode : public QObject
{
    Q_OBJECT
public:
    int count;
    QString loginForQML;
    QString nameForQML;
    explicit MainCode(QObject *parent = nullptr);


signals:
    void sendErrorMessage (QString errorMesage);
    void openMainMenu(QString nameForQML, QString loginForQML);
    void sendSalaryToQML (int count); // Сигнал, для передачи данных в qml-интерфейс

public slots:
    void logIn(QString, QString);
    void ReceiveDataFromQMLforCountSalary(QString, QString, QString);

};

#endif // MAINCODE_H
