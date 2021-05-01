import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Window 2.12
import QtQuick.Layouts 1.12


TemplatePage {  // Страница для добавления нового сотрудника
    property alias nameForDBp: nameForDB.text
    property alias nameErrorMesp: nameErrorMes.text
    property alias beginDateForDBp: beginDateForDB.text
    property alias beginDateErrorMesp: beginDateErrorMes.text
    property alias baseRateForDBp: baseRateForDB.text
    property alias baseRateErrorMesp: baseRateErrorMes.text
    property alias typeForDBp: typeForDB.displayText
    property alias typeErrorMesp: typeErrorMes.text
    property alias loginForDBp: loginForDB.text
    property alias loginErrorMesp: loginErrorMes.text
    property alias firstPasswordForDBp: firstPasswordForDB.text
    property alias firstPasswordErrorMesp: firstPasswordErrorMes.text
    property alias secondPasswordForDBp: secondPasswordForDB.text
    property alias secondPasswordErrorMesp: secondPasswordErrorMes.text
    property alias chiefForDBp: chiefForDB.displayText
    property alias chiefIdErrorMesp: chiefIdErrorMes.text
    property alias superuserForDBp: superuserForDB.displayText
    property alias superuserErrorMesp: superuserErrorMes.text
    property alias commonErrorMesp: commonErrorMes.text


    id: addWorkerPage
    visible: false

    Connections {
      target: MainCode
      onSendWorkersInfoToQML: chiefIdListModel.append({ workersInfo })
      onSendErrorMessageForName: nameErrorMes.text = errorMsg
      onSendErrorMessageForBeginDate: beginDateErrorMes.text = errorMsg
      onSendErrorMessageForBaseRate: baseRateErrorMes.text = errorMsg
      onSendErrorMessageForType: typeErrorMes.text = errorMsg
      onSendErrorMessageForLogin: loginErrorMes.text = errorMsg
      onSendErrorMessageForFirstPassword: firstPasswordErrorMes.text = errorMsg
      onSendErrorMessageForSecondPassword: secondPasswordErrorMes.text = errorMsg
      onSendErrorMessageForChief: chiefIdErrorMes.text = errorMsg
      onSendErrorMessageForSuperuser: superuserErrorMes.text = errorMsg
      onSendCommonErrorMessage: commonErrorMes.text = errorMsg
    }

    Connections {
        target: allWindows
        onClearListModelsFromAllWindows: {
            chiefIdListModel.clear()
        }
    }

    CommonButton {                          // Для возвращения в главное меню
        id: showMainMenuFromAddWorkerPage
        text: "<"
        width: allWindows.width /16
        height: allWindows.width / 16
        color: "#00000000"
        onClicked: {
            nameForDB.text = ""
            nameErrorMes.text = ""
            beginDateForDB.text = ""
            beginDateErrorMes.text = ""
            baseRateForDB.text = ""
            baseRateErrorMes.text = ""
            typeForDB.displayText = "Тип сотрудника"
            typeErrorMes.text = ""
            loginForDB.text = ""
            loginErrorMes.text = ""
            firstPasswordForDB.text = ""
            firstPasswordErrorMes.text = ""
            secondPasswordForDB.text = ""
            secondPasswordErrorMes.text = ""
            chiefForDB.displayText = "Начальник"
            chiefIdErrorMes.text = ""
            superuserForDB.displayText = "Суперпользователь"
            superuserErrorMes.text = ""
            commonErrorMes.text = ""
            stackView.pop()
        }
    }

    ColumnLayout {                            // Слой с кнопками и местом для ввода информации

        id: mainLinker
        width: allWindows.width
        anchors.bottom: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: showMainMenuFromAddWorkerPage.bottom

        ColumnLayout {                        // Поля для ввода
            Layout.fillWidth: true
            Layout.leftMargin: 200
            Layout.alignment: Qt.AlignLeft

            Label {
                text: "Введите данные"
                color: "#aeb0b6"
                font.pointSize: 16
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
            }
            TemplateTextField {
                id: beginDateForDB
                placeholderText: "Дата устройства на работу: дд.мм.гггг"
            }
            TemplateLabelForAddWorkerPage {
                id: beginDateErrorMes
            }
            TemplateTextField {
                id: baseRateForDB
                placeholderText: "Базовая ставка"
            }
            TemplateLabelForAddWorkerPage {
                id: baseRateErrorMes
            }

            ComboBox {
                id:typeForDB
                Keys.onEscapePressed: {
                    nameForDB.text = ""
                    nameErrorMes.text = ""
                    beginDateForDB.text = ""
                    beginDateErrorMes.text = ""
                    baseRateForDB.text = ""
                    baseRateErrorMes.text = ""
                    typeForDB.displayText = "Тип сотрудника"
                    typeErrorMes.text = ""
                    loginForDB.text = ""
                    loginErrorMes.text = ""
                    firstPasswordForDB.text = ""
                    firstPasswordErrorMes.text = ""
                    secondPasswordForDB.text = ""
                    secondPasswordErrorMes.text = ""
                    chiefForDB.displayText = "Начальник"
                    chiefIdErrorMes.text = ""
                    superuserForDB.displayText = "Суперпользователь"
                    superuserErrorMes.text = ""
                    commonErrorMes.text = ""

                    onPressedItem.focus = true
                    stackView.pop()
                }
                Keys.onReturnPressed: {
                    nameErrorMes.text = ""
                    beginDateErrorMes.text = ""
                    baseRateErrorMes.text = ""
                    typeErrorMes.text = ""
                    loginErrorMes.text = ""
                    firstPasswordErrorMes.text = ""
                    secondPasswordErrorMes.text = ""
                    chiefIdErrorMes.text = ""
                    superuserErrorMes.text = ""

                    MainCode.addWorker(nameForDB.text, beginDateForDB.text, baseRateForDB.text, typeForDB.currentText,
                                       loginForDB.text, firstPasswordForDB.text, secondPasswordForDB.text, chiefForDB.currentText, superuserForDB.currentText)
                    chiefIdListModel.clear()
                    MainCode.fillChiefListModel()
                    chiefForDB.displayText = "Начальник"
                }

                font.pointSize: 8
                Layout.maximumHeight: 30
                Layout.minimumWidth: 250
                displayText: "Тип сотрудника"
                onActivated: displayText = typeForDbListModel.text

                model: ListModel {
                    id: typeForDbListModel
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
            }
            TemplateTextField {
                id: loginForDB
                placeholderText: "Логин"
            }
            TemplateLabelForAddWorkerPage {
                id: loginErrorMes
            }
            TemplateTextField {
                id: firstPasswordForDB
                echoMode: TextInput.Password
                placeholderText: "Пароль"
            }
            TemplateLabelForAddWorkerPage {
                id: firstPasswordErrorMes
            }
            TemplateTextField {
                id: secondPasswordForDB
                echoMode: TextInput.Password
                placeholderText: "Повторите пароль"
            }
            TemplateLabelForAddWorkerPage {
                id: secondPasswordErrorMes
            }

            ComboBox {
                id: chiefForDB
                displayText: "Начальник"
                Layout.maximumHeight: 30
                Layout.minimumWidth: 250

                Keys.onEscapePressed: {
                    nameForDB.text = ""
                    nameErrorMes.text = ""
                    beginDateForDB.text = ""
                    beginDateErrorMes.text = ""
                    baseRateForDB.text = ""
                    baseRateErrorMes.text = ""
                    typeForDB.displayText = "Тип сотрудника"
                    typeErrorMes.text = ""
                    loginForDB.text = ""
                    loginErrorMes.text = ""
                    firstPasswordForDB.text = ""
                    firstPasswordErrorMes.text = ""
                    secondPasswordForDB.text = ""
                    secondPasswordErrorMes.text = ""
                    chiefForDB.displayText = "Начальник"
                    chiefIdErrorMes.text = ""
                    superuserForDB.displayText = "Суперпользователь"
                    superuserErrorMes.text = ""
                    commonErrorMes.text = ""

                    onPressedItem.focus = true
                    stackView.pop()
                }
                Keys.onReturnPressed: {
                    nameErrorMes.text = ""
                    beginDateErrorMes.text = ""
                    baseRateErrorMes.text = ""
                    typeErrorMes.text = ""
                    loginErrorMes.text = ""
                    firstPasswordErrorMes.text = ""
                    secondPasswordErrorMes.text = ""
                    chiefIdErrorMes.text = ""
                    superuserErrorMes.text = ""

                    MainCode.addWorker(nameForDB.text, beginDateForDB.text, baseRateForDB.text, typeForDB.currentText,
                                       loginForDB.text, firstPasswordForDB.text, secondPasswordForDB.text, chiefForDB.currentText, superuserForDB.currentText)

                    chiefIdListModel.clear()
                }

                onActivated: displayText = chiefIdListModel.text

                model: ListModel {
                    id:chiefIdListModel
                }
            }
            TemplateLabelForAddWorkerPage {
                id: chiefIdErrorMes
            }


            ComboBox {
                id:superuserForDB
                font.pointSize: 8
                Layout.maximumHeight: 30
                Layout.minimumWidth: 250
                displayText: "Суперпользователь"

                Keys.onEscapePressed: {
                    nameForDB.text = ""
                    nameErrorMes.text = ""
                    beginDateForDB.text = ""
                    beginDateErrorMes.text = ""
                    baseRateForDB.text = ""
                    baseRateErrorMes.text = ""
                    typeForDB.displayText = "Тип сотрудника"
                    typeErrorMes.text = ""
                    loginForDB.text = ""
                    loginErrorMes.text = ""
                    firstPasswordForDB.text = ""
                    firstPasswordErrorMes.text = ""
                    secondPasswordForDB.text = ""
                    secondPasswordErrorMes.text = ""
                    chiefForDB.displayText = "Начальник"
                    chiefIdErrorMes.text = ""
                    superuserForDB.displayText = "Суперпользователь"
                    superuserErrorMes.text = ""
                    commonErrorMes.text = ""

                    onPressedItem.focus = true
                    stackView.pop()
                }
                Keys.onReturnPressed: {
                    nameErrorMes.text = ""
                    beginDateErrorMes.text = ""
                    baseRateErrorMes.text = ""
                    typeErrorMes.text = ""
                    loginErrorMes.text = ""
                    firstPasswordErrorMes.text = ""
                    secondPasswordErrorMes.text = ""
                    chiefIdErrorMes.text = ""
                    superuserErrorMes.text = ""
                    MainCode.addWorker(nameForDB.text, beginDateForDB.text, baseRateForDB.text, typeForDB.currentText,
                                       loginForDB.text, firstPasswordForDB.text, secondPasswordForDB.text, chiefForDB.currentText, superuserForDB.currentText)

                    chiefIdListModel.clear()
                    MainCode.fillChiefListModel()
                    chiefForDB.displayText = "Начальник"
                }
                onActivated: displayText = superuserListModel.text

                model: ListModel {
                    id: superuserListModel
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
            }
            TemplateLabelForAddWorkerPage {
                id: commonErrorMes
                font.pointSize: 14
            }
        }

        RowLayout{                          // Кнопки для добавления сотрудника и очистки полей
            id: layoutWithButtons
            Layout.alignment: Qt.AlignBottom
            Layout.bottomMargin: 20
            height: 50

            Item {
                Layout.fillWidth: true
            }

            CommonButton {              // Очистить поля
                id: clearFields
                width: 130
                height: 40
                text: "Очистить поля"
                onClicked: {
                    nameForDB.text = ""
                    nameErrorMes.text = ""
                    beginDateForDB.text = ""
                    beginDateErrorMes.text = ""
                    baseRateForDB.text = ""
                    baseRateErrorMes.text = ""
                    typeForDB.displayText = "Тип сотрудника"
                    typeErrorMes.text = ""
                    loginForDB.text = ""
                    loginErrorMes.text = ""
                    firstPasswordForDB.text = ""
                    firstPasswordErrorMes.text = ""
                    secondPasswordForDB.text = ""
                    secondPasswordErrorMes.text = ""
                    chiefForDB.displayText = "Начальник"
                    chiefIdErrorMes.text = ""
                    superuserForDB.displayText = "Суперпользователь"
                    superuserErrorMes.text = ""
                    commonErrorMes.text = ""
                }
            }

            CommonButton {              // Добавить сотрудника
                id: addWorker
                width: 130
                height: 40
                Layout.rightMargin: 20
                text: "Добавить сотрудника"
                onClicked: {
                    chiefForDB.displayText = "Начальник"
                    nameErrorMes.text = ""
                    beginDateErrorMes.text = ""
                    baseRateErrorMes.text = ""
                    typeErrorMes.text = ""
                    loginErrorMes.text = ""
                    firstPasswordErrorMes.text = ""
                    secondPasswordErrorMes.text = ""
                    chiefIdErrorMes.text = ""
                    superuserErrorMes.text = ""
                    MainCode.addWorker(nameForDB.text, beginDateForDB.text, baseRateForDB.text, typeForDB.currentText,
                                       loginForDB.text, firstPasswordForDB.text, secondPasswordForDB.text, chiefForDB.currentText, superuserForDB.currentText)
                    chiefIdListModel.clear()
                    MainCode.fillChiefListModel()
                }
            }
         }
    }
}
