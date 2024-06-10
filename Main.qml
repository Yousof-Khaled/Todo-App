import QtQuick
import QtQuick.Controls

// import Todo 1.0
import ToDo_app

Window {
    width: 810
    height: 600

    color: "darkblue"

    visible: true
    title: qsTr("Hello World")

    Item {
        id: root

        property bool isEditing: false

        property int vSeparation: 2
        property int hSeparation: 2

        anchors.fill: parent
        focus: true

        onIsEditingChanged: {
            if(isEditing === false) {
                root.forceActiveFocus()
            }
        }

        Keys.onPressed: (event) => { // todo: cleanup
            if (event.key === Qt.Key_Return) {
                isEditing = !isEditing
                console.log("enter pressed")
            }
            else if (event.key === Qt.Key_Escape) {
                isEditing = false
            }
            else if (event.key === Qt.Key_Up) {
                if ((event.modifiers & Qt.ShiftModifier) > 0) {
                    ToDoDriver.updateItemRow(-1);ToDoDriver.printData()
                }
                else
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
        }


        Component {
            id: itemComponent
            Rectangle {
                id: itemRoot

                width: 200
                height: 70
                y: rowNumber * (height + root.vSeparation)
                x: colNumber * (width + root.hSeparation)
                color: "black"


                NumberAnimation {
                    id: anim
                    duration: 400
                }
                NumberAnimation {
                    id: anim2
                    duration: 400
                }
                Behavior on x {
                    animation: anim
                }
                Behavior on y {
                    animation: anim2
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
                    color: "lightblue"
                    width: 3
                }
                state: {
                    if (rowNumber === ToDoDriver.selectedRow && colNumber === ToDoDriver.selectedColumn) {
                        return root.isEditing ? "edit" : "hover"
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
                            border.color: "darkgrey"
                        }
                    },
                    State {
                        name: "edit"
                        PropertyChanges {
                            target: itemRoot
                            border.color: "lightblue"
                        }
                    }
                ]
            }
        }

        Repeater {
            id: cardRepeater
            model: ToDoDriver.model//ToDoListModel {}
            delegate: itemComponent
        }
    }
}
