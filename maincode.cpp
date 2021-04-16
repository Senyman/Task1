#include "maincode.h"

class Worker
{
public:
    int id;
    QString name;
    QString firstDayDate;
    int baseRate = 1000;
    QString typeOfWorker;
    QString login;
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


MainCode::MainCode(QObject *parent) : QObject(parent)
{
  count = 0;
}

void MainCode::logIn(QString login, QString password) {                                                            // Вход по логину и паролю
    {
        count = 0;
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
                    count++;
                }

                if(count==1) {
                 emit  openMainMenu(nameForQML, loginForQML); // Добавить сигнал для открытия второго окна
#ifdef DEBUG
                qDebug() << "Your login and password are correct ";
                qDebug() << login;  // Отладочная
                qDebug() << password;
                emit  sendErrorMessage("Работает!!!");

#endif
                }
                if(count<1)  {
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

void MainCode::ReceiveDataFromQMLforCountSalary(QString beginDate, QString endDate, QString name)
{
    vector<shared_ptr<Worker>> workers = {      // Массив, который хранит элементы типа <shared_ptr<Worker>>
//        make_shared<Employee>( "Иван"),
//        make_shared<Manager>("Сергей"),
//        make_shared<Sales>("Дмитрий")
    };

for( auto &a: workers) {
   if(name == a->name) {

       emit sendSalaryToQML(a->CountSalary(5,1000, beginDate, endDate));
   }
   else qDebug() << "Ничего не считал, т.к. ошибка ввода";
}
}

int CounterWorkDays(QString beginDate, QString endDateFromQML)
{
    int amountOfWorkingDays = 0;
    setlocale(LC_ALL, "rus");
    // system("cls");
    bool badenter = false;
    QString date;
    QString endDate;
    string my = "aaa";  ////////////////////////////////////////////////
    int dayBegin, monthBegin, yearBegin;
    int dayEnd, monthEnd, yearEnd;
    int mDays[12] = { 31,28,31,30,31,30,31,31,30,31,30,31 };

     qDebug() << "Данная программа считает количество рабочих дней за введенный период";

    // Ввод начальной даты и проверка
    do
    {
        qDebug() << "Введите начальную дату в формате дд.мм.гггг";
        //cin >> date;
        date = beginDate;
        if (date.length() != 10 || date[2] != '.' || date[5] != '.' || date[1] == 0 || !date[0].isDigit() || !date[1].isDigit() || !date[3].isDigit() || !date[4].isDigit()
            || !date[6].isDigit() || !date[7].isDigit() || !date[8].isDigit() || !date[9].isDigit())
        {
            badenter = true;
            qDebug() << "Вы ввели неправильную дату или вообще не дату";
            amountOfWorkingDays = 0;
             return amountOfWorkingDays;
        }
        else
        {
//            dayBegin = ((int)date[0] - 48) * 10 + (int)date[1] - 48;
//            monthBegin = ((int)date[3] - 48) * 10 + ((int)date[4] - 48);
//            yearBegin = ((int)date[6] - 48) * 1000 + ((int)date[7] - 48) * 100 + ((int)date[8] - 48) * 10 + ((int)date[9] - 48);

            dayBegin = (date[0].unicode() - 48) * 10 + date[1].unicode()-48;
            monthBegin = (date[3].unicode()-48) * 10 + (date[4].unicode()-48);
            yearBegin = (date[6].unicode()-48) * 1000 + (date[7].unicode()-48) * 100 + (date[8].unicode()-48) * 10 + (date[9].unicode()-48);

            if (yearBegin < 1970 || yearBegin >= 2038 || monthBegin < 1 || monthBegin >12 || dayBegin < 1 || dayBegin >31 || dayBegin > mDays[monthBegin - 1])
            {
                badenter = true;
                qDebug() << "Вы ввели неправильную дату";
                amountOfWorkingDays = 0;
                 return amountOfWorkingDays;
            }
            else
            {
                qDebug() << "Вы ввели дату: " << dayBegin << "." << monthBegin << "." << yearBegin;
                badenter = false;
            }
        }
    } while (badenter);

    // Ввод конечной даты и проверка
    do
    {
        qDebug() << "Введите конечную дату в формате дд.мм.гггг";
        //cin >> date;
        endDate = endDateFromQML;
        if (endDate.length() != 10 || endDate[2] != '.' || endDate[5] != '.' || endDate[1] == 0 || ! endDate[0].isDigit() || ! endDate[1].isDigit() || ! endDate[3].isDigit() || ! endDate[4].isDigit()
            || ! endDate[6].isDigit() || ! endDate[7].isDigit() || ! endDate[8].isDigit() || ! endDate[9].isDigit())
        {
            badenter = true;
            qDebug() << "Вы ввели неправильную дату или вообще не дату";
            amountOfWorkingDays = 0;
            return amountOfWorkingDays;
        }
        else
        {
            dayEnd = (  endDate[0].unicode() - 48) * 10 +   endDate[1].unicode() - 48;
            monthEnd = ( endDate[3].unicode() - 48) * 10 + ( endDate[4].unicode() - 48);
            yearEnd = (  endDate[6].unicode() - 48) * 1000 + (  endDate[7].unicode() - 48) * 100 + (  endDate[8].unicode() - 48) * 10 + (  endDate[9].unicode() - 48);
            if (yearEnd < 1970 || yearEnd >= 2038 || monthEnd < 1 || monthEnd >12 || dayEnd < 1 || dayEnd >31 || dayEnd > mDays[monthBegin - 1])
            {
                badenter = true;
                qDebug() << "Вы ввели неправильную дату";
                amountOfWorkingDays = 0;
                return amountOfWorkingDays;
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
                            amountOfWorkingDays = 0;
                            return amountOfWorkingDays;
                        }
                    }
                }
                else
                {
                    badenter = true;
                    qDebug() << "Вы ввели неправильную дату";
                    amountOfWorkingDays = 0;
                    return amountOfWorkingDays;
                }

            }
        }
    } while (badenter);

