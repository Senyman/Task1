#include "maincode.h"

MainCode::MainCode(QObject *parent) : QObject(parent)
{

}

void MainCode::logIn(QString login, QString password)
{
    qDebug() << login;
    qDebug() << password;
}
