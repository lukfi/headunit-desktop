import QtQuick 2.0
import QtQuick.Layouts 1.0
import com.navit.graphics_qt5 1.0
import com.navit.gui 1.0

Item {
    id: poiItem
    visible: true
    property var small_font_size : 16


    Text {
        x: 8
        y: 8
        color: "#ffffff"
        text: NavitGUI.activePoi.name
        font.pixelSize: 32
    }

    Text {
        x: 80
        y: 64
        color: "#ffffff"
        text: qsTr("Type")
        font.pixelSize: small_font_size
    }

    Text {
        x: 80
        y: 96
        color: "#ffffff"
        text: qsTr("Distance")
        font.pixelSize: small_font_size
    }

    Text {
        x: 160
        y: 64
        color: "#ffffff"
        text: NavitGUI.activePoi.type
        font.pixelSize: small_font_size
    }

    Text {
        x: 160
        y: 96
        color: "#ffffff"
        text: NavitGUI.activePoi.distance
        font.pixelSize: small_font_size
    }

    Rectangle {
        id: rectangle
        x: 8
        y: 64
        height: 64
        width: height
        color: "#ffffff"
        radius: 8
        border.width: 1
        Image {
            height: parent.width
            width: parent.height
            source : NavitGUI.get_icon_path() + NavitGUI.activePoi.icon
            sourceSize.width: parent.width
            sourceSize.height: parent.height
        }
    }

    ColumnLayout {
        id: columnLayout
        width: parent.width/2
        height: parent.height
        anchors.right: parent.right
        anchors.rightMargin: 0
        QNavitQuick {
            id: navit1
            width: 300
            height: 240

            Component.onDestruction: {
                console.log("Destroying a navit widget. Blocking draw operations")
                NavitGUI.block_draw()
            }
        }
    }

    MainButton {
        id: mainButton3
        x: 8
        y: parent.height-78
        width: parent.width/2 - 16
        height: 64
        radius: 1
        text: "Set as destination"
        icon: "icons/appbar.location.checkin.svg"
        onClicked: {
            NavitGUI.setActivePoiAsDestination()
        }
    }

}
