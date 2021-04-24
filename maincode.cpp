#include "maincode.h"

MainCode::MainCode(QObject *parent) : QObject(parent) {}

void MainCode::logIn(QString login, QString password) {                                                            // Вход по логину и паролю
    {
        foundCoincidence = false;
        QSqlDatabase workersDB = QSqlDatabase::addDatabase("QSQLITE");                              // Создаем объект для работы с базой данных
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
                    qDebug() << "Your login and password are correct ";
                    qDebug() << login;  // Отладочная
                    qDebug() << password;
                    emit  sendErrorMessage(" ");
                }

                if(!foundCoincidence)  {
                    qDebug() <<"Your login or password isn't correct";
                    emit sendErrorMessage("Неправильно введен логин или пароль");
                }
        }
        else {
            qDebug() << "Your login or password isn't correct ";
        }
        workersDB.close();
    }
    QSqlDatabase::removeDatabase("qt_sql_default_connection");
}

void MainCode::receiveDataFromQMLforCountSalary(QString beginDate, QString endDate, QString name)
{
    vector<shared_ptr<Worker>> workers = createWorkers();
    emit sendSalaryToQML((CountSalary(beginDate, endDate, name, workers, idForQML)).toInt());
}

void MainCode::findSubordinates()
{
    createWorkers();
}

