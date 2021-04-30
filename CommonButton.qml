import QtQuick 2.0

Item {

    id: item
    property int size: 100
    property color color: "#213141"
    property alias text: text.text
    property alias fontSize: text.font.pointSize

    width: parent.width /5
    height: parent.height / 16

    signal clicked()

    Rectangle {
        id: rec
        radius: width /25
        anchors.fill: parent
        color: mouseArea.containsPress ? Qt.darker(item.color, 0.8 ): item.color

        Text {
            id: text
            text: qsTr(" ")
            color: "#aeb0b6"
            anchors.centerIn: parent
        }

        MouseArea {
            id: mouseArea
            anchors.fill: parent
            onClicked: {
                item.clicked()
            }
        }
    }
}
