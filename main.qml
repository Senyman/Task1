import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

ApplicationWindow {

    id: loginWindow
    maximumWidth: 600
    minimumWidth: 600
    maximumHeight: 500
    minimumHeight: 500
    visible: true
    title: qsTr("Авторизация")
    color: "#1B242F"

    Connections {
        target: MainCode
        onSendErrorMessage: {
            errorMessage.text =  errorMesage
        }
        onOpenMainMenu: {
            allWindows.show()
            loginWindow.hide()
        }
    }

    Item {      // Для входа через Enter и выхода через Esc
        id: onPressedItem
        focus: true
        anchors.fill: parent
        Keys.onEscapePressed: Qt.quit()
        Keys.onReturnPressed : {
            MainCode.logIn(login.text, password.text)
            login.text = ""
            password.text = ""
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

        Item { // Spacer
            height: 10
            focus: true
            Keys.onReturnPressed : {
                MainCode.logIn(login.text, password.text)
                login.text = ""
                password.text = ""
            }
        }

        TextField {
            id: login
            placeholderText: "Логин"
            Keys.onEscapePressed: Qt.quit()
            Keys.onReturnPressed : {
                MainCode.logIn(login.text, password.text)
                login.text = ""
                password.text = ""
            }
        }

        TextField {
            id: password
            echoMode: TextInput.Password
            placeholderText: "Пароль"
            Keys.onEscapePressed: Qt.quit()
            Keys.onReturnPressed : {
                MainCode.logIn(login.text, password.text)
                login.text = ""
                password.text = ""
            }
        }

        Label {
            id: errorMessage
            color: "#aeb0b6"
            font.pointSize: 8
        }

        Item { // Spacer
            height: 10
            focus: true
            Keys.onEscapePressed: Qt.quit()
        }

        CommonButton {
            id: enter
            width: 200
            height: 25
            text: "Войти"
            onClicked : {
                MainCode.logIn(login.text, password.text)
                login.text = ""
                password.text = ""
            }
       }
    }

    AllWindows {
        id: allWindows
        onGoToLoginWindow:
        {
            allWindows.close()
            loginWindow.show()
        }
    }
}
