import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Window 2.12
import QtQuick.Layouts 1.12

Page {                                      // Страница расчета зарплаты

    property alias salary: salary.text
    property alias idText: workerId.text
    property alias beginDateText: beginDate.text
    property alias endDateText: endDate.text

    id: calculateSalaryPage
    visible: false
    background: Rectangle {     // Для цвета фона
        id: backgroundRect
        color: "#1B242F"
    }

    CommonButton {                // Кнопка для возвращения в главное меню
        id: showMainMenu
        width: allWindows.width /16
        height: allWindows.width / 16
        text: "<"
        color: "#00000000"
        onClicked: {
            stackView.pop()
            salary.text = ""
        }
    }

    ColumnLayout {                 // Слой с местом для ввода имени, дат и выводом сообщения об ошибке или рассчитанной з/п
        id: generalLinker

        width: allWindows.width
        anchors.bottom: parent.bottom
        anchors.top: showMainMenu.bottom
        anchors.topMargin: allWindows.height / 6
        anchors.horizontalCenter: parent.horizontalCenter

        ColumnLayout {
            Layout.fillWidth: true
            Layout.leftMargin: 200
            Layout.alignment: Qt.AlignLeft

            Label {
                font.bold: true
                color: "#aeb0b6"
                font.pointSize: 12
                text: "Введите id сотрудника"
            }

            TextField {
                id: workerId
                placeholderText: "id"
                Keys.onEscapePressed: {
                    salary.text = ""
                    stackView.pop()
                    onPressedItem.focus = true
                }
                Keys.onReturnPressed: MainCode.receiveDataFromQMLforCountSalary(beginDate.text, endDate.text, workerId.text )
            }

            Label {
                font.bold: true
                color: "#aeb0b6"
                font.pointSize: 12
                text: "Введите даты"
            }

            TextField {
                id: beginDate
                placeholderText: "Начальная: дд.мм.гггг"
                Keys.onEscapePressed: {
                    salary.text = ""
                    stackView.pop()
                    onPressedItem.focus = true 
                }
                Keys.onReturnPressed: MainCode.receiveDataFromQMLforCountSalary(beginDate.text, endDate.text, workerId.text )
            }

            TextField {
                id: endDate
                placeholderText: "Конечная: дд.мм.гггг"
                Keys.onEscapePressed: {
                    salary.text = ""
                    stackView.pop()
                    onPressedItem.focus = true
                }
                Keys.onReturnPressed: MainCode.receiveDataFromQMLforCountSalary(beginDate.text, endDate.text, workerId.text )
            }

            Label {
                id: salary
                color: "#aeb0b6"
                font.pointSize: 12
            }
        }

        RowLayout{                  // Кнопки для расчета заработной платы за определенный период
            height: 50
            id: rowLayout
            Layout.bottomMargin: 20
            Layout.alignment: Qt.AlignBottom

            Item {                      // Spacer
                Layout.fillWidth: true
            }

            CommonButton {          // Очистить поля
                id: clearAllFields
                width: 150
                height: 40
                text: "Очистить поля"
                onClicked: {
                    endDate.text = ""
                    workerId.text = ""
                    beginDate.text = ""
                }
            }

            CommonButton {          // Расчет зарплаты всех сотрудников
                id: calculateAllSalary
                width: 150
                height: 40
                text: "Рассчитать для всех"
                onClicked: MainCode.receiveDataFromQMLforCountSalaryForAll(beginDate.text, endDate.text)
            }

            CommonButton {          // Расчет зарплаты сотрудника
                id: calculateSalary
                width: 150
                height: 40
                text: "Рассчитать"
                Layout.rightMargin: 20
                onClicked: MainCode.receiveDataFromQMLforCountSalary(beginDate.text, endDate.text, workerId.text )
            }
         }
    }
}
