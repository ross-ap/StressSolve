import QtQuick
import QtQuick.Controls 2.15
import QtQuick.Layouts 2.15

Window {
    width: 640
    height: 480
    visible: true
    color: "#494949"
    title: qsTr("Hello World")

    Label {
        id: label
        x: 124
        y: 15
        width: 393
        height: 80
        text: qsTr("StressSolve")
        horizontalAlignment: Text.AlignHCenter
        font.pointSize: 30
        styleColor: "#ffffff"
    }

    GroupBox {
        id: groupBox
        x: 18
        y: 70
        width: 605
        height: 361
        visible: true
        title: qsTr("")

        Text {
            id: _text
            x: 40
            y: 15
            width: 525
            height: 72
            color: "#ffffff"
            text: qsTr("Stress affects academic performance by increasing anxiety, depression, and physical symptoms like headaches and sleep issues, which reduce focus and productivity. External factors like poor living conditions, unmet needs, and safety concerns worsen the impact, while study loads, peer pressure, and strained relationships further hinder success.\n\n\n\n\n\n\n")
            font.pixelSize: 12
            horizontalAlignment: Text.AlignHCenter
            wrapMode: Text.WordWrap
        }

        ColumnLayout {
            id: columnLayout
            x: 264
            y: 131
            width: 155
            height: 100
            visible: true
            uniformCellSizes: true
            layoutDirection: Qt.LeftToRight
            spacing: 5

            Button {
                id: button
                text: qsTr("Continue")
            }

            Button {
                id: button1
                text: qsTr("Exit")
            }
        }
    }
}
