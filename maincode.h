#ifndef MAINCODE_H
#define MAINCODE_H

#include <QObject>
#include <QDebug>
#include <QQmlContext>      // Для соединения qml и cpp
#include <QtSql>                   // Для работы с SQLite базой данных
#include <QFileInfo>
#include <QString>
#include <iostream>
#include <cmath>
using namespace std;


class Worker
{
public:
    QString name;
    QString firstDayDate;
    QString typeOfWorker;
    QString login;
    int id;
    int baseRate;
    int chiefId;
    double salary;
    int workDays = 0;
    int superuser;
};

class Employee : public Worker {
public:
    Employee(int id, QString name, QString firstDayDate, int baseRate, QString typeOfWorker, QString login, int chiefId, int superuser) {
        this-> id = id;
        this->name = name;
        this->firstDayDate = firstDayDate;
        this->baseRate = baseRate;
        this->typeOfWorker = typeOfWorker;
        this->login = login;
        this->chiefId = chiefId;
        this->superuser = superuser;
    }
};

class Manager : public Worker
{
public:
    Manager(int id, QString name, QString firstDayDate, int baseRate, QString typeOfWorker, QString login, int chiefId, int superuser) {
        this-> id = id;
        this->name = name;
        this->firstDayDate = firstDayDate;
        this->baseRate = baseRate;
        this->typeOfWorker = typeOfWorker;
        this->login = login;
        this->chiefId = chiefId;
        this->superuser = superuser;
    }
};

class Sales : public Worker
{
public:
    Sales(int id, QString name, QString firstDayDate, int baseRate, QString typeOfWorker, QString login, int chiefId, int superuser) {
        this-> id = id;
        this->name = name;
        this->firstDayDate = firstDayDate;
        this->baseRate = baseRate;
        this->typeOfWorker = typeOfWorker;
        this->login = login;
        this->chiefId = chiefId;
        this->superuser = superuser;
    }
};


QString CountSalary(QString, QString, QString,  vector<shared_ptr<Worker>>);
QString CountSalaryForAll();

class MainCode : public QObject {
    Q_OBJECT
public:

    int idForQML;
    int levelOfSub = 0;
    bool foundCoincidence;
    QString loginForQML;
    QString nameForQML;
    QString superuserSub;
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


    // Переменные для addWorker
    QString errorMsg;

signals:
    void sendSalaryToQML (QString resultSalary);
    void sendSalaryOfAllWorkersToQML (QString resultSalary);
    void sendErrorMessage (QString errorMesage);
    void openMainMenu(QString nameForQML, QString loginForQML);
    void sendSubordinatesInfoToQML(QString idSub, QString nameSub, QString typeOfWorkerSub, QString firstDayDateSub, QString baseRateSub, QString chiefIdSub, QString levelOfSubStr);                         // Сигнал для передачи информации о подчиненных

    // Сигналы для addWorker
    void sendErrorMessageForName(QString errorMsg);
    void sendErrorMessageForBeginDate(QString errorMsg);
    void sendErrorMessageForType(QString errorMsg);
    void sendErrorMessageForBaseRate(QString errorMsg);
    void sendErrorMessageForLogin(QString errorMsg);
    void sendErrorMessageForFirstPassword(QString errorMsg);
    void sendErrorMessageForSecondPassword(QString errorMsg);
    void sendErrorMessageForChief(QString errorMsg);
    void sendErrorMessageForSuperuser(QString errorMsg);
    void sendCommonErrorMessage(QString errorMsg);



public slots:
    void findSubordinates();
    void logIn(QString, QString);
    void receiveDataFromQMLforCountSalary(QString, QString, QString);
    void receiveDataFromQMLforCountSalaryForAll(QString, QString );
    void addWorker(QString, QString, QString, QString, QString, QString, QString, QString, QString);
};

#endif
