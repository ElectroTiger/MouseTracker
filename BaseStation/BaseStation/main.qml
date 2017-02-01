import QtQuick 2.7
import QtQuick.Controls 2.1
import QtQuick.Layouts 1.1
import QtQuick.Dialogs 1.2


ApplicationWindow {
    visible: true
    width: 540
    height: 960


    header: Label {
        padding:10
        text: qsTr("Mousetracker Base Station")
        font.pixelSize: 20
        horizontalAlignment:  Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
    }

    FileDialog {
        id: fileDialog
        visible: false
        modality: Qt.WindowModal
        title: qsTr("Choose a folder")
        selectExisting: true
        selectMultiple: false
        selectFolder: true
        sidebarVisible: true
        onAccepted: {
            console.log("Accepted: " + fileUrls)
            textEditDownloadLocation.text=fileUrl
        }
        onRejected: { console.log("Rejected") }
    }




    Frame {
        anchors.fill: parent
        ColumnLayout {
            anchors.fill: parent
            RowLayout {
                spacing: 2
                Layout.fillWidth: true
                Text {
                    id: textDownloadLocation
                    text: qsTr("Download Location:")
                    font.pixelSize: 12
                }
                Text {
                    id: textEditDownloadLocation
                    Layout.fillWidth: true
                    font.pixelSize: 12
                    text: qsTr("Not set...")
                    // placeholderText: qsTr("Select a location...")
                }
                Button {
                    id: buttonDownloadLocationSelect
                    text: qsTr("Open")
                    font.pixelSize: 12
                    onClicked: {
                        console.log("buttonDownloadLocationSet clicked")
                        fileDialog.open()
                    }
                }
            }
//            Text {
//                id: textValidLocation
//                text: qsTr("Set a location...")
//            }
            Text {
                text: qsTr("Devices")
                font.pixelSize: 12
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }

            ListView {
                id: listViewDevices
                model: ["Placeholder 1", "Placeholder 2", "Placeholder 3"]
                delegate: ItemDelegate {
                    text: modelData
                    width:  listViewDevices.width - listViewDevices.leftMargin - listViewDevices.rightMargin
                    height: 12
                    leftPadding: 32

                }
            }

        }
    }
}
