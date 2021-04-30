import QtQuick 2.0
import QtQuick.Window 2.12
import QtQuick.Controls 2.12

Label {
    id: templateLabel
    property alias text: templateLabel.text
    property alias size: templateLabel.font.pointSize
    color: "#aeb0b6"
    font.pointSize: 8
}
