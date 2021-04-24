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

 int CounterWorkDays(QString beginDate, QString endDate);
 int CounterWorkYears(QString beginDate, QString endDate);


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

    double years;        // Изменить это !!!!! В данный момент это количество лет, проведенных на работе в этой фирме

    virtual double CountSalary(double years, double baseRate, QString beginDate, QString endDate) { return 666; }
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

    double CountSalary(double years, double baseRate, QString beginDate, QString endDate) {
        workDays = CounterWorkDays(beginDate, endDate);
        double salary;
        this->baseRate = baseRate;
        this->years = years;
        if (years>=10)
        {
            salary = baseRate * workDays + baseRate * workDays * 0.3;
        }
        else
        {
            salary = baseRate* workDays + baseRate * workDays * years * 0.05;
        }
         qDebug() << "Зарплата рабочего равна: " << salary << " рублей";
        return salary;
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
    int numberOfSubordinatesFirst = 2;     // Количество подчиненных первого уровня !!!!
    //  double CountSalary(double years, double baseRate, int numberOfSubordinatesFirst, QString beginDate, QString endDate) // убрал одну переменную ради эксперимента

    double CountSalary(double years, double baseRate, QString beginDate, QString endDate)        // Количество подчиненных первого уровня
    {
        double salary;
        this->baseRate = baseRate;
        this->years = years;
        this->numberOfSubordinatesFirst = numberOfSubordinatesFirst;
        if (years >= 8)
        {
            salary = baseRate * CounterWorkDays(beginDate, endDate) + baseRate * CounterWorkDays(beginDate, endDate) * 0.4 + baseRate * CounterWorkDays(beginDate, endDate) *0.005* numberOfSubordinatesFirst;
        }
        else
        {
            salary = baseRate * CounterWorkDays(beginDate, endDate) + baseRate* CounterWorkDays(beginDate, endDate) * years * 0.05+0.005 * numberOfSubordinatesFirst * baseRate;
        }
        return salary;
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
    int numberOfSubordinatesFirst;     // Количество подчиненных первого уровня
    int numberOfSubordinatesSecond;
    double CountSalary(double years, double baseRate, int numberOfSubordinates, int numberOfSubordinatesSecond, QString beginDate, QString endDate)        // Количество всех подчиненных
    {
        double salary;
        this->baseRate = baseRate;
        this->years = years;
        this->numberOfSubordinatesFirst = numberOfSubordinatesFirst;
        this->numberOfSubordinatesSecond = numberOfSubordinatesSecond;
        if (years >= 8)
        {
            salary = baseRate* CounterWorkDays(beginDate, endDate) + baseRate * CounterWorkDays(beginDate, endDate) * 0.4 + baseRate* CounterWorkDays(beginDate, endDate) * 0.005 * (numberOfSubordinates+ numberOfSubordinatesSecond);
        }
        else
        {
            salary = baseRate * CounterWorkDays(beginDate, endDate) + baseRate * CounterWorkDays(beginDate, endDate) * years * 0.05 + 0.005 * (numberOfSubordinates + numberOfSubordinatesSecond);
        }
        return salary;
    }
};



double CountSalary2(QString, QString, QString,  vector<shared_ptr<Worker>>, int);

class MainCode : public QObject
{
    Q_OBJECT
public:
    bool foundCoincidence;
    int idForQML;
    QString loginForQML;
    QString nameForQML;
    explicit MainCode(QObject *parent = nullptr);
    vector<shared_ptr<Worker>> createWorkers();

    QString idSub;
    QString nameSub;
    QString typeOfWorkerSub;
    QString firstDayDateSub;
    QString baseRateSub;
    QString chiefIdSub;
    QString loginSub;
    int levelOfSub = 0;
    QString levelOfSubStr;

signals:
    void sendErrorMessage (QString errorMesage);
    void openMainMenu(QString nameForQML, QString loginForQML);
    void sendSalaryToQML (int count);                                                               // Сигнал, для передачи данных в qml-интерфейс
    void sendSubordinatesInfoToQML(QString idSub, QString nameSub, QString typeOfWorkerSub, QString firstDayDateSub, QString baseRateSub, QString chiefIdSub, QString levelOfSubStr);                         // Сигнал для передачи информации о подчиненных

public slots:
    void logIn(QString, QString);
    void receiveDataFromQMLforCountSalary(QString, QString, QString);
    void findSubordinates();


};

#endif // MAINCODE_H
