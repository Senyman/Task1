#include "maincode.h"

MainCode::MainCode(QObject *parent) : QObject(parent)
{

}

void MainCode::logIn(QString login, QString password)
{
        int count = 0;
        QSqlDatabase workersDB = QSqlDatabase::addDatabase("QSQLITE");  // Создаем объект для работы с базой данных
        workersDB.setDatabaseName("C://Users//User//Desktop//WorkersDB//Workers.db");   // Указываю путь к базе данных
        if(!workersDB.open()) qDebug() << "Failed to open database";
        else qDebug() << "DataBase is connected";

    QSqlQuery qry;            // Объект для запроса информации из БД
    if(qry.exec("SELECT * FROM WorkersTable WHERE Login = '"+login+"' and Password = '"+password+"' "))
    {
        while(qry.next()) count++;
        if(count==1) {

#ifdef DEBUG
            qDebug() << "Your login and password are correct ";
            qDebug() << login;  // Отладочная
            qDebug() << password;
#endif
        }
        if(count<1)  {
            qDebug() <<"Your login or password isn't correct";

        }
    }

    else {
        qDebug() << "Your login or password isn't correct ";
    }
}
