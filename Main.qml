import QtQuick
import QtQuick.Controls

Window {
    width: 800
    height: 600

    color: "darkblue"

    visible: true
    title: qsTr("Hello World")

    property int vSeparation: 2

    Item {
        id: root

        property int selectedRow: 1
        property int selectedColumn: 0
        property bool isEditing: false

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
                selectedRow -= 1
                selectedRow = Math.max(selectedRow, 0)
            }
            else if (event.key === Qt.Key_Down) {
                selectedRow += 1
                selectedRow = Math.min (selectedRow, myModel.count - 1)
            }
        }


        Component {
            id: itemComponent
            Rectangle {
                id: itemRoot

                width: 200
                height: 70
                y: row * (height + vSeparation)

                required property int row
                required property int col
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
                        console.log(row)
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
                    if (row === root.selectedRow) {
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

        ListModel {
            id: myModel
            ListElement {
                row: 0
                col: 0
                card_text: "hello1"
            }
            ListElement {
                row: 1
                col: 0
                card_text: "hello2"
            }
            ListElement {
                row: 2
                col: 0
                card_text: "hello3"
            }
        }

        Repeater {
            model: myModel
            delegate: itemComponent
        }


    }



}
