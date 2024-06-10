import QtQuick
import QtQuick.Controls

import Todo 1.0

Window {
    width: 800
    height: 600

    color: "darkblue"

    visible: true
    title: qsTr("Hello World")

    Item {
        id: root

        property int selectedRow: 0
        property int selectedColumn: 0
        property bool isEditing: false

        property int vSeparation: 2
        property int hSeparation: 2

        anchors.fill: parent
        focus: true

        onSelectedRowChanged: {
            isEditing = false
        }
        onIsEditingChanged: {
            if(isEditing === false) {
                root.forceActiveFocus()
            }
        }

        Keys.onPressed: (event) => {
            if (event.key === Qt.Key_Return) {
                isEditing = !isEditing
                console.log("enter pressed")
            }
            else if (event.key === Qt.Key_Escape) {
                isEditing = false
            }
            else if (event.key === Qt.Key_Up) {
                if ((event.modifiers & Qt.ShiftModifier) > 0) {
                    cardRepeater.updateItemPosition(selectedRow, selectedColumn, -1, 0);
                }
                selectedRow -= 1
                // selectedRow = Math.max(selectedRow, 0)
            }
            else if (event.key === Qt.Key_Down) {
                if ((event.modifiers & Qt.ShiftModifier) > 0) {
                    cardRepeater.updateItemPosition(selectedRow, selectedColumn, 1, 0);
                }
                selectedRow += 1
                // selectedRow = Math.min (selectedRow, cardRepeater.model.rowCount() - 1)
            }
            else if (event.key === Qt.Key_Left) {
                if ((event.modifiers & Qt.ShiftModifier) > 0) {
                    cardRepeater.updateItemPosition(selectedRow, selectedColumn, 0, -1);
                }
                selectedColumn -= 1
                // selectedCol = Math.max(selectedRow, 0)
            }
            else if (event.key === Qt.Key_Right) {
                if ((event.modifiers & Qt.ShiftModifier) > 0) {
                    cardRepeater.updateItemPosition(selectedRow, selectedColumn, 0, 1);
                }
                selectedColumn += 1
                // selectedCol = Math.min (selectedRow, cardRepeater.model.rowCount() - 1)
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

                required property var index
                required property int cardID
                required property int rowNumber
                required property int colNumber
                required property string card_text

                color: "black"
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
                        // console.log(rowNumber)
                        // console.log(colNumber)
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
                        // colNumber = 1
                        // rowNumber = 0
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
                    if (rowNumber === root.selectedRow && colNumber == root.selectedColumn) {
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
                transitions: Transition {
                    from: "edit"; to: "hover";// reversible: true
                    ScriptAction {
                        script: {
                            cardRepeater.updateItem(index, itemRoot.cardID, itemRoot.rowNumber, itemRoot.colNumber, itemText.text)
                            cardRepeater.printData()
                        }
                    }
                }
            }
        }

        Repeater {
            id: cardRepeater
            model: ToDoListModel {}
            delegate: itemComponent

            function printData() {
                model.printData()
            }

            function updateItem (index, cardID, newRow, newColumn, newText) {
                model.updateItem(index, cardID, newRow, newColumn, newText)
            }
            function updateItemPosition(row, col, rowChange, colChange) {
                model.updateItemPosition(row, col, rowChange, colChange)
                printData()
            }
        }


    }



}
