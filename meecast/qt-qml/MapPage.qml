//import QtQuick 1.1
import Qt 4.7
import com.nokia.meego 1.0

Page {
    id: mapweather
    property int margin: 16
    property string map_pattern: ""
    property string filename: ""
    property string filenamebackground: ""
    property string count_of_maps: ""
    tools: ToolBarLayout {
        ToolIcon {
            iconId: "toolbar-back"
            onClicked: {
                //menu.close();
                pageStack.pop();
            }
        }
   
        ToolIcon {
            iconId: "toolbar-view-menu"
            onClicked: {(myMenu.status == DialogStatus.Closed) ? myMenu.open() : myMenu.close()}
            anchors.right: parent == undefined ? undefined : parent.right
        }

    }
    orientationLock: PageOrientation.LockPortrait
    function openFile(file)
    {
        var component = Qt.createComponent(file);
        if (component.status == Component.Ready){
            pageStack.push(component);
        }else {
            console.log("error open file "+file);
        }
    }
    Rectangle {
        id: maprectangle
        property int velocity: 50
        property int currentImage: 0

        Image {
            id: mapimage
            source: filename 
            opacity: 1
            // For Full Screen
            transform: Rotation { origin.x: 240; origin.y: 240; angle: 90}
         }

        Timer {
            interval: 1000
            running: true;
            repeat: true;
            onTriggered: {
                     parent.currentImage = parent.currentImage - 1;
                     if (parent.currentImage < 0) 
                          parent.currentImage = count_of_maps;
                     filename = map_pattern.replace("%s", parent.currentImage);
            }
        }
    }
    Component.onCompleted: {
        filename = map_pattern.replace("%s", (count_of_maps));
    }

}

