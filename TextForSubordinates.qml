import QtQuick 2.0

Text {

    property alias text: subordinatesText.text

    id: subordinatesText
    anchors.left: mainRec.left
    anchors.leftMargin: myMargin
    anchors.topMargin: 1
    font.pointSize: 14
    color: "#aeb0b6"
}
