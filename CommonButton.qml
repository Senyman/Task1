import QtQuick 2.0

Item {
    id: item
    property int size: 100
    property color color: "#213141"
    property alias text: text.text        // Текст кнопки, связан с объектом Text и ключевым словом text: в main файле
    property alias fontSize: text.font.pointSize

    width: parent.width /5
    height: parent.height / 16

    signal clicked()                            // Для связи с onClicked:  в main файле

    Rectangle {
        id: rec
        //opacity: 1
        anchors.fill: parent
        radius: width /25
        color: mouseArea.containsPress ? Qt.darker(item.color, 0.8 ): item.color // Для затемнения при нажатии
        Text {
            id: text
            anchors.centerIn: parent
            text: qsTr(" ")
            color: "#aeb0b6"
        }
        MouseArea {
            id: mouseArea
            anchors.fill: parent
            onClicked: {
                item.clicked()              // Для связи с signal clicked
            }
        }
    }
}
