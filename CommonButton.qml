import QtQuick 2.0

Item {
    id: showSalaryItem
    property int size: 100
    property color color: "#213141"
    property alias text: showSalaryCaptionButton.text   // text = showSalaryCaptionButton.text

    width: parent.width /5
    height: parent.height / 16

    signal clicked()
    Rectangle {
        id: button
        //opacity: 1
        anchors.fill: parent
        radius: width /25
        color: showSalaryMouseArea.containsPress ? Qt.darker(showSalaryItem.color, 0.8 ): showSalaryItem.color // Затемнение при нажатии и цвет кнопки по умолчанию
        Text {
            id: showSalaryCaptionButton
            anchors.centerIn: parent
            text: qsTr("Show Salary")
            color: "#aeb0b6"
        }
        MouseArea {
            id: showSalaryMouseArea
            anchors.fill: parent
            onClicked: {
                showSalaryItem.clicked()
            }
        }
    }
}