vector<shared_ptr<Worker>> MainCode::createWorkers() {

    int sizeArr;
    int idForArr;                                                    // Переменные для занесения в массив с работниками
    int chiefIdForArr;
    int chiefsCounter;
    int baseSalaryForArr;
    QString nameForArr;
    QString firstDayDateForArr;
    QString typeOfWorkerForArr;
    QString loginForArr;
    vector<int> chiefsid;                                       // Массив куда будут добавлятся id начальников, а потом удаляться старые
    vector<shared_ptr<Worker>> workers;
    vector<shared_ptr<Worker>> UserAndSubordinateWorkers;

    chiefsid.push_back(idForQML);


   {
       QSqlDatabase workersDB = QSqlDatabase::addDatabase("QSQLITE");                              // Создаем объект для работы с базой данных
       workersDB.setDatabaseName("C://Users//User//Desktop//WorkersDB//Workers.db");    // Указываю путь к базе данных
       if(!workersDB.open()) qDebug() << "Failed to open database";
       else qDebug() << "DataBase is connected";
       QSqlQuery qry;                                                                                                                    // Объект для запроса информации из БД

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

                   if(typeOfWorkerForArr == "Employee") workers.push_back(make_shared<Employee>( idForArr, nameForArr, firstDayDateForArr, baseSalaryForArr, typeOfWorkerForArr, loginForArr, chiefIdForArr));
                   else if (typeOfWorkerForArr == "Sales") workers.push_back(make_shared<Sales>( idForArr, nameForArr, firstDayDateForArr, baseSalaryForArr, typeOfWorkerForArr, loginForArr, chiefIdForArr));
                   else if(typeOfWorkerForArr == "Manager") workers.push_back(make_shared<Manager>( idForArr, nameForArr, firstDayDateForArr, baseSalaryForArr, typeOfWorkerForArr, loginForArr, chiefIdForArr));
               }
           }
           workersDB.close();
       }
    }

        QSqlDatabase::removeDatabase("qt_sql_default_connection");

        int level = 0;
        QString a = nameForQML;
   for(unsigned long long j = 0; j < workers.size(); j++) {
       if(a == workers[j]->name) {
           if(typeOfWorkerForArr == "Employee") UserAndSubordinateWorkers.push_back(make_shared<Employee>( workers[j]->id, nameForQML, workers[j]->firstDayDate, workers[j]->baseRate, workers[j]->typeOfWorker, workers[j]->login, workers[j]->chiefId));
           else if (typeOfWorkerForArr == "Sales") UserAndSubordinateWorkers.push_back(make_shared<Sales>( workers[j]->id, nameForQML, workers[j]->firstDayDate, workers[j]->baseRate, workers[j]->typeOfWorker, workers[j]->login, workers[j]->chiefId));
           else if(typeOfWorkerForArr == "Manager") UserAndSubordinateWorkers.push_back(make_shared<Manager>( workers[j]->id, nameForQML, workers[j]->firstDayDate, workers[j]->baseRate, workers[j]->typeOfWorker, workers[j]->login, workers[j]->chiefId));
       }
   }

   while(true) {                           // Выполняется пока не будет break
       chiefsCounter = 0;
       sizeArr = chiefsid.size();

       for(unsigned long long i = 0; i <chiefsid.size()-chiefsCounter; i++ ) { // Для каждого элемента в массиве chiefsid ( вначале там только id одного человека,
           // под  логином и паролем которого я зашел)
           for (unsigned long long j = 0; j < workers.size(); j++) {                       // Для каждого работника, которого я добавил из базы данных
               if(chiefsid[i] == workers[j]->chiefId) {                                            // Если id начальника ( 1-й коннор) совпадает с полем Chief у подчиненного

                   idSub = QString::number(workers[j]->id);                                    // Записываем его данные, что бы отобразить далее на экране
                   nameSub = workers[j]->name;
                   typeOfWorkerSub = workers[j]->typeOfWorker;
                   firstDayDateSub =  workers[j]->firstDayDate;
                   baseRateSub = QString::number(workers[j]->baseRate);
                   chiefIdSub = QString::number(workers[j]->chiefId);
                   loginSub = workers[j]->login;

                   for(auto &k: chiefsid) {
                       if(workers[j]->chiefId == k && chiefsid.size() == sizeArr) {
                           level ++;
                       }
                   }
                     chiefsid.push_back(workers[j]->id);                                             // То Добавляем его id, делаем его тоже шефом, только он теперь ниже по уровню

                   emit sendSubordinatesInfoToQML(idSub, nameSub, typeOfWorkerSub, firstDayDateSub, baseRateSub, chiefIdSub, QString::number(level));               // Отправляем данные в QML для отображения в ListModel

                     if(typeOfWorkerForArr == "Employee") UserAndSubordinateWorkers.push_back(make_shared<Employee>( workers[j]->id, nameSub, firstDayDateSub, workers[j]->baseRate, typeOfWorkerSub, loginSub, workers[j]->chiefId));
                     else if (typeOfWorkerForArr == "Sales") UserAndSubordinateWorkers.push_back(make_shared<Sales>( workers[j]->id, nameSub, firstDayDateSub, workers[j]->baseRate, typeOfWorkerSub, loginSub, workers[j]->chiefId));
                     else if(typeOfWorkerForArr == "Manager") UserAndSubordinateWorkers.push_back(make_shared<Manager>( workers[j]->id, nameSub, firstDayDateSub, workers[j]->baseRate, typeOfWorkerSub, loginSub, workers[j]->chiefId));
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
   for(auto &a: UserAndSubordinateWorkers) {
           qDebug() << a->name;
   }

   return UserAndSubordinateWorkers;
}

QString CountSalary(QString beginDate, QString endDate, QString name, vector<shared_ptr<Worker>> workers, int idForQML)
{

    double salary = 0;

    for( auto &a: workers) {
       if(name == a->name) {

           setlocale(LC_ALL, "rus");
           bool badenter = false;
           int dayBegin, monthBegin, yearBegin;   // firstDayDate - день когда работник устроился на работу
           int dayEnd, monthEnd, yearEnd;
           int firstDayDay, firstDayMonth, firstDayYear;
           int mDays[12] = { 31,28,31,30,31,30,31,31,30,31,30,31 };
           int mainChiefId = a->id;

           // Проверка введенных дат начальной и конечной и перевод их в int
           do
           {
               if (beginDate.length() != 10 || beginDate[2] != '.' || beginDate[5] != '.' || beginDate[1] == 0 || !beginDate[0].isDigit() || !beginDate[1].isDigit() || !beginDate[3].isDigit() || !beginDate[4].isDigit()
                   || !beginDate[6].isDigit() || !beginDate[7].isDigit() || !beginDate[8].isDigit() || !beginDate[9].isDigit())
               {
                   badenter = true;
                   qDebug() << "Вы ввели неправильную дату или вообще не дату";
                   return "Вы ввели неправильную дату или вообще не дату";
               }
               else
               {
                   dayBegin = (beginDate[0].unicode() - 48) * 10 + beginDate[1].unicode()-48;
                   monthBegin = (beginDate[3].unicode()-48) * 10 + (beginDate[4].unicode()-48);
                   yearBegin = (beginDate[6].unicode()-48) * 1000 + (beginDate[7].unicode()-48) * 100 + (beginDate[8].unicode()-48) * 10 + (beginDate[9].unicode()-48);

                   if (yearBegin < 1970 || yearBegin >= 2038 || monthBegin < 1 || monthBegin >12 || dayBegin < 1 || dayBegin >31 || dayBegin > mDays[monthBegin - 1])
                   {
                       badenter = true;
                       qDebug() << "Вы ввели неправильную дату";
                       return "Вы ввели неправильную дату";
                   }
                   else
                   {
                       qDebug() << "Вы ввели дату: " << dayBegin << "." << monthBegin << "." << yearBegin;
                       badenter = false;
                   }
               }
           } while (badenter);
           do
           {
               qDebug() << "Введите конечную дату в формате дд.мм.гггг";

               if (endDate.length() != 10 || endDate[2] != '.' || endDate[5] != '.' || endDate[1] == 0 || ! endDate[0].isDigit() || ! endDate[1].isDigit() || ! endDate[3].isDigit() || ! endDate[4].isDigit()
                   || ! endDate[6].isDigit() || ! endDate[7].isDigit() || ! endDate[8].isDigit() || ! endDate[9].isDigit())
               {
                   badenter = true;
                   qDebug() << "Вы ввели неправильную дату или вообще не дату";
                   return "Вы ввели неправильную дату или вообще не дату";
               }
               else
               {
                   dayEnd = (  endDate[0].unicode() - 48) * 10 +   endDate[1].unicode() - 48;
                   monthEnd = ( endDate[3].unicode() - 48) * 10 + ( endDate[4].unicode() - 48);
                   yearEnd = (  endDate[6].unicode() - 48) * 1000 + (  endDate[7].unicode() - 48) * 100 + (  endDate[8].unicode() - 48) * 10 + (  endDate[9].unicode() - 48);
                   if (yearEnd < 1970 || yearEnd >= 2038 || monthEnd < 1 || monthEnd >12 || dayEnd < 1 || dayEnd >31 || dayEnd > mDays[monthEnd - 1])
                   {
                       badenter = true;
                       qDebug() << "Вы ввели неправильную дату";
                       return "Вы ввели неправильную дату";
                   }
                   else
                   {
                       if (yearEnd > yearBegin)
                       {
                           qDebug() << "Вы ввели дату: " << dayEnd << "." << monthEnd << "." << yearEnd;
                           badenter = false;
                       }
                       else if (yearEnd == yearBegin)
                       {
                           if (monthEnd > monthBegin)
                           {
                               qDebug() << "Вы ввели дату: " << dayEnd << "." << monthEnd << "." << yearEnd;
                               badenter = false;
                           }
                           else if (monthEnd == monthBegin)
                           {
                               if (dayEnd > dayBegin)
                               {
                                   qDebug() << "Вы ввели дату: " << dayEnd << "." << monthEnd << "." << yearEnd;
                                   badenter = false;
                               }
                               else if (dayEnd == dayBegin)
                               {
                                   qDebug() << "Вы ввели дату: " << dayEnd << "." << monthEnd << "." << yearEnd;	// Тут надо написать, что количество дней = 1
                                   badenter = false;
                               }
                               else
                               {
                                   badenter = true;
                                   qDebug() << "Вы ввели неправильную дату";
                                   return "Вы ввели неправильную дату";
                               }
                           }
                       }
                       else
                       {
                           badenter = true;
                           qDebug() << "Вы ввели неправильную дату";
                           return "Вы ввели неправильную дату";
                       }

                   }
               }
           } while (badenter);

           // Переводим дату первого рабочего дня в int
           firstDayDay = (  a->firstDayDate[0].unicode() - 48) * 10 +   a->firstDayDate[1].unicode() - 48;
           firstDayMonth = ( a->firstDayDate[3].unicode() - 48) * 10 + ( a->firstDayDate[4].unicode() - 48);
           firstDayYear = (  a->firstDayDate[6].unicode() - 48) * 1000 + (  a->firstDayDate[7].unicode() - 48) * 100 + (  a->firstDayDate[8].unicode() - 48) * 10 + (  a->firstDayDate[9].unicode() - 48);
           qDebug() << "Дата из базы данных: " << firstDayDay << "." << firstDayMonth << "." << firstDayYear;




           // Считаем количество дней от 1970 до начала периода, для которого необходимо посчитать количество рабочих дней и зарплату
           int amountOfDays = 0;
           for (int i = 0; i < monthBegin - 1; i++)
           {
               if (i == 1)
                   if (yearBegin % 400 == 0 || (yearBegin % 4 == 0 && yearBegin % 100 != 0))
                       amountOfDays += 29;
                   else amountOfDays += mDays[i];
               else amountOfDays += mDays[i];
           }
           for (int i = 1970; i < yearBegin; i++)
           {
               if (i % 400 == 0 || (i % 4 == 0 && i % 100 != 0))
                   amountOfDays += 366;
               else amountOfDays += 365;
           }
           amountOfDays += dayBegin;
           qDebug() << "Количество дней с начала 1970 года да начала отсчета: " << amountOfDays;




           // Считаем количество дней от 1970 до конца отрезка, для которого необходимо посчитать количество рабочих дней
           int amountOfDaysEndDate = 0;
           for (int i = 0; i < monthEnd - 1; i++)
           {
               if (i == 1)
                   if (yearEnd % 400 == 0 || (yearEnd % 4 == 0 && yearEnd % 100 != 0))
                       amountOfDaysEndDate += 29;
                   else amountOfDaysEndDate += mDays[i];
               else amountOfDaysEndDate += mDays[i];
           }

           for (int i = 1970; i < yearEnd; i++)
           {
               if (i % 400 == 0 || (i % 4 == 0 && i % 100 != 0))
                   amountOfDaysEndDate += 366;
               else amountOfDaysEndDate += 365;
           }
           amountOfDaysEndDate += dayEnd;
           qDebug() << "Количество дней с начала 1970 года до конечной даты: " << amountOfDaysEndDate;




           // Считаем количество дней от 1970 до даты начала работы на работе
           int amountOfDaysForStartedToWork = 0;
           for (int i = 0; i < firstDayMonth - 1; i++)
           {
               if (i == 1)
               {
                   if (firstDayYear % 400 == 0 || (firstDayYear % 4 == 0 && firstDayYear % 100 != 0))
                       amountOfDaysForStartedToWork += 29;
                   else amountOfDaysForStartedToWork += mDays[i];
               }
               else amountOfDaysForStartedToWork += mDays[i];
           }

           for (int i = 1970; i < firstDayYear; i++)
           {
               if (i % 400 == 0 || (i % 4 == 0 && i % 100 != 0))
                   amountOfDaysForStartedToWork += 366;
               else amountOfDaysForStartedToWork += 365;
           }
           amountOfDaysForStartedToWork += firstDayDay;
           qDebug() << "Количество дней с начала 1970 года до даты начала работы на работе: " << amountOfDaysForStartedToWork;




           // Считаю количество дней с 1970 до года, в котором человек устроился на работу
           int amountOfDaysBeforeStartWorkYear = 0;

           for (int i = 1970; i < firstDayYear; i++)
           {
               if (i % 400 == 0 || (i % 4 == 0 && i % 100 != 0))
                   amountOfDaysBeforeStartWorkYear += 366;
               else amountOfDaysBeforeStartWorkYear += 365;
           }
           qDebug() << "Количество дней с 1970 года до начала года, в котором человек устроился на работу: " << amountOfDaysBeforeStartWorkYear;




           //  Находим с какого дня ( номер дня) начинается год, в которм начинается период расчета рабочих дней, что бы для этого года и всех последующих рассчитать праздничные дни
           int amountOfDaysForBeginYear = 0;
           for (int i = 1970; i < yearBegin; i++)
           {
               if (i % 400 == 0 || (i % 4 == 0 && i % 100 != 0))
                   amountOfDaysForBeginYear += 366;
               else amountOfDaysForBeginYear += 365;
           }
           qDebug() << "Количество дней с 1970 года до начала отсчетного года: " << amountOfDaysForBeginYear;

           // Считаем количество праздничных дней в году, для которых необходимо посчитать количество рабочих дней
           int HolidaysYears = yearEnd - yearBegin + 1;
           int HolidaysCount = (yearEnd - yearBegin + 1) * 14;
           int amountOfDaysForCurrentYear = amountOfDaysForBeginYear;
           int yearCurrent = yearBegin;
           int* mHolidays = new int[HolidaysCount];
           int k;
           for (int j = 0; j < HolidaysYears; j++)		// Записываем все праздничные дни расчетного года (года) в массив
           {
               k = 0;
               for (int i = 14 * j; i < 8 + 14 * j; i++)		// Январьские праздники (1-8 января)
               {

                   mHolidays[i] = amountOfDaysForCurrentYear + 1 + k;
                   k++;
               }
               mHolidays[8 + j * 14] = amountOfDaysForCurrentYear + 54;		// 23 февраля

               // Проверка на високосный год для того, что бы записать в массив с празднечными днями правильные праздничные дни
               if (yearCurrent % 400 == 0 || (yearCurrent % 4 == 0 && yearCurrent % 100 != 0))		// Записываю в массив праздничные дни, если год високосный
               {
                   for (int i = 9 + 14 * j; i < 10 + 14 * j; i++)
                   {
                       mHolidays[i] = amountOfDaysForCurrentYear + 68;			// 8 марта
                       mHolidays[i + 1] = amountOfDaysForCurrentYear + 122;		// 1 мая
                       mHolidays[i + 2] = amountOfDaysForCurrentYear + 130;		// 9 мая
                       mHolidays[i + 3] = amountOfDaysForCurrentYear + 164;		// 12 июня
                       mHolidays[i + 4] = amountOfDaysForCurrentYear + 309;		// 4 ноября
                   }
               }
               else
               {
                   for (int i = 9 + 14 * j; i < 10 + 14 * j; i++)		// Записываю в массив праздничные дни, если год невисокосный
                   {
                       mHolidays[i] = amountOfDaysForCurrentYear + 67;			// 8 марта
                       mHolidays[i + 1] = amountOfDaysForCurrentYear + 121;		// 1 мая
                       mHolidays[i + 2] = amountOfDaysForCurrentYear + 129;		// 9 мая
                       mHolidays[i + 3] = amountOfDaysForCurrentYear + 163;		// 12 июня
                       mHolidays[i + 4] = amountOfDaysForCurrentYear + 308;		// 4 ноября
                   }
               }
               if (yearCurrent % 400 == 0 || (yearCurrent % 4 == 0 && yearCurrent % 100 != 0))
               {
                   amountOfDaysForCurrentYear += 366;
               }
               else
               {
                   amountOfDaysForCurrentYear += 365;
               }
               yearCurrent++;
           }


           // Находим день недели, от которого необходимо начать отсчет для подсчета количества рабочих дней
           QString weekList[] = { "Monday","Tuesday","Wednesday","Thursday", "Friday","Saturday","Sunday" };
           QString dayOfWeek = " ";
           int dayOfWeekBegin;

           for (int i = 0, j = 3; i < amountOfDays; i++)
           {
               dayOfWeek = weekList[j];
               dayOfWeekBegin = j;
               if (j % 6 == 0 && j != 0)
               {
                   j = 0;
               }
               else
               {
                   j++;
               }
           }
           qDebug() << "Номер дня недели от 0 до 6: " << dayOfWeekBegin;		// Номер дня недели дня от которого начинается отсчет
           qDebug() << "День недели начального дня: " << dayOfWeek;			// Название дня недели от которого начинается отсчет



           // Определяем день недели конца отрезка, для которого необходимо посчитать количество рабочих дней (не обязательно)
           QString dayOfWeekEndDate;
           for (int i = 0, j = 3; i < amountOfDaysEndDate; i++)
           {
               dayOfWeekEndDate = weekList[j];
               if (j % 6 == 0 && j != 0)  j = 0;
               else j++;
           }
           qDebug() << "День недели конечного дня: " << dayOfWeekEndDate;


           // Определяем день недели конца сотрудник вступил в должность ( не обязательно)
           QString dayOfWeekBeginWorkingGlobal;
           for (int i = 0, j = 3; i < amountOfDaysForStartedToWork; i++)
           {
               dayOfWeekBeginWorkingGlobal = weekList[j];
               if (j % 6 == 0 && j != 0) j = 0;
               else j++;
           }
           qDebug() << "День недели конца сотрудник вступил в должность : " << dayOfWeekBeginWorkingGlobal;




           // Добавляю в массив номера всех последних дней февраля начиная с года, в котором человек устроился на работу
           vector<int> LastFebraryDays;
           int currentYear = firstDayYear;
           amountOfDaysForCurrentYear = 0;  // Количество дней с 1970 года до года в котором человек устроился на работу
           while(currentYear < yearEnd+1) {

               if (currentYear % 400 == 0 || (currentYear % 4 == 0 && currentYear % 100 != 0))
               {
                   if(currentYear>=firstDayYear) LastFebraryDays.push_back(amountOfDaysForCurrentYear + 60);
                    amountOfDaysForCurrentYear += 366;
               }
               else
               {
                    if(currentYear>=firstDayYear) LastFebraryDays.push_back(amountOfDaysForCurrentYear + 59);
                    amountOfDaysForCurrentYear += 365;
               }
               currentYear++;
           }


           // Считаю зарплату рабочего за каждый день выбранного периода
           bool dayIsHoliday;                          // Для проверки на то выходной/праздничный или рабочий день
           int numberOfWorkedYears;     // Количество лет, которых человек проработал в компании
           int currentYear2;     // Года считаются с того, с которого человек устроился на работу firstDayYear = 2010
           int currentDayOfWeek = dayOfWeekBegin;
           int amountOfDaysEveryYear = amountOfDaysBeforeStartWorkYear;
           double result;
           int iterator;
           int amountOfDaysForStartedToWorkPlusYears;
           int quantityOfSubodinates = 0;


           for(int i = amountOfDays; i < amountOfDaysEndDate + 1; i++) {    // От первого до последнего дня отсчета считаем зарплату для каждого дня
               // amountOfDays - количество дней от 1970 до 30.04.2021 ( до первого дня промежутка, для которого нужно рассчитать зарплату)
               dayIsHoliday = false;


               for(int j = 0; j < HolidaysCount; j++) {    // Проверка, праздничный ли день
                   if(i == mHolidays[j]) {
                       dayIsHoliday = true;
                   }

                   if(currentDayOfWeek == 5 || currentDayOfWeek ==6) {  // Проверка выходной ли день
                       dayIsHoliday = true;
                    }
               }


               if(currentDayOfWeek == 6) {
                   currentDayOfWeek = 0;
               }
               else {
                   currentDayOfWeek++;
               }

                   if(!dayIsHoliday) {                      // Если день рабочий, то...
                       iterator = 0;
                       numberOfWorkedYears = 0;
                       currentYear2 = firstDayYear;
                       amountOfDaysForStartedToWorkPlusYears = amountOfDaysForStartedToWork;

                       // Должно быть несколько проверок:
                       // 1) является ли год, в котором человек устроился на работу выскокосным
                       // 2) если да то сравнить дату, когда человек устроился на работу с номером дня 29 февраля
                       // 3) если позже 29 февраля, то +365
                       // 4) если раньше, то +366
                       // 5) если год не високосный
                       // 6) является ли следующий год високосным
                       // 7) если да, сравнить дату, когда человек устроился на работу с 28 феврался
                       // 8) если позже +366
                       // 9) если раньше или = +365
                       // 10) увеличить год на 1
                       // Это все делается для того, что бы

                       while(i>amountOfDaysForStartedToWorkPlusYears) {
                           if(currentYear2 % 400 == 0 || (currentYear2 % 4 == 0 && currentYear2 % 100 != 0)) {
                                if(amountOfDaysForStartedToWorkPlusYears >  amountOfDaysEveryYear + LastFebraryDays[iterator]) {
                                    amountOfDaysForStartedToWorkPlusYears +=365;
                                }
                                else amountOfDaysForStartedToWorkPlusYears+=366;
                           }
                           else {
                               if((currentYear2 + 1)% 400 == 0 || ((currentYear2 +1) % 4 == 0 && (currentYear2 +1 )% 100 != 0)) {
                                   if(amountOfDaysForStartedToWorkPlusYears >  amountOfDaysEveryYear + LastFebraryDays[iterator]) {
                                       amountOfDaysForStartedToWorkPlusYears +=366;
                                   }
                                   else amountOfDaysForStartedToWorkPlusYears +=365;
                               }
                               else amountOfDaysForStartedToWorkPlusYears +=365;
                           }
                            currentYear2++;
                            if(i>=amountOfDaysForStartedToWorkPlusYears) numberOfWorkedYears++;
                            iterator++;
                       }
                       qDebug() << numberOfWorkedYears;
                       if(a->typeOfWorker == "Employee") {
                           if(numberOfWorkedYears > 10) numberOfWorkedYears =10;
                           result = a->baseRate + 0.03*numberOfWorkedYears*a->baseRate;
                       }

                       else if(a->typeOfWorker == "Manager") {
                           if(numberOfWorkedYears > 8) numberOfWorkedYears =8;
                           quantityOfSubodinates =0;
                           for(auto &a: workers ) {
                               if(a->chiefId == mainChiefId) {
                                   quantityOfSubodinates++;
                               }
                           }
                           result = a->baseRate + 0.05*numberOfWorkedYears*a->baseRate+0.005*a->baseRate*quantityOfSubodinates; // Добавить заплату за всех подчиненных первого уровня!!!
                           // result = a->baseRate + 0.05*numberOfWorkedYears*a->baseRate;
                           qDebug() << a->baseRate;
                           qDebug() << quantityOfSubodinates;

                       }
                       else if(a->typeOfWorker == "Sales") {
                           // Тут посчитать количество подчиненных
                           // У меня есть массив со всеми подчиненными и человеком, под логином которого я зашел
                           // Необходимо для человека, имя которого я написал в поле для расчета з/п определить подчиненных всех уровней

                            vector<int> subChiefIds;
                            subChiefIds.push_back(mainChiefId);
                            unsigned long long subChiefIdsMainSize = subChiefIds.size();
                            unsigned long long quantityOfSubForDelete;
                            unsigned long long subChiefIdsMainSize2 =0;
                            quantityOfSubodinates = 0;

                           while (true) {
                               quantityOfSubForDelete = 0;
                                for(unsigned long long a = 0; a < subChiefIdsMainSize; a++) {
                                   for(auto &b : workers) {
                                       if(subChiefIds[a] == b->chiefId) {
                                           quantityOfSubodinates++;
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
                                if(subChiefIdsMainSize == 0) {
                                    break;
                                }
                                 subChiefIdsMainSize = subChiefIds.size();
                           }

                           qDebug() << quantityOfSubodinates;

                           if(numberOfWorkedYears > 35) numberOfWorkedYears =35;
                           result = a->baseRate + 0.01*numberOfWorkedYears*a->baseRate + a->baseRate*0.003*quantityOfSubodinates; // Добавить заплату за всех подчиненных!!!
                       }
                       salary+=result;
                       qDebug() << "Результат = " << result;
                       qDebug() << "Итого = " << salary;
                   }
           }
       }    // if(name == a->name)
    }

    return QString::number(salary);
}
