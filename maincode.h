#ifndef MAINCODE_H
#define MAINCODE_H

#include <QObject>
#include <QDebug>
#include <QQmlContext>      // Для соединения qml и cpp
#include <QtSql>                   // Для работы с SQLite базой данных
#include <QFileInfo>
#include <iostream>
#include <QString>

using namespace std;

// #include <time.h>
// #include <iomanip>
// #include <conio.h>
//#include <QQmlApplicationEngine>
// #include <windows.h>


class Worker
{
public:
    QString name;
    QString firstDayDate;
    QString typeOfWorker;
    QString login;
    int id;
    int baseRate = 1000;
    int chiefId;
    double salary = 0;
    int workDays = 0;
};

class Employee : public Worker {
public:
    Employee(int id, QString name, QString firstDayDate, int baseRate, QString typeOfWorker, QString login, int chiefId) {
        this-> id = id;
        this->name = name;
        this->firstDayDate = firstDayDate;
        this->baseRate = baseRate;
        this->typeOfWorker = typeOfWorker;
        this->login = login;
        this->chiefId =chiefId;
    }
};

class Manager : public Worker
{
public:
    Manager(int id, QString name, QString firstDayDate, int baseRate, QString typeOfWorker, QString login, int chiefId) {
        this-> id = id;
        this->name = name;
        this->firstDayDate = firstDayDate;
        this->baseRate = baseRate;
        this->typeOfWorker = typeOfWorker;
        this->login = login;
        this->chiefId =chiefId;
    }
};

class Sales :public Worker
{
public:
    Sales(int id, QString name, QString firstDayDate, int baseRate, QString typeOfWorker, QString login, int chiefId) {
        this-> id = id;
        this->name = name;
        this->firstDayDate = firstDayDate;
        this->baseRate = baseRate;
        this->typeOfWorker = typeOfWorker;
        this->login = login;
        this->chiefId =chiefId;
    }
    int numberOfSubordinatesFirst;
    int numberOfSubordinatesSecond;
};

QString CountSalary(QString, QString, QString,  vector<shared_ptr<Worker>>, int);

class MainCode : public QObject
{
    Q_OBJECT
public:

    int idForQML;
    int levelOfSub = 0;
    bool foundCoincidence;
    QString loginForQML;
    QString nameForQML;
    QString idSub;
    QString nameSub;
    QString typeOfWorkerSub;
    QString firstDayDateSub;
    QString baseRateSub;
    QString chiefIdSub;
    QString loginSub;
    QString levelOfSubStr;
    vector<shared_ptr<Worker>> createWorkers();
    explicit MainCode(QObject *parent = nullptr);

signals:
    void sendSalaryToQML (int count);                                                               // Сигнал, для передачи данных в qml-интерфейс
    void sendErrorMessage (QString errorMesage);
    void openMainMenu(QString nameForQML, QString loginForQML);
    void sendSubordinatesInfoToQML(QString idSub, QString nameSub, QString typeOfWorkerSub, QString firstDayDateSub, QString baseRateSub, QString chiefIdSub, QString levelOfSubStr);                         // Сигнал для передачи информации о подчиненных

public slots:
    void findSubordinates();
    void logIn(QString, QString);
    void receiveDataFromQMLforCountSalary(QString, QString, QString);
};

#endif // MAINCODE_H
