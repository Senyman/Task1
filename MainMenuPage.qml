import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Window 2.12
import QtQuick.Layouts 1.12

Page {

    id: mainMenuPage
    property alias logintext: loginText.text
    property alias nametext: nameText.text
    property alias errorTextFromMainMenu: errorTextMainMenu.text

    signal clearListModel
    signal clearChiefIdListModel

    Connections {
        target: MainCode
        onOpenAddWorkerPage: {
            mainMenuPage.clearChiefIdListModel()
            stackView.push(addWorkerPage)
        }
        onDoesNotHavePermissionToOpenPage: {
            errorTextMainMenu.text = " Недостаточно прав"
        }
    }

    background: Rectangle {
        id: backgroundRect
        color: "#1B242F"

        ColumnLayout {
            id: mainMenuLinker
            anchors.centerIn: parent

            CommonButton {               // Открыть окно расчета з/п
                id: goToCountSalaryWindowButton
                width: 300
                height: 35
                text: "Рассчитать зарплату"
                fontSize: 10
                onClicked: {
                    errorTextMainMenu.text = " "
                    stackView.push(calculateSalaryPage)
                    onPressedItem.focus
                }
           }

            CommonButton {               // Показать подчиненных
                id: goToSubordinateWindowButton
                width: 300
                height: 35
                text: "Показать подчиненных"
                fontSize: 10
                onClicked: {
                    mainMenuPage.clearListModel()
                    onPressedItem.focus = true
                    stackView.push(subordinatesPage)
                    MainCode.findWorkersAndSubordinatesForSubPage()
                    errorTextMainMenu.text = " "
                }
           }

            CommonButton {             // Добавить рабочего
                id: goToAddWorkersWindowButton
                width: 300
                height: 35
                text: "Добавить сотрудника"
                fontSize: 10
                onClicked: {
                    MainCode.findWorkersAndSubordinates()
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
                    errorTextMainMenu.text = " "
                }
           }

            Item {
                height: 10
            }

            TemplateLabelForAddWorkerPage {
                id: errorTextMainMenu
                size: 12
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
                    color: "#aeb0b6"
                }

                Text {
                    id: nameText
                    font.pointSize: 12
                    color: "#aeb0b6"
                }
            }
        }
    }
}
