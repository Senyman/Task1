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

    StackView
    {
        id: stackView
        anchors.fill: parent
        initialItem: mainMenuPage     // Запускаем первую страницу
    }

    TemplatePage {
        id: mainMenuPage
        ColumnLayout {
            id: mainMenuLinker
            anchors.centerIn: parent

            CommonButton {          // Открыть окно расчета з/п
                id: goToCountSalaryWindowButton
                width: 300
                height: 35
                text: "Расчитать зарплату"
                fontSize: 10
                onClicked: {
                    stackView.push(calculateSalaryPage);
                }
           }

            CommonButton {          // Показать подчиненных
                id: goToSubordinateWindowButton
                width: 300
                height: 35
                text: "Показать подчиненных"
                fontSize: 10
                onClicked: {

                }
           }

            CommonButton {          // Показать подчиненных
                id: goToAddWorkersWindowButton
                width: 300
                height: 35
                text: "Добавить рабочего"
                fontSize: 10
                onClicked: {

                }
           }

            CommonButton {          // Выход в окно регистрации
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

    }

    TemplatePage {
        id: calculateSalaryPage
        visible: false

        CommonButton {          // В главное меню
            id: exitButton
            width: 200
            height: 25
            text: "В главное меню"
            anchors.centerIn: parent
            onClicked: {
                stackView.pop()
            }
        }
    }

}


