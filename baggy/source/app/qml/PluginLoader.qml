import QtQuick 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12
import QtQuick.Dialogs 1.3
import QtQuick.Window 2.12
import chip.Baggy 1.0 as Baggy

Window {
    id: win
    property bool confirm: false

    flags: Qt.Dialog /*| Qt.CustomizeWindowHint | Qt.WindowTitleHint*/
    modality: Qt.WindowModal
    title: qsTr("Loading plugins")
    color: palette.window

    property int margin: 5
    minimumWidth: contentItem.childrenRect.width + margin * 2
    maximumWidth: minimumWidth
    width: maximumWidth
    minimumHeight: contentItem.childrenRect.height + margin * 2
    maximumHeight: minimumHeight
    height: maximumHeight

    SystemPalette {
        id: palette
        colorGroup: SystemPalette.Active
    }

    GridLayout {
        id: grid
        columns: 3
        columnSpacing: 5
        anchors.centerIn: parent
        anchors.margins: 5

        Baggy.PluginLoader {
            id: loader
            readonly property int processed: loaded + failed
            onCompleted: {
                console.log(pluginList)
                if (!confirm)
                    win.close()
            }
        }

        Item {
            Layout.columnSpan: 3
            width: 480
        }

        Label {
            text: qsTr("Processing:")
        }
        Label {
            Layout.columnSpan: 2
            Layout.fillWidth: true
            text: loader.processingFile
        }

        Label {
            text: qsTr("Loaded:")
        }
        Label {
            Layout.columnSpan: 2
            Layout.fillWidth: true
            text: loader.loaded
        }

        Label {
            text: qsTr("Failed:")
        }
        Label {
            Layout.columnSpan: 2
            Layout.fillWidth: true
            text: loader.failed
        }

        Label {
            text: qsTr("Total:")
        }
        Label {
            Layout.columnSpan: 2
            Layout.fillWidth: true
            text: loader.processed + " / " + loader.total
        }

        ProgressBar {
            from: 0
            to: loader.total
            value: loader.processed
            enabled: loader.isWorking
            Layout.columnSpan: 2
            Layout.fillWidth: true
        }

        BusyIndicator {
            running: loader.isWorking
        }

        Button {
            text: qsTr("Done")
            visible: confirm
            enabled: !loader.isWorking
            onClicked: win.close()
            Layout.columnSpan: 3
            Layout.alignment: Qt.AlignRight
        }
    }

    Component.onCompleted: {
        loader.load()
    }
}
