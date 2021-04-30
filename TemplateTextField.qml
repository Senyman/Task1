import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Window 2.12
import QtQuick.Layouts 1.12

TextField {

    width: 250
    Layout.maximumHeight: 35
    Layout.minimumWidth: 250
    Layout.maximumWidth: 250

    Keys.onEscapePressed: {

        nameForDB.text = ""
        nameErrorMes.text = ""
        beginDateForDB.text = ""
        beginDateErrorMes.text = ""
        baseRateForDB.text = ""
        baseRateErrorMes.text = ""
        typeForDB.displayText = "Тип сотрудника"
        typeErrorMes.text = ""
        loginForDB.text = ""
        loginErrorMes.text = ""
        firstPasswordForDB.text = ""
        firstPasswordErrorMes.text = ""
        secondPasswordForDB.text = ""
        secondPasswordErrorMes.text = ""
        chiefForDB.displayText = "Начальник"
        chiefIdErrorMes.text = ""
        superuserForDB.displayText = "Суперпользователь"
        superuserErrorMes.text = ""
        commonErrorMes.text = ""


        onPressedItem.focus = true
        stackView.pop()
    }
    Keys.onReturnPressed: {
        chiefForDB.displayText = "Начальник"
        nameErrorMes.text = ""
        beginDateErrorMes.text = ""
        baseRateErrorMes.text = ""
        typeErrorMes.text = ""
        loginErrorMes.text = ""
        firstPasswordErrorMes.text = ""
        secondPasswordErrorMes.text = ""
        chiefIdErrorMes.text = ""
        superuserErrorMes.text = ""

        MainCode.addWorker(nameForDB.text, beginDateForDB.text, baseRateForDB.text, typeForDB.currentText,
                           loginForDB.text, firstPasswordForDB.text, secondPasswordForDB.text, chiefForDB.currentText, superuserForDB.currentText)

        chiefIdListModel.clear()
        MainCode.fillChiefListModel()
    }
}
