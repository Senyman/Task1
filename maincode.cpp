#include "maincode.h"

MainCode::MainCode(QObject *parent) : QObject(parent) {}

void MainCode::logIn(QString login, QString password) {                                                            // Вход по логину и паролю
    {
        foundCoincidence = false;
        QSqlDatabase workersDB = QSqlDatabase::addDatabase("QSQLITE");                             // Создаем объект для работы с базой данных
        workersDB.setDatabaseName("C://Users//User//Desktop//WorkersDB//Workers.db");   // Указываю путь к базе данных
        if(!workersDB.open()) qDebug() << "Failed to open database";
        else qDebug() << "DataBase is connected";
        QSqlQuery qry;                                                                                                                        // Объект для запроса информации из БД

        if(qry.exec("SELECT * FROM WorkersTable WHERE Login = '"+login+"' and Password = '"+password+"' ")) {
                while(qry.next()) {
                    idForQML = qry.value(0).toInt();
                    nameForQML = qry.value(1).toString();
                    loginForQML = qry.value(5).toString();

                    emit  openMainMenu(nameForQML, loginForQML);
                    foundCoincidence = true;
                    qDebug() << "Такой логин и пароль существуют";
                    emit  sendErrorMessage(" ");
                }

                if(!foundCoincidence)  {
                    qDebug() <<"Некорректный логин или пароль";
                    emit sendErrorMessage("Неправильно введен логин или пароль");
                }
        }
        else {
            qDebug() << "Некорректный логин или пароль";
        }
        workersDB.close();
    }
    QSqlDatabase::removeDatabase("qt_sql_default_connection");
}

void MainCode::receiveDataFromQMLforCountSalary(QString beginDate, QString endDate, QString id)
{
    vector<shared_ptr<Worker>> workers = createWorkers();
    emit sendSalaryToQML(CountSalary(beginDate, endDate, id, workers));
}

void MainCode::receiveDataFromQMLforCountSalaryForAll(QString beginDate, QString endDate)
{
    QString currentSalary;
    double currentSalarydouble = 0;
    int discharge = 0;
    int forNumericOfSalaryBeforePoint = 0;
    bool foundPoint = false;
    bool haveAccess = false;
    vector<shared_ptr<Worker>> workers = createWorkers();
    vector< int> numericOfSalaryBeforePoint;
    vector<double> numericOfSalaryAfterPoint;

    for( auto &a: workers) {
        if(idForQML == a->id) {
            if(a->superuser == 1) {
                haveAccess = true;
                for(auto &a: workers) {
                    foundPoint = false;
                    currentSalary = CountSalary(beginDate, endDate, QString::number(a->id), workers);
                    forNumericOfSalaryBeforePoint = 0;


                    for(int i =0; i < currentSalary.size(); i++) {

                        if( !foundPoint && currentSalary[i].isDigit()) {
                            numericOfSalaryBeforePoint.push_back(currentSalary[i].unicode() - 48);
                        }
                        else if (foundPoint && currentSalary[i].isDigit()) {
                            numericOfSalaryAfterPoint.push_back(currentSalary[i].unicode() - 48);
                        }

                        if(currentSalary[i] == ".") foundPoint = true;
                    }
                    for(unsigned long long i = numericOfSalaryBeforePoint.size(); i > 0; i--) {
                        discharge = pow(10, i-1);
                        currentSalarydouble += numericOfSalaryBeforePoint[forNumericOfSalaryBeforePoint]*discharge;
                        forNumericOfSalaryBeforePoint ++;
                    }

                    for(unsigned long long i = 0; i < numericOfSalaryAfterPoint.size(); i++) {
                        discharge = pow(10, i+1);
                        currentSalarydouble +=  numericOfSalaryAfterPoint[i] / discharge;
                    }
                    numericOfSalaryBeforePoint.clear();
                    numericOfSalaryAfterPoint.clear();
                }
            }
        }
    }
    currentSalarydouble *=10;
    currentSalarydouble = floor(currentSalarydouble) /10;

    if(haveAccess) emit sendSalaryOfAllWorkersToQML ("Зарплата всех работников равна: " + QString::number(currentSalarydouble, 'g', 8) + "руб.");
    else emit sendSalaryOfAllWorkersToQML ("У вас нет прав доступа");
}

void MainCode::findWorkersAndSubordinates() {

    bool badEnter = true;
    vector<shared_ptr<Worker>> workers = createWorkers();
    for(auto &a:workers) {
        if(a->login == loginForQML && a->superuser == 1) {
            emit openAddWorkerPage();
            badEnter = false;
            createWorkers();
        }
    }
    if(badEnter) emit doesNotHavePermissionToOpenPage();
}

void MainCode::findWorkersAndSubordinatesForSubPage()
{
    createWorkers();
}

