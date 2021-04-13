import QtQuick 2.12                 // Модули для qml файла
import QtQuick.Window 2.12  // Нужен для Window
import QtQuick.Controls 2.12  // Для объекта Button
import QtQuick.Layouts 1.12

ApplicationWindow {
    id: loginWindow
    maximumWidth: 600
    minimumWidth: 600
    maximumHeight: 500
    minimumHeight: 500
    // width: 640
    // height: 480
    visible: true
    title: qsTr("App")  // Название окна
    color: "#1B242F"

    ColumnLayout {                                                                      // Слоай с местом для двух слоев 1. Для ввода логина, пароля 2. Для кнопок
        id: generalLinker                                                                 // Главный компоновщик
        anchors.fill: parent

        ColumnLayout {
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignLeft
            Layout.leftMargin: 200

            Label {
                text: "Введите логин и пароль"
                color: "#aeb0b6"
                font.pointSize: 12  // fontsize
                font.bold: true
            }
            Item {                  // Spring for buttons
                height: 10
            }
            TextField {
                id: beginDate
                placeholderText: "Логин"
                //Layout.fillWidth: true
            }
            TextField {
                id: endDate
                placeholderText: "Пароль"
                //Layout.fillWidth: true
            }
            Label {
                id: salary
                text: "Неправильно введен логин или пароль"
                color: "#aeb0b6"
                font.pointSize: 8  // fontsize
            }

        }

        RowLayout{                  // Кнопки для расчета заработной платы за определенный период
            id: rowLayout
            Layout.alignment: Qt.AlignBottom        // Выравнивание по низу
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

            CommonButton {          // Moveback button
                id: calculateSalary
                width: 100
                height: 40
                text: "Войти"
                Layout.rightMargin: 20
                onClicked: {
                    MainCode.receiveFromQml(beginDate.text, endDate.text, name.text )
                }
            }
         }

    }
}
