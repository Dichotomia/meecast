import Qt 4.7

Rectangle {
    id: container

    property  alias text: label.text

    color: "#00000000"

    signal clicked

    width: label.width + 20
    height: 64

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        onReleased: {container.clicked()}
    }

    Text {
        id: label
        anchors.centerIn: parent
        font.pointSize: 16
        color: "white"
    }

    states: State {
        name: "pressed"
        when: mouseArea.pressed
        PropertyChanges {target: label; opacity: 0.7}
    }

}
