import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Window 2.12

Window {
    id: mainMenuWindow
    maximumWidth: 600
    minimumWidth: 600
    maximumHeight: 500
    minimumHeight: 500
    title: qsTr("Main Menu")
    color: "#1B242F"

    signal goToLoginWindow

    CommonButton {          // Выход в окно регистрации
        id: goToLoginWindowButton
        width: 200
        height: 25
        text: "Выйти"
        anchors.centerIn: parent
        onClicked: {
            mainMenuWindow.goToLoginWindow()
        }
   }

}
