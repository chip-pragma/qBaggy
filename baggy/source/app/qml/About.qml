import QtQuick 2.0
import QtQuick.Dialogs 1.2

MessageDialog {
    id: about
    title: "About"
    text: '<b>qBaggy</b><br/><br/>bag-file extractor-convertor<br/><br/>By <span style="color: maroon">Ivan -=chip_pragma=- Cheltsov</span><br/>2K19'
    icon: "Information"
    modality: Qt.WindowModal
}
