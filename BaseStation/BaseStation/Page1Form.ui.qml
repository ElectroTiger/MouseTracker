import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0

Item {
    id: item1

    RowLayout {
        id: rowLayout2
        anchors.rightMargin: 0
        anchors.bottomMargin: 0
        anchors.leftMargin: 0
        anchors.topMargin: 0
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter


        ColumnLayout {
            id: columnLayout
            x: 100
            width: 200
            height: 200
            transformOrigin: Item.Center
            Layout.columnSpan: 2
            anchors.rightMargin: 0
            anchors.bottomMargin: 0
            anchors.leftMargin: 0
            anchors.topMargin: 0

            Text {
                id: textInstruction1
                text: qsTr("1) Choose Download Location")
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                font.pixelSize: 12
            }

            RowLayout {
                id: rowLayout1
                x: 0
                y: 50
                width: 100
                height: 100
                Layout.rowSpan: 2
                transformOrigin: Item.Center
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter

                Text {
                    id: text1
                    text: qsTr("Current Output Directory")
                    verticalAlignment: Text.AlignTop
                    horizontalAlignment: Text.AlignLeft
                    font.pixelSize: 12
                }

                TextInput {
                    id: textInput
                    width: 80
                    height: 20
                    text: qsTr("Text Input")
                    font.pixelSize: 12
                }

                Button {
                    id: button
                    text: qsTr("Button")
                }
            }

            Text {
                id: textInstruction2
                text: qsTr("Text")
                Layout.rowSpan: 3
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                font.pixelSize: 12
            }

            RowLayout {
                id: rowLayout
                width: 100
                height: 100
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
            }
        }
        ColumnLayout {
            id: columnLayout1
            width: 100
            height: 100

            Text {
                id: text2
                text: qsTr("Storage Location")
                font.pixelSize: 12
            }

            Frame {
                id: frame
                width: 200
                height: 200

                ItemDelegate {
                    id: itemDelegate
                    x: -582
                    y: -24
                    text: qsTr("Add a location...")
                    Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                    topPadding: 15
                }
            }

        }
    }

    Connections {
        target: frame
        onClicked: print("clicked")
    }
}