    // Надо посчитать количество дней от 1970 до начала отрезка, для которого необходимо посчитать количество рабочих дней
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
    qDebug() << "Количество дней с начала 1970 года: " << amountOfDays;

    // Необходимо вычислить с какого дня начинается год, в которм начинается период расчета рабочих дней, что бы для этого года и всех последующих рассчитать праздничные дни
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

    // Находим праздничные дни в годах, для которых необходимо посчитать рабочие дни
    //int counterDays = 0;
    //int counterYears = 0;
    //int myyear = 1970;
    //int lostdays = 0;
    //int resultDays = 0;
    //int daysformonth = 31;
    //int monthscounter = 1;
    //int resultmonth = 0;
    //int monththis = 1;
    //bool result = false;
    //int febrarydays;
    //for (int i = 0; i < HolidaysCount; i++) 	// Добавить дату дня в строку после номера дня
    //{
    //	monthscounter = 1;
    //	result = false;
    //	cout << i << "\t День под номером: " << mHolidays[i] << " является праздничным. Дата: ";

    //	while (true)		// Высчитываем дату дня в строку после номера дня
    //	{
    //		if (counterDays < mHolidays[i])	// Если количество дней меньше номера текущего дня
    //		{
    //			resultDays = counterDays;	// ?
    //			counterYears = myyear;				// Прибавляем к текущему году начина с 1970 по году

    //			if (myyear % 400 == 0 || (myyear % 4 == 0 && myyear % 100 != 0))	// Если текущий год високосный, то
    //			{
    //				counterDays += 366;		// прибавляем к нему 366 дней
    //			}
    //			else
    //			{
    //				counterDays += 365;
    //			}
    //			myyear++;					// Прибалсяем к текущему году по году

    //		}
    //		else  // Если количество дней больше чем количество дней в текущем дне начиная с 1970
    //		{
    //			lostdays = mHolidays[i] - resultDays;	// Узнаем, сколько дней осталось

    //			while (true)
    //			{
    //				for (int i = 0; i < 12; i++)		// Для оставшихся дней считаем количество полных месяцев
    //				{
    //					if (i == 1)
    //					{
    //						if (counterYears % 400 == 0 || (counterYears % 4 == 0 && counterYears % 100 != 0))
    //						{
    //							daysformonth = 29;
    //						}
    //						else
    //						{
    //							daysformonth = mDays[i];
    //						}
    //					}
    //					else
    //					{
    //						daysformonth = mDays[i];
    //					}

    //					if (daysformonth > lostdays)
    //					{
    //						cout << lostdays << "." << monthscounter << "." << counterYears << endl;
    //						result = true;
    //						break;
    //					}
    //					else
    //					{
    //						monthscounter++;
    //						lostdays -= daysformonth;
    //					}
    //				}
    //				if (result) break;
    //			}
    //		}
    //		if (result) break;
    //	}
    //}

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

    // Считаем количество дней от 1970 до конца отрезка, для которого нужно посчитать количество рабочих дней
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
    qDebug() << "Количество дней с начала 1970 года: " << amountOfDaysEndDate;

