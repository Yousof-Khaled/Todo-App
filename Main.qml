import QtQuick
import QtQuick.Controls

Window {
    width: 800
    height: 600

    color: "darkblue"

    visible: true
    title: qsTr("Hello World")

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
            else {
                root.activeFocus = false;
            }
        }
        Keys.onReturnPressed: {
            // event.accepted = true;
            isEditing = !isEditing
            console.log("enter pressed")
        }
        Keys.onEscapePressed: {
            isEditing = false
        }
        Keys.onUpPressed: {
            selectedRow -= 1
            selectedRow = Math.max(selectedRow, 0)
        }
        Keys.onDownPressed: {
            selectedRow += 1
            selectedRow = Math.min (selectedRow, myModel.count - 1)
        }


        component TodoItem: Rectangle {
            id: itemRoot

            width: 200
            height: 70

            required property int index
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
                    console.log(index)
                    return card_text
                }

                property int maxLength: 15 // Set the maximum number of characters

                onTextChanged: {
                    if (text.length > maxLength) {
                        text = text.slice(0, maxLength);
                        cursorPosition = maxLength; // Optional: Move cursor to the end
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
                if (index === root.selectedRow) {
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
                else {
                    // root.forceActiveFocus()
                    root.isEditing
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


        // TodoItem {
        //     index: 1
        //     card_text: "geedsmkldsm"
        // }

        ListModel {
            id: myModel
            ListElement {
                index: 0
                card_text: "hello1"
            }
            ListElement {
                index: 1
                card_text: "hello2"
            }
        }

        // ListView {
        //     id: inProgress

        //     width: 500
        //     height: 500
        //     delegate: todoItem

        //     spacing: 2
        //     model: myModel
        // }
        // property var cards: []
        // Component.onCompleted: {
        //     for (var i = 0 ; i < myModel.count ; ++i) {

        //     }
        // }
        Repeater {
            model: myModel
            delegate: itemRoot
        }


    }



}