vector<shared_ptr<Worker>> MainCode::createWorkers() {

    int chiefsCounter;
    int level = 0;
    unsigned long long sizeArr;

    int idForArr;                                                    // Переменные для занесения в массив с работниками, меняются для каждого работника
    int chiefIdForArr;
    int baseSalaryForArr;
    int superuserForArr;
    QString nameForArr;
    QString firstDayDateForArr;
    QString typeOfWorkerForArr;
    QString loginForArr;

    vector<int> chiefsid;                                                                 // Массив для расчета уровней подчиненных
    vector<shared_ptr<Worker>> workers;                                    // Массив со всеми работниками из БД
    vector<shared_ptr<Worker>> UserAndSubordinateWorkers;  // Массив с залогинившимся работником и всеми его подчиненными из БД

    chiefsid.push_back(idForQML);

   {
        // Заносим в массив workers всех людей с базы данных
       QSqlDatabase workersDB = QSqlDatabase::addDatabase("QSQLITE");
       workersDB.setDatabaseName("C://Users//User//Desktop//WorkersDB//Workers.db");
       if(!workersDB.open()) qDebug() << "Failed to open database";
       else qDebug() << "DataBase is connected";
       QSqlQuery qry;

       if(qry.exec("SELECT * FROM WorkersTable ")) {
           while(qry.next()) {
               {
                   idForArr = qry.value(0).toInt();
                   nameForArr = qry.value(1).toString();
                   firstDayDateForArr = qry.value(2).toString();
                   baseSalaryForArr = qry.value(3).toInt();
                   typeOfWorkerForArr = qry.value(4).toString();
                   loginForArr = qry.value(5).toString();
                   chiefIdForArr = qry.value(7).toInt();
                   superuserForArr = qry.value(8).toInt();

                   if(typeOfWorkerForArr == "Employee") workers.push_back(make_shared<Employee>( idForArr, nameForArr, firstDayDateForArr, baseSalaryForArr, typeOfWorkerForArr, loginForArr, chiefIdForArr, superuserForArr));
                   else if (typeOfWorkerForArr == "Sales") workers.push_back(make_shared<Sales>( idForArr, nameForArr, firstDayDateForArr, baseSalaryForArr, typeOfWorkerForArr, loginForArr, chiefIdForArr, superuserForArr));
                   else if(typeOfWorkerForArr == "Manager") workers.push_back(make_shared<Manager>( idForArr, nameForArr, firstDayDateForArr, baseSalaryForArr, typeOfWorkerForArr, loginForArr, chiefIdForArr, superuserForArr));

                   qDebug() << qry.value(0).toString();
                   emit sendWorkersInfoToQML(qry.value(0).toString() + " " + nameForArr);
               }
           }
           workersDB.close();
       }
    }
    QSqlDatabase::removeDatabase("qt_sql_default_connection");

    // Добавляем залогинившегося работника
    for(unsigned long long j = 0; j < workers.size(); j++) {
        if(nameForQML == workers[j]->name) {
            if(typeOfWorkerForArr == "Employee") UserAndSubordinateWorkers.push_back(make_shared<Employee>( workers[j]->id, nameForQML, workers[j]->firstDayDate, workers[j]->baseRate, workers[j]->typeOfWorker, workers[j]->login, workers[j]->chiefId, workers[j]->superuser));
            else if (typeOfWorkerForArr == "Sales") UserAndSubordinateWorkers.push_back(make_shared<Sales>( workers[j]->id, nameForQML, workers[j]->firstDayDate, workers[j]->baseRate, workers[j]->typeOfWorker, workers[j]->login, workers[j]->chiefId, workers[j]->superuser));
            else if(typeOfWorkerForArr == "Manager") UserAndSubordinateWorkers.push_back(make_shared<Manager>( workers[j]->id, nameForQML, workers[j]->firstDayDate, workers[j]->baseRate, workers[j]->typeOfWorker, workers[j]->login, workers[j]->chiefId, workers[j]->superuser));
        }
    }

    // Добавляем подчиненных залогинившегося работника и рассчитываем уровень подчиненнго для каждого работника
    while(true) {
        chiefsCounter = 0;
        sizeArr = chiefsid.size();

        for(unsigned long long i = 0; i <chiefsid.size()-chiefsCounter; i++ ) {     // Для каждого эл-та в chiefsid arr ( вначале там только id одного человека, под  логином и паролем которого я зашел)
            for (unsigned long long j = 0; j < workers.size(); j++) {                      // Для каждого работника, которого я добавил из базы данных
                if(chiefsid[i] == workers[j]->chiefId) {                                            // Если id начальника совпадает с полем Chief у подчиненного

                    idSub = QString::number(workers[j]->id);                                 // Записываем его данные, что бы отобразить далее на экране
                    nameSub = workers[j]->name;
                    typeOfWorkerSub = workers[j]->typeOfWorker;
                    firstDayDateSub =  workers[j]->firstDayDate;
                    baseRateSub = QString::number(workers[j]->baseRate);
                    chiefIdSub = QString::number(workers[j]->chiefId);
                    loginSub = workers[j]->login;
                    superuserSub = QString::number(workers[j]->superuser);

                    for(auto &k: chiefsid) {
                        if(workers[j]->chiefId == k && chiefsid.size() == sizeArr) {
                        level ++;
                        }
                    }
                    chiefsid.push_back(workers[j]->id);                                             // Добавляем id подчиненного, делаем его тоже шефом, только он теперь ниже по уровню
                    emit sendSubordinatesInfoToQML(idSub, nameSub, typeOfWorkerSub, firstDayDateSub, baseRateSub, chiefIdSub, QString::number(level));               // Отправляем данные в QML для отображения в ListModel

                    if(typeOfWorkerForArr == "Employee") UserAndSubordinateWorkers.push_back(make_shared<Employee>( workers[j]->id, nameSub, firstDayDateSub, workers[j]->baseRate, typeOfWorkerSub, loginSub, workers[j]->chiefId, workers[j]->superuser));
                    else if (typeOfWorkerForArr == "Sales") UserAndSubordinateWorkers.push_back(make_shared<Sales>( workers[j]->id, nameSub, firstDayDateSub, workers[j]->baseRate, typeOfWorkerSub, loginSub, workers[j]->chiefId, workers[j]->superuser));
                    else if(typeOfWorkerForArr == "Manager") UserAndSubordinateWorkers.push_back(make_shared<Manager>( workers[j]->id, nameSub, firstDayDateSub, workers[j]->baseRate, typeOfWorkerSub, loginSub, workers[j]->chiefId, workers[j]->superuser));
                    chiefsCounter++;
                }
            }
        }

        unsigned long long previousSize = chiefsid.size();
        vector<int>::iterator chiefsIterator = chiefsid.begin();

        for (unsigned long long i =0; i < previousSize - chiefsCounter; i++) {
            chiefsid.erase(chiefsIterator);
            chiefsIterator = chiefsid.begin();
        }

        if(chiefsCounter == 0) break;
   }
    // Если залогинился суперпользователь, то ему доступны все сотрудники
     for(auto &a: workers) {
         if(a->login == loginForQML && a->superuser ==1) return workers;
     }
    return UserAndSubordinateWorkers;
}

