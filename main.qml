import QtQuick 2.12                 // Модули для qml файла
import QtQuick.Window 2.12  // Нужен для Window
import QtQuick.Controls 2.12  // Для объекта Button

ApplicationWindow {
    id: loginWindow
    width: 640
    height: 480
    visible: true
    title: qsTr("App")  // Название окна
    color: "#1B242F"
}