    // Определяем день недели конца отрезка, для которого необходимо посчитать количество рабочих дней (не обязательно)
    QString dayOfWeekEndDate;
    for (int i = 0, j = 3; i < amountOfDaysEndDate; i++)
    {
        dayOfWeekEndDate = weekList[j];
        if (j % 6 == 0 && j != 0)
        {
            j = 0;
        }
        else
        {
            j++;
        }

    }
    qDebug() << "День недели конечного дня: " << dayOfWeekEndDate;

    // Считаем количество рабочих дней
    bool thisDayisHoliday = false;
    for (int i = amountOfDays, j = dayOfWeekBegin; i < amountOfDaysEndDate + 1; i++)	// amountOfDays - количество дней до начала отрезка, в котором считаю количество рабочих дней
    {
        if (j != 5 && j != 6)		// Если текущий день рабочий
        {
            for (int k = 0; k < HolidaysCount; k++)
            {
                if (i != mHolidays[k])	// Если текущий день не праздничный и не рабочий
                {
                    thisDayisHoliday = false;
                }
                else
                {
                    thisDayisHoliday = true;
                    break;
                }
            }
            if (!thisDayisHoliday)
            {
                amountOfWorkingDays++;
            }
        }
        else  // Если текущий день не рабочий, то нужно проверить, праздничный ли
        {
            for (int k = 0; k < HolidaysCount; k++)
            {
                if (i != mHolidays[k])	// Если текущий день не праздничный
                {
                    thisDayisHoliday = false;
                }
                else
                {
                    thisDayisHoliday = true;
                    mHolidays[k] ++;
                    break;
                }
            }
        }

        if (j % 6 == 0 && j != 0)
        {
            j = 0;
        }
        else
        {
            j++;
        }
    }

    qDebug() << "Количество рабочих дней = " << amountOfWorkingDays;
    return amountOfWorkingDays;
}

void MainCode::createWorkers() {
    vector<shared_ptr<Worker>> workers = {};   // Массив, который хранит элементы типа <shared_ptr<Worker>>

    int idForArr;                                                    // Переменные для занесения в массив с работниками
    QString nameForArr;
    QString firstDayDateForArr;
    int baseSalaryForArr;
    QString typeOfWorkerForArr;
    QString loginForArr;
    int chiefIdForArr;
    {
        count = 0;
        QSqlDatabase workersDB = QSqlDatabase::addDatabase("QSQLITE");                              // Создаем объект для работы с базой данных
        workersDB.setDatabaseName("C://Users//User//Desktop//WorkersDB//Workers.db");   // Указываю путь к базе данных
        if(!workersDB.open()) qDebug() << "Failed to open database";
        else qDebug() << "DataBase is connected";
        QSqlQuery qry;                                                                                                                        // Объект для запроса информации из БД

        if(qry.exec("SELECT * FROM WorkersTable ")) {
                while(qry.next()) {
                    idForArr = qry.value(0).toInt();
                    nameForArr = qry.value(1).toString();
                    firstDayDateForArr = qry.value(2).toString();
                    baseSalaryForArr = qry.value(3).toInt();
                    typeOfWorkerForArr = qry.value(4).toString();
                    loginForArr = qry.value(5).toString();
                    chiefIdForArr = qry.value(6).toInt();
                    count++;
                    // qDebug() << idForArr <<" "<< nameForArr <<" "<<firstDayDateForArr << " "<<baseSalaryForArr <<" "<<typeOfWorkerForArr << " "<<loginForArr;

                    if(typeOfWorkerForArr == "Employee") workers.push_back(make_shared<Employee>( idForArr, nameForArr, firstDayDateForArr, baseSalaryForArr, typeOfWorkerForArr, loginForArr, chiefIdForArr));
                    else if (typeOfWorkerForArr == "Sales") workers.push_back(make_shared<Sales>( idForArr, nameForArr, firstDayDateForArr, baseSalaryForArr, typeOfWorkerForArr, loginForArr, chiefIdForArr));
                    else if(typeOfWorkerForArr == "Manager") workers.push_back(make_shared<Manager>( idForArr, nameForArr, firstDayDateForArr, baseSalaryForArr, typeOfWorkerForArr, loginForArr, chiefIdForArr));
                }
        }
        workersDB.close();
    }
    QSqlDatabase::removeDatabase("qt_sql_default_connection");
    for( auto &a: workers) {
        // qDebug() << a->id << " " << a->name<< ""<<a->firstDayDate << " " << a->baseRate << " " << a->typeOfWorker << " " << a->login << " " << a->chiefId<< " " ;
    }
}
