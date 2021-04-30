import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Window 2.12
import QtQuick.Layouts 1.12

TemplatePage {                            // Страница со списком подчиненных

    id: subordinatesPage
    visible: false

    Connections {
      target: MainCode
          onSendSubordinatesInfoToQML: {
              listModel.append({idOfSubordinate: "Id: " + idSub, nameOfSubordinate: "Имя: " + nameSub, typeOfSubordinate: "Тип рабочего: " + typeOfWorkerSub,
                                   firstDayDateOfSubordinate: "Дата первого рабочего дня: " + firstDayDateSub, baseRateOfSubordinate: "Дневная ставка: " + baseRateSub + " руб.",
                                   chiefIdOfSubordinate: "Id начальника: " + chiefIdSub, levelOfSubordinate: "Уровень подчиненного: " + levelOfSubStr});
          }
    }

    Connections {
        target: allWindows
        onClearListModelsFromAllWindows: listModel.clear()
    }
    ColumnLayout {
        id: subordinatesColumn
        anchors.fill: parent
        spacing: 0

        Rectangle {
            z: +1
            height: 200
            color: "#1B242F"
            Layout.fillWidth: subordinatesColumn

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
                height: 50
                width: parent.width
            }

            Label {
                z: 1
                font.bold: true
                font.pointSize: 16
                color: "#aeb0b6"
                anchors.centerIn: parent
                text: "Список подчиненных"
            }

            Item {
                height: 100
                Layout.fillWidth: subordinatesColumn
            }
        }

        ListView {      // Список всех подчиненных всех уровней

            id: listSubordinates
            Layout.fillHeight: true
            Layout.alignment: Qt.AlignHCenter
            ScrollBar.vertical: ScrollBar {}
            width: 500
            spacing: 6
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
                border.color: "black"
                color: "#213141"

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
            z: +1
            height: 100
            color: "#1B242F"
            Layout.fillWidth: subordinatesColumn
        }
    }

    ListModel {
        id: listModel
    }
}
