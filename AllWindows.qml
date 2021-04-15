import QtQuick 2.12
import QtQuick.Controls 2.12    // Для StackView
import QtQuick.Window 2.12
import QtQuick.Layouts 1.12

Window {
    id: allWindows
    maximumWidth: 1200
    minimumWidth: 1200
    maximumHeight: 800
    minimumHeight: 800
    title: qsTr("Приложение")
    color: "#1B242F"
    signal goToLoginWindow

    Connections {                        // Для связи C++ и qml
        target: MainCode              // Связываем
        onSendSalaryToQML: {
            salary.text = "Зарплата рабочего составляет: " + count + " рублей "  // Сюда задается текст
        }
        onOpenMainMenu: {
            loginText.text = "Логин: " + loginForQML
            nameText.text = "Имя: " + nameForQML
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
                }
           }

            CommonButton {             // Показать подчиненных
                id: goToAddWorkersWindowButton
                width: 300
                height: 35
                text: "Добавить рабочего"
                fontSize: 10
                onClicked: {

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

                CommonButton {          // Moveback button
                    id: calculateAllSalary
                    width: 100
                    height: 40
                    text: "Calculate all"
                    onClicked: {

                    }
                }

                CommonButton {          // Расчет зарплаты сотрудника
                    id: calculateSalary
                    width: 100
                    height: 40
                    text: "Calculate"
                    Layout.rightMargin: 20
                    onClicked: {
                        MainCode.ReceiveDataFromQMLforCountSalary(beginDate.text, endDate.text, name.text )
                    }
                }
             }
        }   // Column
    }

    TemplatePage {                            // Страница со списком подчиненных
        id: subordinatesPage
        visible: false

        CommonButton {                    // Кнопка для возвращения в главное меню
            id: showMainMenuFromSubordinatesPage
            text: "<"
            width: allWindows.width /16
            height: allWindows.width / 16
            color: "#00000000"
            onClicked: {
                stackView.pop()
            }
        }
    }
}


