#ifndef MAINCODE_H
#define MAINCODE_H

#include <QObject>
#include <QDebug>

class MainCode : public QObject
{
    Q_OBJECT
public:
    explicit MainCode(QObject *parent = nullptr);

signals:

public slots:
    void logIn(QString, QString);

};

#endif // MAINCODE_H