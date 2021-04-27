import QtQuick 2.12
import QtQuick.Controls 2.12    // Для StackView
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

    property int myMargin: 10

    Connections {                        // Для связи C++ и qml
        target: MainCode              // Связываем
        onSendSalaryToQML: {
            salary.text = resultSalary  // Сюда задается текст
        }

        onSendSalaryOfAllWorkersToQML: {
            salary.text = resultSalary
        }

        onOpenMainMenu: {
            loginText.text = "Логин: " + loginForQML
            nameText.text = "Имя: " + nameForQML
        }
        onSendSubordinatesInfoToQML: {
            listModel.append({idOfSubordinate: "Id: " + idSub, nameOfSubordinate: "Имя: " + nameSub, typeOfSubordinate: "Тип рабочего: " + typeOfWorkerSub,
                                 firstDayDateOfSubordinate: "Дата первого рабочего дня: " + firstDayDateSub, baseRateOfSubordinate: "Дневная ставка: " + baseRateSub + " руб.",
                                 chiefIdOfSubordinate: "Id начальника: " + chiefIdSub, levelOfSubordinate: "Уровень подчиненного: " + levelOfSubStr});
        }

        onSendErrorMessageForName: {
            nameErrorMes.text = errorMsg
        }
    }

    StackView {
        id: stackView
        anchors.fill: parent
        initialItem: mainMenuPage     // Запускаем первую страницу
    }

    TemplatePage {                            // Страница главного меню
        id: mainMenuPage
        ColumnLayout {
            id: mainMenuLinker
            anchors.centerIn: parent

            CommonButton {               // Открыть окно расчета з/п
                id: goToCountSalaryWindowButton
                width: 300
                height: 35
                text: "Расчитать зарплату"
                fontSize: 10
                onClicked: {
                    stackView.push(calculateSalaryPage);
                }
           }

            CommonButton {               // Показать подчиненных
                id: goToSubordinateWindowButton
                width: 300
                height: 35
                text: "Показать подчиненных"
                fontSize: 10
                onClicked: {
                    stackView.push(subordinatesPage);
                    MainCode.findSubordinates()
                }
           }

            CommonButton {             // Добавить рабочего
                id: goToAddWorkersWindowButton
                width: 300
                height: 35
                text: "Добавить рабочего"
                fontSize: 10
                onClicked: {
                        stackView.push(addWorkerPage);
                }
           }

            CommonButton {            // Выход в окно регистрации
                id: goToLoginWindowButton
                width: 300
                height: 35
                text: "Выйти"
                fontSize: 10
                onClicked: {
                    allWindows.goToLoginWindow()
                }
           }
        }

        Item {
            height: 50
            width: 300
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            anchors.leftMargin: 10

            ColumnLayout {
                Text {
                    id: loginText
                    font.pointSize: 12
                    //text: qsTr("Логин: Канвальд")
                    color: "#aeb0b6"
                }

                Text {
                    id: nameText
                    font.pointSize: 12
                   // text: qsTr("Имя: Йонас")
                    color: "#aeb0b6"
                }

            }
        }
    }

    TemplatePage {                            // Страница расчета зарплаты
        id: calculateSalaryPage
        visible: false

        CommonButton {                    // Кнопка для возвращения в главное меню
            id: showMainMenu
            text: "<"
            width: allWindows.width /16
            height: allWindows.width / 16
            color: "#00000000"
            onClicked: {
                stackView.pop()
            }
        }
        ColumnLayout {              // Слой с местом для ввода имени, дат, пояснительного текста
            id: generalLinker
            anchors.top: showMainMenu.bottom
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.topMargin: allWindows.height / 6
            width: allWindows.width
            anchors.bottom: parent.bottom

            ColumnLayout {                  // Entry field basically
                Layout.fillWidth: true
                Layout.alignment: Qt.AlignLeft
                Layout.leftMargin: 200

                Label {

                    //anchors.right: Layout.right
                    text: "Введите имя"
                    color: "#aeb0b6"
                    font.pointSize: 12  // fontsize
                    font.bold: true
                }
                TextField {
                    id: name
                    placeholderText: "Имя"
                    //Layout.fillWidth: true
                }

                Label {
                    text: "Введите даты"
                    color: "#aeb0b6"
                    font.pointSize: 12  // fontsize
                    font.bold: true
                }
                TextField {
                    id: beginDate
                    placeholderText: "XX.XX.XXXX"
                    //Layout.fillWidth: true
                }
                TextField {
                    id: endDate
                    placeholderText: "XX.XX.XXXX"
                    //Layout.fillWidth: true
                }
                Label {
                    id: salary
                    color: "#aeb0b6"
                    font.pointSize: 14  // fontsize
                    font.bold: true
                }

            }

            RowLayout{                  // Кнопки для расчета заработной платы за определенный период
                id: rowLayout
                Layout.alignment: Qt.AlignBottom
                Layout.bottomMargin: 20
                height: 50

                Item {                  // Spring for buttons
                    Layout.fillWidth: true
                }

                CommonButton {          // Расчет зарплаты всех сотрудников
                    id: calculateAllSalary
                    width: 100
                    height: 40
                    text: "Calculate salary for all"
                    onClicked: {
                        MainCode.receiveDataFromQMLforCountSalaryForAll(beginDate.text, endDate.text)
                    }
                }

                CommonButton {          // Расчет зарплаты сотрудника
                    id: calculateSalary
                    width: 100
                    height: 40
                    text: "Calculate"
                    Layout.rightMargin: 20
                    onClicked: {
                        MainCode.receiveDataFromQMLforCountSalary(beginDate.text, endDate.text, name.text )
                    }
                }
             }
        }   // Column
    }

    TemplatePage {                            // Страница со списком подчиненных
        id: subordinatesPage
        visible: false

        ColumnLayout {
            id: subordinatesColumn
            anchors.fill: parent
            spacing: 0

            Rectangle {
                color: "#1B242F"
                z: +1
                Layout.fillWidth: subordinatesColumn
                height: 200
                CommonButton {                    // Кнопка для возвращения в главное меню
                    id: showMainMenuFromSubordinatesPage
                    text: "<"
                    width: allWindows.width /16
                    height: allWindows.width / 16
                    color: "#00000000"

                    onClicked: {
                        listModel.clear()
                        stackView.pop()
                    }
                }

                Item {
                    width: parent.width
                    height: 50
                }
                Label {
                    text: "Список подчиненных"
                    color: "#aeb0b6"
                    font.pointSize: 16
                    font.bold: true
                    z: 1
                    anchors.centerIn: parent
                }


                Item {
                    Layout.fillWidth: subordinatesColumn
                    height: 100

                }
            }

            ListView {      // Список всех подчиненных всех уровней

                id: listSubordinates
                Layout.fillHeight: true
                // Layout.fillWidth: true
                Layout.alignment: Qt.AlignHCenter
                width: 500
                spacing: 6
                ScrollBar.vertical: ScrollBar {}
                z: -1


                Rectangle {
                    anchors.fill: listSubordinates
                    border.width: 1
                    border.color: "black"
                    color: "transparent"
                }
                model: listModel
                delegate: Rectangle {

                    id: mainRec
                    height: 200
                    anchors.left: parent.left
                    anchors.right: parent.right
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    // anchors.margins:  300
                    color: "#213141"
                    border.color: "black"

                    TextForSubordinates {
                        id: idText
                        anchors.top:  mainRec.top
                        text: model.idOfSubordinate
                        anchors.topMargin: 20
                    }
                    TextForSubordinates {
                        id: textName
                        anchors.top: idText.bottom
                        text: model.nameOfSubordinate
                    }
                    TextForSubordinates {
                        id: textType
                        anchors.top: textName.bottom
                        text: model.typeOfSubordinate
                    }
                    TextForSubordinates {
                        id: textBeginData
                        anchors.top: textType.bottom
                        text: model.firstDayDateOfSubordinate
                    }
                    TextForSubordinates {
                        id: textRate
                        anchors.top: textBeginData.bottom
                        text: model.baseRateOfSubordinate
                    }
                    TextForSubordinates {
                        id: chiefId
                        anchors.top: textRate.bottom
                        text: model.chiefIdOfSubordinate
                    }
                    TextForSubordinates {
                        id: level
                        anchors.top: chiefId.bottom
                        text: model.levelOfSubordinate
                    }
                }
            }

            Rectangle {
                color: "#1B242F"
                z: +1
                Layout.fillWidth: subordinatesColumn
                height: 100

            }
        }

        ListModel { id: listModel }
    }

    TemplatePage {                            // Страница для добавления нового сотрудника
        id: addWorkerPage
        visible: false

        CommonButton {                    // Кнопка для возвращения в главное меню
            id: showMainMenuFromAddWorkerPage
            text: "<"
            width: allWindows.width /16
            height: allWindows.width / 16
            color: "#00000000"
            onClicked: {
                stackView.pop()
            }
        }
        ColumnLayout {              // Слой с местом для ввода информации и кнопок
            id: mainLinker
            anchors.top: showMainMenuFromAddWorkerPage.bottom
            anchors.horizontalCenter: parent.horizontalCenter
            // anchors.topMargin: allWindows.height / 6
            width: allWindows.width
            anchors.bottom: parent.bottom

            ColumnLayout {                  // Поля для ввода
                Layout.fillWidth: true
                Layout.alignment: Qt.AlignLeft
                Layout.leftMargin: 200

                Label {

                    //anchors.right: Layout.right
                    text: "Введите данные"
                    color: "#aeb0b6"
                    font.pointSize: 16  // fontsize
                }
                Item {
                    height: 15
                }



                TemplateTextField {
                    id: nameForDB
                    placeholderText: "Имя"
                }
                TemplateLabelForAddWorkerPage {
                    id: nameErrorMes
                    text: "Неправильно введено имя"
                }



                TemplateTextField {
                    id: beginDateForDB
                    placeholderText: "Дата устройства на работу"
                }
                TemplateLabelForAddWorkerPage {
                    id: dateErrorMes
                    text: "Дата должна быть в формате дд.мм.гггг"
                }



                TemplateTextField {
                    id: baseRateForDB
                    placeholderText: "Базовая ставка"
                }
                TemplateLabelForAddWorkerPage {
                    id: rateErrorMes
                    text: "Нельзя вводить буквы"
                }


                ComboBox {
                    id:typeForDB2
                    Layout.maximumHeight: 22
                    Layout.minimumWidth: 250
                    font.pointSize: 8
                    model: ListModel {
                        ListElement {
                            text: "-"
                        }
                        ListElement {
                            text: "Employee"
                        }
                        ListElement {
                            text: "Manager"
                        }
                        ListElement {
                            text: "Sales"
                        }
                    }
                }

                TemplateLabelForAddWorkerPage {
                    id: typeErrorMes
                    text: "Выберите тип"
                }



                TemplateTextField {
                    id: loginForDB
                    placeholderText: "Логин"
                }
                TemplateLabelForAddWorkerPage {
                    id: loginErrorMes
                    text: "Слишком длинный логин"
                }



                TemplateTextField {
                    id: firstPasswordForDB
                    placeholderText: "Пароль"
                }
                TemplateLabelForAddWorkerPage {
                    id: firstPasswordErrorMes
                    text: "Слишком длинный пароль"
                }


                TemplateTextField {
                    id: secondPasswordForDB
                    placeholderText: "Повторите пароль"
                }
                TemplateLabelForAddWorkerPage {
                    id: secondPasswordErrorMes
                    text: "Пароли не совпадают"
                }



                TemplateTextField {
                    id: chiefForDB
                    placeholderText: "Начальник"
                }
                TemplateLabelForAddWorkerPage {
                    id: chiefErrorMes
                    text: "Выберите начальника"
                }


                ComboBox {
                    id:superuserForDB
                    Layout.maximumHeight: 22
                    Layout.minimumWidth: 250
                    font.pointSize: 8
                    model: ListModel {
                        ListElement {
                            text: "-"
                        }
                        ListElement {
                            text: "Да"
                        }
                        ListElement {
                            text: "Нет"
                        }
                    }
                }

                TemplateLabelForAddWorkerPage {
                    id: superuserErrorMes
                    text: "Выберите суперпользователя"
                }

            }

            RowLayout{                  // Кнопки для расчета заработной платы за определенный период
                id: layoutWithButtons
                Layout.alignment: Qt.AlignBottom
                Layout.bottomMargin: 20
                height: 50

                Item {                  // Spring for buttons
                    Layout.fillWidth: true
                }

                CommonButton {          // Расчет зарплаты всех сотрудников
                    id: clearFields
                    width: 130
                    height: 40
                    text: "Очистить поля"
                    onClicked: {
                        // MainCode.receiveDataFromQMLforCountSalaryForAll(beginDate.text, endDate.text)
                    }
                }

                CommonButton {          // Расчет зарплаты сотрудника
                    id: addWorker
                    width: 130
                    height: 40
                    text: "Добавить рабочего"
                    Layout.rightMargin: 20
                    onClicked: {
                        MainCode.addWorker(nameForDB.text, beginDateForDB.text, baseRateForDB.text, typeForDB.text,
                                           loginForDB.text, firstPasswordForDB.text, secondPasswordForDB.text, chiefForDB.text, superuserForDB.text)
                    }
                }
             }
        }   // Column
    }

}