QString CountSalary(QString beginDate, QString endDate, QString idFromQML, vector<shared_ptr<Worker>> workers) {
    double salary = 0;
    bool badenter = true;
    int* mHolidays;
    for( auto &a: workers) {
       if((idFromQML).toInt()  == a->id) {

           badenter = false;

           int dayBegin, monthBegin, yearBegin;
           int dayEnd, monthEnd, yearEnd;
           int firstDayDay, firstDayMonth, firstDayYear;
           int mDays[12] = { 31,28,31,30,31,30,31,31,30,31,30,31 };
           int mainChiefId = a->id;


           // Проверяем введенные даты начальной и конечной и перевод их в int
           if (beginDate.length() != 10 || beginDate[2] != '.' || beginDate[5] != '.' || beginDate[1] == 0 || !beginDate[0].isDigit() || !beginDate[1].isDigit() || !beginDate[3].isDigit() || !beginDate[4].isDigit()
               || !beginDate[6].isDigit() || !beginDate[7].isDigit() || !beginDate[8].isDigit() || !beginDate[9].isDigit()) {
               return "Вы ввели неправильную дату или вообще не дату";
           }
           else {
               dayBegin = (beginDate[0].unicode() - 48) * 10 + beginDate[1].unicode()-48;
               monthBegin = (beginDate[3].unicode()-48) * 10 + (beginDate[4].unicode()-48);
               yearBegin = (beginDate[6].unicode()-48) * 1000 + (beginDate[7].unicode()-48) * 100 + (beginDate[8].unicode()-48) * 10 + (beginDate[9].unicode()-48);

               if (yearBegin < 1970 || yearBegin >= 2038 || monthBegin < 1 || monthBegin >12 || dayBegin < 1 || dayBegin >31 ) return "Вы ввели неправильную дату";
               if(yearBegin % 400 == 0 || (yearBegin % 4 == 0 && yearBegin % 100 != 0)) {
                   if(monthBegin == 2) {
                       if(dayBegin > 29) return "Вы ввели неправильную дату";
                   }
                   else if (dayBegin > mDays[monthBegin - 1]) return "Вы ввели неправильную дату";
               }
               else if(dayBegin > mDays[monthBegin - 1]) return "Вы ввели неправильную дату";
               qDebug() << "Вы ввели дату: " << dayBegin << "." << monthBegin << "." << yearBegin;
           }


           if (endDate.length() != 10 || endDate[2] != '.' || endDate[5] != '.' || endDate[1] == 0 || ! endDate[0].isDigit() || ! endDate[1].isDigit() || ! endDate[3].isDigit() || ! endDate[4].isDigit()
               || ! endDate[6].isDigit() || ! endDate[7].isDigit() || ! endDate[8].isDigit() || ! endDate[9].isDigit()) {
               return "Вы ввели неправильную дату или вообще не дату";
           }
           else {
               dayEnd = (  endDate[0].unicode() - 48) * 10 +   endDate[1].unicode() - 48;
               monthEnd = ( endDate[3].unicode() - 48) * 10 + ( endDate[4].unicode() - 48);
               yearEnd = (  endDate[6].unicode() - 48) * 1000 + (  endDate[7].unicode() - 48) * 100 + (  endDate[8].unicode() - 48) * 10 + (  endDate[9].unicode() - 48);


               if (yearEnd < 1970 || yearEnd >= 2038 || monthEnd < 1 || monthEnd >12 || dayEnd < 1 || dayEnd >31 ) return "Вы ввели неправильную дату";
               if(yearEnd % 400 == 0 || (yearEnd % 4 == 0 && yearEnd % 100 != 0)) {
                   if(monthEnd == 2) {
                       if(dayEnd > 29) return "Вы ввели неправильную дату";
                   }
                   else if (dayEnd > mDays[monthEnd - 1]) return "Вы ввели неправильную дату";
               }
               else if(dayEnd > mDays[monthEnd - 1]) return "Вы ввели неправильную дату";
               qDebug() << "Вы ввели дату: " << dayEnd << "." << monthEnd << "." << yearEnd;
           }

           if (yearEnd > yearBegin) qDebug() << "Вы ввели дату: " << dayEnd << "." << monthEnd << "." << yearEnd;
           else if (yearEnd == yearBegin) {
               if (monthEnd > monthBegin) qDebug() << "Вы ввели дату: " << dayEnd << "." << monthEnd << "." << yearEnd;
               else if (monthEnd == monthBegin) {
                   if (dayEnd >= dayBegin) qDebug() << "Вы ввели дату: " << dayEnd << "." << monthEnd << "." << yearEnd;
                   else return "Начальная дата позже конечной";
               }
           }
           else return "Вы ввели неправильную дату";


           // Переводим дату первого рабочего дня в int
           firstDayDay = (  a->firstDayDate[0].unicode() - 48) * 10 +   a->firstDayDate[1].unicode() - 48;
           firstDayMonth = ( a->firstDayDate[3].unicode() - 48) * 10 + ( a->firstDayDate[4].unicode() - 48);
           firstDayYear = (  a->firstDayDate[6].unicode() - 48) * 1000 + (  a->firstDayDate[7].unicode() - 48) * 100 + (  a->firstDayDate[8].unicode() - 48) * 10 + (  a->firstDayDate[9].unicode() - 48);



           // Считаем количество дней от 1970 до начала периода, для которого необходимо посчитать количество рабочих дней и зарплату
           int amountOfDaysBeforeBeginningOfPeriod = 0;
           for (int i = 0; i < monthBegin - 1; i++) {
               if (i == 1)
                   if (yearBegin % 400 == 0 || (yearBegin % 4 == 0 && yearBegin % 100 != 0))
                       amountOfDaysBeforeBeginningOfPeriod += 29;
                   else amountOfDaysBeforeBeginningOfPeriod += mDays[i];
               else amountOfDaysBeforeBeginningOfPeriod += mDays[i];
           }
           for (int i = 1970; i < yearBegin; i++) {
               if (i % 400 == 0 || (i % 4 == 0 && i % 100 != 0))
                   amountOfDaysBeforeBeginningOfPeriod += 366;
               else amountOfDaysBeforeBeginningOfPeriod += 365;
           }
           amountOfDaysBeforeBeginningOfPeriod += dayBegin;
           qDebug() << "Количество дней с начала 1970 года до начала периода: " << amountOfDaysBeforeBeginningOfPeriod;



           // Считаем количество дней от 1970 до конца отрезка, для которого необходимо посчитать количество рабочих дней
           int amountOfDaysBeforeEndingOfPeriod = 0;
           for (int i = 0; i < monthEnd - 1; i++) {
               if (i == 1)
                   if (yearEnd % 400 == 0 || (yearEnd % 4 == 0 && yearEnd % 100 != 0))
                       amountOfDaysBeforeEndingOfPeriod += 29;
                   else amountOfDaysBeforeEndingOfPeriod += mDays[i];
               else amountOfDaysBeforeEndingOfPeriod += mDays[i];
           }
           for (int i = 1970; i < yearEnd; i++) {
               if (i % 400 == 0 || (i % 4 == 0 && i % 100 != 0))
                   amountOfDaysBeforeEndingOfPeriod += 366;
               else amountOfDaysBeforeEndingOfPeriod += 365;
           }
           amountOfDaysBeforeEndingOfPeriod += dayEnd;
           qDebug() << "Количество дней с начала 1970 года до конечной даты периода: " << amountOfDaysBeforeEndingOfPeriod;



           // Считаем количество дней от 1970 до даты начала работы на работе
           int amountOfDaysBeforeFirstWorkingDay = 0;
           for (int i = 0; i < firstDayMonth - 1; i++) {
               if (i == 1) {
                   if (firstDayYear % 400 == 0 || (firstDayYear % 4 == 0 && firstDayYear % 100 != 0))
                       amountOfDaysBeforeFirstWorkingDay += 29;
                   else amountOfDaysBeforeFirstWorkingDay += mDays[i];
               }
               else amountOfDaysBeforeFirstWorkingDay += mDays[i];
           }
           for (int i = 1970; i < firstDayYear; i++) {
               if (i % 400 == 0 || (i % 4 == 0 && i % 100 != 0))
                   amountOfDaysBeforeFirstWorkingDay += 366;
               else amountOfDaysBeforeFirstWorkingDay += 365;
           }
           amountOfDaysBeforeFirstWorkingDay += firstDayDay;
           qDebug() << "Количество дней с начала 1970 года до даты начала работы на работе: " << amountOfDaysBeforeFirstWorkingDay;
           // Проверяем, раньше ли дата начала периода, для которого необходимо рассчитать количество рабоачих дней, чем дата, когда сотрудник устроился на работу
           if(amountOfDaysBeforeBeginningOfPeriod < amountOfDaysBeforeFirstWorkingDay) {
               amountOfDaysBeforeBeginningOfPeriod = amountOfDaysBeforeFirstWorkingDay;
               qDebug() << "В периоде есть даты, в которые сотрудник еще не работал";
           }
           if(amountOfDaysBeforeEndingOfPeriod < amountOfDaysBeforeFirstWorkingDay) {
               return "В рассчетный период сотрудник еще не устроился на работу";
           }


           // Считаем количество дней с 1970 до года, в котором человек устроился на работу
           int amountOfDaysBeforeStartWorkYear = 0;
           for (int i = 1970; i < firstDayYear; i++) {
               if (i % 400 == 0 || (i % 4 == 0 && i % 100 != 0))
                   amountOfDaysBeforeStartWorkYear += 366;
               else amountOfDaysBeforeStartWorkYear += 365;
           }
           qDebug() << "Количество дней с 1970 года до начала года, в котором человек устроился на работу: " << amountOfDaysBeforeStartWorkYear;



           //  Находим с какого номера дня начинается год, в которм начинается период расчета рабочих дней, что бы для этого года и всех последующих рассчитать праздничные дни
           int amountOfDaysBeforeYearWithPeriod = 0;
           for (int i = 1970; i < yearBegin; i++) {
               if (i % 400 == 0 || (i % 4 == 0 && i % 100 != 0))
                   amountOfDaysBeforeYearWithPeriod += 366;
               else amountOfDaysBeforeYearWithPeriod += 365;
           }
           qDebug() << "Количество дней с 1970 года до начала отсчетного года: " << amountOfDaysBeforeYearWithPeriod;

           // Считаем количество праздничных дней в годах, для которых необходимо определить количество рабочих дней
           int HolidaysYears = yearEnd - yearBegin + 1;
           int HolidaysCount = (HolidaysYears) * 14;
           int amountOfDaysForCurrentYear = amountOfDaysBeforeYearWithPeriod;
           int yearCurrent = yearBegin;
           int januaryNumOfHoliday;
           mHolidays = new int[HolidaysCount];  // Массив с праздничными днями

           for (int j = 0; j < HolidaysYears; j++)	{  // Записываем все праздничные дни расчетного года в массив
               januaryNumOfHoliday = 0;
               for (int i = 14 * j; i < 8 + 14 * j; i++) { // Январьские праздники (1-8 января)
                   mHolidays[i] = amountOfDaysForCurrentYear + 1 + januaryNumOfHoliday;
                   januaryNumOfHoliday++;
               }
               mHolidays[8 + j * 14] = amountOfDaysForCurrentYear + 54;		// 23 февраля

               // Проверяем на високосный год для того, что бы записать в массив с празднечными днями правильные праздничные дни
               if (yearCurrent % 400 == 0 || (yearCurrent % 4 == 0 && yearCurrent % 100 != 0)) {
                   for (int i = 9 + 14 * j; i < 10 + 14 * j; i++) {
                       mHolidays[i] = amountOfDaysForCurrentYear + 68;			// 8 марта
                       mHolidays[i + 1] = amountOfDaysForCurrentYear + 122;		// 1 мая
                       mHolidays[i + 2] = amountOfDaysForCurrentYear + 130;		// 9 мая
                       mHolidays[i + 3] = amountOfDaysForCurrentYear + 164;		// 12 июня
                       mHolidays[i + 4] = amountOfDaysForCurrentYear + 309;	// 4 ноября
                   }
               }
               else {
                   for (int i = 9 + 14 * j; i < 10 + 14 * j; i++) {
                       mHolidays[i] = amountOfDaysForCurrentYear + 67;			// 8 марта
                       mHolidays[i + 1] = amountOfDaysForCurrentYear + 121;		// 1 мая
                       mHolidays[i + 2] = amountOfDaysForCurrentYear + 129;		// 9 мая
                       mHolidays[i + 3] = amountOfDaysForCurrentYear + 163;		// 12 июня
                       mHolidays[i + 4] = amountOfDaysForCurrentYear + 308;	// 4 ноября
                   }
               }
               if (yearCurrent % 400 == 0 || (yearCurrent % 4 == 0 && yearCurrent % 100 != 0)) amountOfDaysForCurrentYear += 366;
               else amountOfDaysForCurrentYear += 365;
               yearCurrent++;
           }



           // Находим день недели, от которого необходимо начать отсчет для подсчета количества рабочих дней
           QString weekList[] = { "Monday","Tuesday","Wednesday","Thursday", "Friday","Saturday","Sunday" };
           QString dayOfWeek = " ";
           int dayOfWeekBegin;

           for (int i = 0, j = 3; i < amountOfDaysBeforeBeginningOfPeriod; i++) {  // j (от 0 до 6) = 3 - потому что 1970 год начинается с четверга
               dayOfWeek = weekList[j];
               dayOfWeekBegin = j;
               if (j % 6 == 0 && j != 0) j = 0;
               else j++;
           }
           qDebug() << "День недели начального дня: " << dayOfWeek;



           // Определяем день недели конца отрезка, для которого необходимо посчитать количество рабочих дней (не обязательно)
           QString dayOfWeekEndOfPeriod;
           for (int i = 0, j = 3; i < amountOfDaysBeforeEndingOfPeriod; i++) {
               dayOfWeekEndOfPeriod = weekList[j];
               if (j % 6 == 0 && j != 0)  j = 0;
               else j++;
           }
           qDebug() << "День недели конечного дня: " << dayOfWeekEndOfPeriod;



           // Определяем день недели когда сотрудник вступил в должность (не обязательно)
           QString dayOfWeekBeginWorkingAtWork;
           for (int i = 0, j = 3; i < amountOfDaysBeforeFirstWorkingDay; i++) {
               dayOfWeekBeginWorkingAtWork = weekList[j];
               if (j % 6 == 0 && j != 0) j = 0;
               else j++;
           }
           qDebug() << "День недели конца сотрудник вступил в должность : " << dayOfWeekBeginWorkingAtWork;


           // Добавляем в массив номера всех последних дней февраля, начиная с года, в котором человек устроился на работу
           vector<int> LastFebraryDays;
           int currentYear = firstDayYear;
           amountOfDaysForCurrentYear = 0;
           while(currentYear < yearEnd+1) {
               if (currentYear % 400 == 0 || (currentYear % 4 == 0 && currentYear % 100 != 0)) {
                   if(currentYear>=firstDayYear) LastFebraryDays.push_back(amountOfDaysForCurrentYear + 60);
                    amountOfDaysForCurrentYear += 366;
               }
               else {
                    if(currentYear>=firstDayYear) LastFebraryDays.push_back(amountOfDaysForCurrentYear + 59);
                    amountOfDaysForCurrentYear += 365;
               }
               currentYear++;
           }



           // Считаем зарплату рабочего за каждый день выбранного периода
           int numberOfWorkedYears;                                   // Количество лет, которых человек проработал в компании
           int currentYear2;
           int currentDayOfWeek = dayOfWeekBegin;
           int amountOfDaysEveryYear = amountOfDaysBeforeStartWorkYear;
           int iterator;
           int amountOfDaysForStartToWorkPlusYears;
           int quantityOfSubodinates = 0;
           double result;
           bool dayIsHoliday;                                                  // Для проверки на то выходной/праздничный или рабочий день

           for(int i = amountOfDaysBeforeBeginningOfPeriod; i < amountOfDaysBeforeEndingOfPeriod + 1; i++) {
                dayIsHoliday = false;
                for(int j = 0; j < HolidaysCount; j++) {               // Праздничный ли день?
                    if(i == mHolidays[j]) dayIsHoliday = true;
                    if(currentDayOfWeek == 5 || currentDayOfWeek ==6) dayIsHoliday = true;  // Выходной ли день?
                }

                if(currentDayOfWeek == 6) currentDayOfWeek = 0;
                else currentDayOfWeek++;

                if(!dayIsHoliday) {
                    iterator = 0;
                    numberOfWorkedYears = 0;
                    currentYear2 = firstDayYear;
                    amountOfDaysForStartToWorkPlusYears = amountOfDaysBeforeFirstWorkingDay;

                    // Проверки:
                    // 1) является ли год, в котором человек устроился на работу вискокосным
                    // 2) если да то сравнить дату, когда человек устроился на работу с номером дня 29 февраля
                    // 3) если позже 29 февраля, то +365, если раньше, то +366
                    // 4) если год не високосный
                    // 5) является ли следующий год високосным
                    // 6) если да, сравнить дату, когда человек устроился на работу с 28 феврался
                    // 7) если позже +366, если раньше или = +365
                    // 8) увеличить год на 1

                    while(i>amountOfDaysForStartToWorkPlusYears) {
                       if(currentYear2 % 400 == 0 || (currentYear2 % 4 == 0 && currentYear2 % 100 != 0)) {
                            if(amountOfDaysForStartToWorkPlusYears >  amountOfDaysEveryYear + LastFebraryDays[iterator]) {
                                amountOfDaysForStartToWorkPlusYears +=365;
                            }
                            else amountOfDaysForStartToWorkPlusYears+=366;
                       }
                       else {
                           if((currentYear2 + 1)% 400 == 0 || ((currentYear2 +1) % 4 == 0 && (currentYear2 +1 )% 100 != 0)) {
                               if(amountOfDaysForStartToWorkPlusYears >  amountOfDaysEveryYear + LastFebraryDays[iterator]) {
                                   amountOfDaysForStartToWorkPlusYears +=366;
                               }
                               else amountOfDaysForStartToWorkPlusYears +=365;
                           }
                           else amountOfDaysForStartToWorkPlusYears +=365;
                       }
                        currentYear2++;
                        if(i>=amountOfDaysForStartToWorkPlusYears) numberOfWorkedYears++;
                        iterator++;
                   }
                    if(a->typeOfWorker == "Employee") {
                        if(numberOfWorkedYears > 10) numberOfWorkedYears =10;
                        result = a->baseRate + 0.03*numberOfWorkedYears*a->baseRate;
                    }
                    else if(a->typeOfWorker == "Manager") {
                        if(numberOfWorkedYears > 8) numberOfWorkedYears =8;
                        quantityOfSubodinates =0;
                        for(auto &a: workers ) {
                            if(a->chiefId == mainChiefId) {

                                int firstDayDaySub = (  a->firstDayDate[0].unicode() - 48) * 10 +   a->firstDayDate[1].unicode() - 48;
                                int firstDayMonthSub = ( a->firstDayDate[3].unicode() - 48) * 10 + ( a->firstDayDate[4].unicode() - 48);
                                int firstDayYearSub = (  a->firstDayDate[6].unicode() - 48) * 1000 + (  a->firstDayDate[7].unicode() - 48) * 100 + (  a->firstDayDate[8].unicode() - 48) * 10 + (  a->firstDayDate[9].unicode() - 48);

                                // Считаем количество дней от 1970 до даты начала работы на работе подчиненного для того, что бы узнать
                                // был ли этот человек подчиненным в тот день, для которого рассчитывается зарплата для typeOfWorker
                                int amountOfDaysBeforeFirstWorkingDaySub = 0;
                                for (int i = 0; i < firstDayMonthSub - 1; i++) {
                                    if (i == 1) {
                                        if (firstDayYearSub % 400 == 0 || (firstDayYearSub % 4 == 0 && firstDayYearSub % 100 != 0))
                                            amountOfDaysBeforeFirstWorkingDaySub += 29;
                                        else amountOfDaysBeforeFirstWorkingDaySub += mDays[i];
                                    }
                                    else amountOfDaysBeforeFirstWorkingDaySub += mDays[i];
                                }
                                for (int i = 1970; i < firstDayYearSub; i++) {
                                    if (i % 400 == 0 || (i % 4 == 0 && i % 100 != 0))
                                        amountOfDaysBeforeFirstWorkingDaySub += 366;
                                    else amountOfDaysBeforeFirstWorkingDaySub += 365;
                                }
                                amountOfDaysBeforeFirstWorkingDaySub += firstDayDaySub;
                                if(i>=amountOfDaysBeforeFirstWorkingDaySub) quantityOfSubodinates++;
                           }
                       }
                        result = a->baseRate + 0.05*numberOfWorkedYears*a->baseRate+0.005*a->baseRate*quantityOfSubodinates;
                    }
                    else if(a->typeOfWorker == "Sales") {

                        vector<int> subChiefIds;
                        subChiefIds.push_back(mainChiefId);
                        unsigned long long subChiefIdsMainSize = subChiefIds.size();
                        unsigned long long quantityOfSubForDelete;
                        unsigned long long subChiefIdsMainSize2 =0;


                       // Считаем количество подчиненных всех уровней
                       quantityOfSubodinates = 0;
                       while (true) {
                            quantityOfSubForDelete = 0;
                            for(unsigned long long a = 0; a < subChiefIdsMainSize; a++) {
                                for(auto &b : workers) {
                                    if(subChiefIds[a] == b->chiefId) {
                                        int firstDayDaySub = (  b->firstDayDate[0].unicode() - 48) * 10 +   b->firstDayDate[1].unicode() - 48;
                                        int firstDayMonthSub = ( b->firstDayDate[3].unicode() - 48) * 10 + ( b->firstDayDate[4].unicode() - 48);
                                        int firstDayYearSub = (  b->firstDayDate[6].unicode() - 48) * 1000 + (  b->firstDayDate[7].unicode() - 48) * 100 + (  b->firstDayDate[8].unicode() - 48) * 10 + (  b->firstDayDate[9].unicode() - 48);


                                        int amountOfDaysBeforeFirstWorkingDaySub = 0;
                                        for (int i = 0; i < firstDayMonthSub - 1; i++) {
                                            if (i == 1) {
                                                if (firstDayYearSub % 400 == 0 || (firstDayYearSub % 4 == 0 && firstDayYearSub % 100 != 0))
                                                    amountOfDaysBeforeFirstWorkingDaySub += 29;
                                                else amountOfDaysBeforeFirstWorkingDaySub += mDays[i];
                                            }
                                            else amountOfDaysBeforeFirstWorkingDaySub += mDays[i];
                                        }
                                        for (int i = 1970; i < firstDayYearSub; i++) {
                                            if (i % 400 == 0 || (i % 4 == 0 && i % 100 != 0))
                                                amountOfDaysBeforeFirstWorkingDaySub += 366;
                                            else amountOfDaysBeforeFirstWorkingDaySub += 365;
                                        }
                                        amountOfDaysBeforeFirstWorkingDaySub += firstDayDaySub;
                                        if(i>=amountOfDaysBeforeFirstWorkingDaySub) quantityOfSubodinates++;

                                        subChiefIds.push_back(b->id);
                                        quantityOfSubForDelete++;
                                    }
                                }
                            }
                            subChiefIdsMainSize2 = subChiefIds.size();
                            auto iter = subChiefIds.begin();
                            for (unsigned long long i = 0 ; i < subChiefIdsMainSize2 - quantityOfSubForDelete; i++) {
                                subChiefIds.erase(iter);
                                iter = subChiefIds.begin();
                            }
                            if(subChiefIdsMainSize == 0) break;
                            subChiefIdsMainSize = subChiefIds.size();
                       }

                       if(numberOfWorkedYears > 35) numberOfWorkedYears =35;
                       result = a->baseRate + 0.01*numberOfWorkedYears*a->baseRate + a->baseRate*0.003*quantityOfSubodinates; // Добавить заплату за всех подчиненных!!!
                   }
                    salary+= result;
                }
            }
        }
    }
    salary *=10;
    salary = floor(salary) /10;

    if(badenter) return "Такого id в базе данных нет";
    else delete[] mHolidays;
    return "Зарплата сотрудника составляет " + QString::number(salary, 'g', 8) + " руб.";
}

