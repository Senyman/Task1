import QtQuick 2.12                 // Модули для qml файла
import QtQuick.Window 2.12  // Нужен для ApplicationWindow
import QtQuick.Controls 2.12  // Для объекта Button
import QtQuick.Layouts 1.12

ApplicationWindow {
    id: loginWindow
    maximumWidth: 600
    minimumWidth: 600
    maximumHeight: 500
    minimumHeight: 500
    visible: true
    title: qsTr("Log In")
    color: "#1B242F"

    Connections {                   // Для связи сигнала из C++ с текстовым полем errorMessage в qml
        target: MainCode
        onSendErrorMessage: {
            errorMessage.text =  errorMesage
        }
    }

    ColumnLayout {

        id: verticalLayout
        anchors.centerIn: parent

        Label {
            text: "Введите логин и пароль"
            color: "#aeb0b6"
            font.pointSize: 12
            font.bold: true
        }
        Item {                  // Spacer
            height: 10
        }
        TextField {
            id: login
            placeholderText: "Логин"
            //Layout.fillWidth: true
        }
        TextField {
            id: password
            placeholderText: "Пароль"
            //Layout.fillWidth: true
        }
        Label {
            id: errorMessage
            // text: "Неправильно введен логин или пароль"
            color: "#aeb0b6"
            font.pointSize: 8
        }
        Item {                  // Spacer
            height: 10
        }
        CommonButton {          // Moveback button
            id: enter
            width: 200
            height: 25
            text: "Войти"
            onClicked: {
                MainCode.logIn(login.text, password.text)
            }
       }
    }
}
