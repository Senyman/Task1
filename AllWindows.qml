import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Window 2.12
import QtQuick.Layouts 1.12


Window {

    id: allWindows
    maximumWidth: 950
    minimumWidth: 950
    maximumHeight: 800
    minimumHeight: 800
    title: qsTr("Приложение")
    color: "#1B242F"

    signal goToLoginWindow
    signal clearListModelsFromAllWindows        // Для очистки listModels в SubordinatesPage и выпадающего списка в AddWorkerPage
    property int myMargin: 10

    Connections {
        target: MainCode
        onOpenMainMenu: {
            mainMenuPage.logintext = "Логин: " + loginForQML
            mainMenuPage.nametext = "Имя: " + nameForQML
        }
        onSendSalaryToQML: calculateSalaryPage.salary = resultSalary                                  // Для расчета зарплаты
        onSendSalaryOfAllWorkersToQML: calculateSalaryPage.salary = resultSalary             // Для расчета зарплаты всех сотрудников
    }

    Item {  // Для возвращения назад (Esc) и очищения поля с рассчитаной зарплатой
        id: onPressedItem
        focus: true
        anchors.fill: parent
        Keys.onEscapePressed: {
            (stackView.currentItem === calculateSalaryPage)  ? calculateSalaryPage.salary = "" : "" ;

            (stackView.currentItem === addWorkerPage)  ? addWorkerPage.nameForDBp = "" : ""  ;
            (stackView.currentItem === addWorkerPage)  ? addWorkerPage.nameErrorMesp = "" : "";
            (stackView.currentItem === addWorkerPage)  ? addWorkerPage.beginDateForDBp = "" : "";
            (stackView.currentItem === addWorkerPage)  ? addWorkerPage.beginDateErrorMesp = "" : "";
            (stackView.currentItem === addWorkerPage)  ? addWorkerPage.baseRateForDBp = "" : "";
            (stackView.currentItem === addWorkerPage)  ? addWorkerPage.baseRateErrorMesp = "" : "";
            (stackView.currentItem === addWorkerPage)  ? addWorkerPage.typeForDBp = "Тип сотрудника" : "";
            (stackView.currentItem === addWorkerPage)  ? addWorkerPage.typeErrorMesp = "" : "";
            (stackView.currentItem === addWorkerPage)  ? addWorkerPage.loginForDBp = "" : "";
            (stackView.currentItem === addWorkerPage)  ? addWorkerPage.loginErrorMesp = "" : "";
            (stackView.currentItem === addWorkerPage)  ? addWorkerPage.firstPasswordForDBp = "" : "";
            (stackView.currentItem === addWorkerPage)  ? addWorkerPage.firstPasswordErrorMesp = "" : "";
            (stackView.currentItem === addWorkerPage)  ? addWorkerPage.secondPasswordForDBp = "" : "";
            (stackView.currentItem === addWorkerPage)  ? addWorkerPage.secondPasswordErrorMesp = "" : "";
            (stackView.currentItem === addWorkerPage)  ? addWorkerPage.chiefForDBp = "Начальник" : "";
            (stackView.currentItem === addWorkerPage)  ? addWorkerPage.chiefIdErrorMesp = "" : "";
            (stackView.currentItem === addWorkerPage)  ? addWorkerPage.superuserForDBp = "Суперпользователь" : "";
            (stackView.currentItem === addWorkerPage)  ? addWorkerPage.superuserErrorMesp = "" : "";
            (stackView.currentItem === addWorkerPage)  ? addWorkerPage.commonErrorMesp = "" : "";

            (stackView.depth  === 1 ) ? allWindows.goToLoginWindow() : stackView.pop();
            mainMenuPage.errorTextFromMainMenu = " "
        }
        Keys.onReturnPressed: {
            (stackView.currentItem === calculateSalaryPage)  ?  MainCode.receiveDataFromQMLforCountSalary(calculateSalaryPage.beginDateText, calculateSalaryPage.endDateText, calculateSalaryPage.idText ):  "" ;
            (stackView.currentItem === addWorkerPage)  ?  (MainCode.addWorker(addWorkerPage.nameForDBp, addWorkerPage.beginDateForDBp, addWorkerPage.baseRateForDBp,
                                                                             addWorkerPage.typeForDBp, addWorkerPage.loginForDBp, addWorkerPage.firstPasswordForDBp,
                                                                             addWorkerPage.secondPasswordForDBp, addWorkerPage.chiefForDBp, addWorkerPage.superuserForDBp),
                                                           MainCode.fillChiefListModel(), addWorkerPage.chiefForDBp = "Начальник", addWorkerPage.nameErrorMesp = "",
                                                           addWorkerPage.beginDateErrorMesp = "", addWorkerPage.baseRateErrorMesp = "" , addWorkerPage.typeErrorMesp = "",
                                                            addWorkerPage.loginErrorMesp = "", addWorkerPage.firstPasswordErrorMesp = "", addWorkerPage.secondPasswordErrorMesp = "" ,
                                                           addWorkerPage.chiefIdErrorMesp = "", addWorkerPage.superuserErrorMesp = "") :  "" ;
        }
    }

    StackView {
        id: stackView
        anchors.fill: parent
        initialItem: mainMenuPage           // Запускаем первую страницу
    }

    MainMenuPage {                              // Страница главного меню
        id: mainMenuPage
        onClearListModel: clearListModelsFromAllWindows();
        onClearChiefIdListModel: clearListModelsFromAllWindows();
    }

    CalculateSalaryPage {                      // Страница для расчета зарплаты
        id: calculateSalaryPage
    }

    SubordinatesPage {                          // Страница со списком подчиненных
        id: subordinatesPage
    }

    AddWorkerPage {                            // Страница для добавления нового сотрудника
        id: addWorkerPage
    }
}