void MainCode::addWorker(QString name, QString firstDayDate, QString rate, QString type, QString login, QString firstPassword, QString secondPassword, QString chiefId, QString superuser) {
    vector<shared_ptr<Worker>> workers = createWorkers();
    bool badEnter = false;
    int mDays[12] = { 31,28,31,30,31,30,31,31,30,31,30,31};
    while(true) {

    // Проверка имени
    if(name.length() > 20) {
        badEnter = true;
        emit sendErrorMessageForName("Длинна имени не должна привышать 20 символов");
        break;
    }
    if(name.length() < 1) {
        badEnter = true;
        emit sendErrorMessageForName("Ошибка ввода");
        break;
    }
    for(int i = 0; i < name.length(); i ++) {
        if(name[i].isDigit()) {
            badEnter = true;
            emit sendErrorMessageForName("Имя не должно содержать цифры");
            break;
        }
    }

    // Проверка даты первого рабочего дня
    if (firstDayDate.length() != 10 || firstDayDate[2] != '.' || firstDayDate[5] != '.' || firstDayDate[1] == 0 || !firstDayDate[0].isDigit() || !firstDayDate[1].isDigit() || !firstDayDate[3].isDigit() || !firstDayDate[4].isDigit()
        || !firstDayDate[6].isDigit() || !firstDayDate[7].isDigit() || !firstDayDate[8].isDigit() || !firstDayDate[9].isDigit()) {
        emit sendErrorMessageForBeginDate("Ошибка ввода");
        badEnter = true;
        break;
    }

    int dayBegin = (firstDayDate[0].unicode() - 48) * 10 + firstDayDate[1].unicode()-48;
    int monthBegin = (firstDayDate[3].unicode()-48) * 10 + (firstDayDate[4].unicode()-48);
    int yearBegin = (firstDayDate[6].unicode()-48) * 1000 + (firstDayDate[7].unicode()-48) * 100 + (firstDayDate[8].unicode()-48) * 10 + (firstDayDate[9].unicode()-48);

    if (yearBegin < 1970 || yearBegin >= 2038 || monthBegin < 1 || monthBegin >12 || dayBegin < 1 || dayBegin >31) {
       badEnter = true;
       emit sendErrorMessageForBeginDate("Ошибка ввода");
       break;
    }

    if (yearBegin % 400 == 0 || (yearBegin % 4 == 0 && yearBegin % 100 != 0)) {
        if(monthBegin == 2 ) {
            if(dayBegin > 29) {
                badEnter = true;
                emit sendErrorMessageForBeginDate("Последний день февраля в этом году: 29");
                break;
            }
        }
        else if(dayBegin > mDays[monthBegin - 1]) {
            emit sendErrorMessageForBeginDate("Ошибка ввода");
            badEnter = true;
            break;
        }
    }
    else if(dayBegin > mDays[monthBegin - 1]) {
        emit sendErrorMessageForBeginDate("Ошибка ввода");
        badEnter = true;
        break;
    }
    qDebug() << "Вы ввели дату: " << dayBegin << "." << monthBegin << "." << yearBegin;

    // Проверка базовой ставки
    if(rate.length() > 20) {
        emit sendErrorMessageForBaseRate("Слишком длинное число");
        badEnter = true;
        break;
    }
    else {
        for(int i = 0; i < rate.length(); i++) {
            if(!rate[i].isDigit()) {
                emit sendErrorMessageForBaseRate( "Базовая ставка должна быть целым числом");
                badEnter = true;
                break;
            }
        }
    }
    if(rate.length() < 1) {
        emit sendErrorMessageForBaseRate("Ошибка ввода");
        badEnter = true;
        break;
    }

     // Проверка типа работника
    if(type != "Employee" && type != "Manager" && type != "Sales") {
        badEnter = true;
        emit sendErrorMessageForType("Выберите тип работника");
        break;
    }

     // Проверка логина и пароля
     if(login.length() > 20) {
         badEnter  = true;
         emit sendErrorMessageForLogin("Длина логина должна быть менее 20 символов");
         break;
     }
     if(login.length() < 1) {
         badEnter  = true;
         emit sendErrorMessageForLogin("Ошибка ввода");
         break;
     }
     for(auto &a: workers) {
         if(a->login == login) {
             badEnter  = true;
             emit sendErrorMessageForLogin("Такой логин уже существует");
             break;
         }
     }
     if(firstPassword.length() > 20) {
         badEnter  = true;
         emit sendErrorMessageForFirstPassword("Длина пароля должна быть менее 20 символов");
         break;
     }
     if(badEnter) break;
     if(firstPassword.length() < 1) {
         badEnter  = true;
         emit sendErrorMessageForFirstPassword("Ошибка ввода");
         break;
     }
     if(secondPassword != firstPassword) {
         badEnter  = true;
         emit sendErrorMessageForSecondPassword("Пароли не совпадают");
         break;
     }

     // Проверка id начальника
     QString newWorkerId = "";
     for(int i = 0; i < chiefId.size(); i++) {
         if(chiefId[i].isDigit()) {
             newWorkerId += chiefId[i];
         }
     }
     if(newWorkerId == "") {
         badEnter  = true;
         emit sendErrorMessageForChief("Выберите начальника");
         break;
     }

     if(superuser == "-") {
         badEnter  = true;
         emit sendErrorMessageForSuperuser("Выберите, является ли суперпользователем");
         break;
     }
     if(superuser == "Да") superuser = "1";
     if(superuser == "Нет") superuser = "0";
    qDebug() << superuser;

    {
         QSqlDatabase workersDB = QSqlDatabase::addDatabase("QSQLITE");
         workersDB.setDatabaseName("C://Users//User//Desktop//WorkersDB//Workers.db");
         if(!workersDB.open()) qDebug() << "Failed to open database";
         else qDebug() << "DataBase is connected";
         QSqlQuery qry;

         qry.prepare("insert into WorkersTable (Name, FirstDayDate, BaseSalary, TypeOfWorker, Login, Password, Chief, Superuser) values ('"+name+"','"+firstDayDate+"' ,'"+rate+"' , '"+type+"','"+login+"' , '"+firstPassword+"','"+newWorkerId+"' , '"+superuser+"') ");

         if(qry.exec()) {
            qDebug() << "Информация о сотруднике добавлена в базу данных";
            emit sendCommonErrorMessage("Информация о сотруднике добавлена в базу данных");
            workersDB.close();
         }
    }
    QSqlDatabase::removeDatabase("qt_sql_default_connection");
    break;

    }
    if(badEnter) emit sendCommonErrorMessage("Неправильный ввод данных, повторите попытку");
}

void MainCode::fillChiefListModel() {
    {
        QSqlDatabase workersDB = QSqlDatabase::addDatabase("QSQLITE");
        workersDB.setDatabaseName("C://Users//User//Desktop//WorkersDB//Workers.db");
        if(!workersDB.open()) qDebug() << "Failed to open database";
        else qDebug() << "DataBase is connected";
        QSqlQuery qry;

        if(qry.exec("SELECT * FROM WorkersTable ")) {
            while(qry.next()) {
                emit sendWorkersInfoToQML(qry.value(0).toString() + " " + qry.value(1).toString());
            }
            workersDB.close();
        }
     }
     QSqlDatabase::removeDatabase("qt_sql_default_connection");
}
