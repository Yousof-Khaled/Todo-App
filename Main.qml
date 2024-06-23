import QtQuick
import QtQuick.Controls

import ToDo_app

Window {
    width: 810
    height: 600

    color: "#383838"

    visible: true
    title: qsTr("ToDo App")

    Item {
        id: root

        property bool isEditing: false
        property bool isDeleting: false

        property int vSeparation: 2
        property int hSeparation: 2

        anchors.fill: parent
        focus: true

        onIsEditingChanged: {
            if(isEditing === false) {
                root.forceActiveFocus()
            }
        }
        onIsDeletingChanged: {
            if(isDeleting === false) {
                root.forceActiveFocus()
            }
        }

        Keys.onPressed: (event) => { // todo: cleanup -> remove key events handling to the c++ side
            if (isDeleting) {
                return
            }

            if (event.key === Qt.Key_Return) {
                isEditing = !isEditing
                console.log("enter pressed")
                console.log("row : " + ToDoDriver.selectedRow + ", column : " + ToDoDriver.selectedColumn)
            }
            else if (event.key === Qt.Key_N && (event.modifiers & Qt.ControlModifier) > 0) {
                ToDoDriver.addNewCard()
                isEditing = true;
            }
            else if (event.key === Qt.Key_Escape) {
                isEditing = false
            }
            else if (event.key === Qt.Key_Up) {
                if ((event.modifiers & Qt.ShiftModifier) > 0) {
                    ToDoDriver.updateItemRow(-1);ToDoDriver.printData()
                }
                ToDoDriver.hoverPreviousRow()
            }
            else if (event.key === Qt.Key_Down) {
                if ((event.modifiers & Qt.ShiftModifier) > 0) {
                    ToDoDriver.updateItemRow(1);ToDoDriver.printData()
                }
                ToDoDriver.hoverNextRow()
            }
            else if (event.key === Qt.Key_Left) {
                if ((event.modifiers & Qt.ShiftModifier) > 0) {
                    ToDoDriver.updateItemColumn(-1);ToDoDriver.printData()
                }
                else
                    ToDoDriver.hoverPreviousColumn()
            }
            else if (event.key === Qt.Key_Right) {
                if ((event.modifiers & Qt.ShiftModifier) > 0) {
                    ToDoDriver.updateItemColumn(1);ToDoDriver.printData()
                }
                else
                    ToDoDriver.hoverNextColumn()
            }
            else if (event.key === Qt.Key_Delete) {
                root.isDeleting = true
                console.log("delete is pressed")
            }
        }


        Component {
            id: itemComponent
            Rectangle {
                id: itemRoot

                width: 200
                height: 70
                y: {
                    var ret = (rowNumber + 1) * (height + root.vSeparation)
                    if (rowNumber > -1)
                        ret -= 20
                    return ret
                }

                x: colNumber * (width + root.hSeparation)
                color: "black"

                Behavior on x {
                    animation: NumberAnimation {
                        duration: 200
                    }
                }
                Behavior on y {
                    animation: NumberAnimation {
                        duration: 200
                    }
                }

                SequentialAnimation {
                    id: savingItem
                    ColorAnimation {
                        target: itemRoot
                        property: "color"
                        from: "black"
                        to: "darkblue"
                        duration: 400
                    }
                    ColorAnimation {
                        target: itemRoot
                        property: "color"
                        from: "darkblue"
                        to: "black"
                        duration: 400
                    }
                }
                ParallelAnimation {
                id: deletingItem
                    ColorAnimation {
                        target: itemRoot
                        property: "color"
                        from: "black"
                        to: "red"
                        duration: 400
                    }
                    NumberAnimation {
                        target: itemRoot
                        property: "opacity"
                        from: 1
                        to: 0
                        duration: 600
                    }
                    running: {
                        return root.isDeleting == true && (rowNumber === ToDoDriver.selectedRow && colNumber === ToDoDriver.selectedColumn)
                    }

                    onRunningChanged: {
                        if (running == true) {
                            // root.activeFocus = false
                            // itemRoot.forceActiveFocus()
                        }
                        else if (running == false) {
                            root.isDeleting = false
                            ToDoDriver.deleteSelectedCard()
                        }
                    }
                }

                required property var index
                required property int cardID
                required property int rowNumber
                required property int colNumber
                required property string card_text

                TextEdit {
                    id: itemText
                    font {
                        bold: true
                        pixelSize: 18
                    }

                    anchors.centerIn: parent
                    color: "white"
                    wrapMode: TextEdit.Wrap
                    text: {
                        return card_text
                    }

                    property int maxLength: 15 // Set the maximum number of characters

                    onTextChanged: {
                        if (text.length > maxLength) {
                            text = text.slice(0, maxLength);
                            cursorPosition = maxLength;
                        }
                    }
                    Keys.onReturnPressed: {
                        root.isEditing = false

                        ToDoDriver.updateItem(index, itemRoot.cardID, itemRoot.rowNumber, itemRoot.colNumber, itemText.text)
                        ToDoDriver.printData()

                        savingItem.start()
                    }
                    Keys.onUpPressed: {
                        cursorPosition = 0
                    }
                    Keys.onDownPressed: {
                        cursorPosition = text.length
                    }
                }
                border {
                    // color: "lightblue"
                    width: 3
                }
                state: {
                    if (rowNumber === -1) {
                        return "header"
                    }

                    if (rowNumber === ToDoDriver.selectedRow && colNumber === ToDoDriver.selectedColumn) {
                        if (root.isEditing)
                            return "edit";
                        else if (root.isDeleting)
                            return "delete"
                        else
                            return "hover";
                    }
                    else {
                         return "sleep"
                    }
                }
                onStateChanged: {
                    if(state === "edit") {
                        itemText.forceActiveFocus()
                        itemText.cursorPosition = itemText.text.length
                    }
                }
                states: [
                    State {
                        name: "sleep"
                        PropertyChanges {
                            target: itemRoot
                            border.color: "black"
                        }
                    },
                    State {
                        name: "hover"
                        PropertyChanges {
                            target: itemRoot
                            border.color: "blue"
                        }
                    },
                    State {
                        name: "edit"
                        PropertyChanges {
                            target: itemRoot
                            border.color: "lightblue"
                        }
                    },
                    State {
                        name: "header"
                        PropertyChanges {
                            target: itemRoot
                            border.color: "orange"
                            color: "orange"
                            height: 50
                        }
                    }

                ]
            }
        }

        Repeater {
            id: header

            model : ListModel {
                ListElement{
                    cardID: -1
                    rowNumber: -1
                    colNumber: 0
                    card_text: "Ready"
                }
                ListElement{cardID: -2; rowNumber: -1; colNumber: 1; card_text: "In Progress"}
                ListElement{cardID: -3; rowNumber: -1; colNumber: 2; card_text: "Testing"}
                ListElement{cardID: -4; rowNumber: -1; colNumber: 3; card_text: "Done"}
            }

            delegate: itemComponent
        }

        Repeater {
            id: cardRepeater
            model: ToDoDriver.model
            delegate: itemComponent
        }
    }
}
