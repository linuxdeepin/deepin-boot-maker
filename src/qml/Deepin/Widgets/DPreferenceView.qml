import QtQuick 2.1

Row {
    id: root
    width: 300
    height: 300
    spacing: 10

    property int sectionListWidth: 200

    property string currentSectionId

    default property alias content: col.children
    property alias sections: section_list.sections

    function changeIndicatorPos(pos) {
        section_indicator.pointerPos = pos + section_list.cellHeight / 2
    }
    
    signal sectionSelected (string sectionId)
    signal anotherSectionCompleted ()

    onSectionSelected: {
        preference_content.scrollTo(sectionId)
    }
    
    onAnotherSectionCompleted: {
        indicate_first_timer.restart()
    }

    DPreferenceSectionList {
        id: section_list
        width: root.sectionListWidth
        height: root.height
    }

    DPreferenceSectionIndicator { id: section_indicator; height: root.height}

    Item {
        clip: true
        width: root.width - section_list.width - section_indicator.width - root.spacing * 2
        height: root.height

        Flickable {
            id: preference_content
            anchors.fill: parent
            contentWidth: col.childrenRect.width
            contentHeight: col.childrenRect.height
            flickableDirection: Flickable.VerticalFlick

            function scrollTo(sectionId) {
                var children = col.visibleChildren
                for (var i = 0; i < children.length; i++) {
                    if (children[i].sectionId == sectionId) {
                        contentY = children[i].y
                    }
                }
            }

            onMovementEnded: {
                if(atYEnd) {
                    root.currentSectionId = col.visibleChildren[col.visibleChildren.length - 1].sectionId
                } else {
                    root.currentSectionId = col.childAt(50, contentY).sectionId
                }
            }

            Timer {
                id: indicate_first_timer
                interval: 200
                onTriggered: {
                    root.currentSectionId = col.visibleChildren[0].sectionId
                    root.sectionSelected(col.visibleChildren[0].sectionId)
                }
            }

            Column {
                id: col
                width: preference_content.width
                height: root.height
            }
        }
    }
}