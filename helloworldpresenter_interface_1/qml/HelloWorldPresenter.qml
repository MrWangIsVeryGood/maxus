import QtQuick 2.9
import QtQuick.Controls 2.2
import com.zuragon.ViCANdo 1.0

Rectangle {
    id: qml_presenter
    width: 600
    height: 200
    color: "#F2F3F4"

    Label{
        id: label_text
        anchors.top: parent.top
        anchors.topMargin: 10
        anchors.horizontalCenter: parent.horizontalCenter
        text: "Hello world QML Presenter"
        font.pointSize: 30
    }

    Grid{
        id: grid
        anchors.top: label_text.bottom
        anchors.topMargin: 20
        anchors.horizontalCenter: parent.horizontalCenter
        rows: 2
        columns: 2
        spacing: 20
        columnSpacing: 100

        Label{
            id: read_size_label
            text: "Read Size:"
        }

        Text{
            id: size_read_text
            text: cplusplus.size   // Read access the property size directly
        }

        Label{
            id: write_size_label
            text: "Write Size:"
        }

        TextField{
            id: size_write_text
            placeholderText: qsTr("Enter number")
            onAccepted: {
                cplusplus.size = parseInt(text)   // Set the property size. This will call the setSize() function call

                cplusplus.updateData("Hello from QML: please update some data");
            }
        }
    }
}
