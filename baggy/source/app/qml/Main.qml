import QtQuick 2.2
import QtQuick.Layouts 1.0
import QtQuick.Controls 1.4
import QtQuick.Dialogs 1.2
import QtQuick.Window 2.0
import "qrc:/qml/" as Baggy

ApplicationWindow {
    id: main
    title: qsTr("qBaggy")
    minimumWidth: 640
    minimumHeight: 480
    visible: true

    Component.onCompleted: {
        pluginLoader.visible = true
    }

    SystemPalette {
        id: palette
        colorGroup: SystemPalette.Active
    }

    menuBar: MenuBar {
        Menu {
            title: qsTr("File")
            MenuItem {
                text: qsTr("Open bag...")
                onTriggered: bagOpenDialog.open()
            }
            MenuSeparator {}
            MenuItem {
                text: qsTr("Quit")
                onTriggered: Qt.quit()
            }
        }

        Menu {
            title: qsTr("&Help")
            MenuItem {
                text: qsTr("&About qBagger...")
                onTriggered: aboutWindow.visible = true
            }
        }
        Menu {
            title: qsTr("&Tools")
            MenuItem {
                text: qsTr("Plugins...")
            }
        }
    }

    // @disable-check M300
    Baggy.About {
        id: aboutWindow
    }

    // @disable-check M300
    Baggy.PluginLoader {
        id: pluginLoader
        confirm: true
    }

    FileDialog {
        id: bagOpenDialog
        title: qsTr("Select bag-file")
        modality: Qt.WindowModal
        nameFilters: qsTr("ROS bag-file (*.bag)")
        selectFolder: false
        selectExisting: true
        selectMultiple: false
        onAccepted: baggy.bagData.open(this.fileUrl)
    }

    SystemPalette {
        id: sysPal
        colorGroup: SystemPalette.Active
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 5
        spacing: 5

        Label {
            Layout.fillWidth: true
            text: qsTr("File:") + " " + baggy.bagData.fileName
        }

        Label {
            Layout.fillWidth: true
            text: qsTr("Connections:")
        }

        Rectangle {
            border.width: 1
            border.color: sysPal.dark
            Layout.fillHeight: true
            Layout.fillWidth: true
            color: sysPal.light

            ScrollView {
                id: connectionsView
                anchors.fill: parent
                horizontalScrollBarPolicy: Qt.ScrollBarAlwaysOff
            }
        }
    }
}
