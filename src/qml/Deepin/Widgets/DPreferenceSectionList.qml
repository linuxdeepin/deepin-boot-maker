// Notice: this file is not intend to be used separately,
// there must be a item called `root' which has a property named `currentSectionId'
// and accessable from this scope to work properly.
import QtQuick 2.1

ListView {
	id: listview
	width: 300
	height: childrenRect.height

	property var sections
    property int cellHeight: 30
    
    DConstants { id: dconstants }

	model: ListModel {
		Component.onCompleted: {
			var sections = listview.sections || []
			for (var i = 0; i < sections.length; i++) {
				append(sections[i])
			}
		}
	}
	delegate: Component {
		Column {
			id: main_column
			state: "normal"
			width: ListView.view.width
            
            Component.onCompleted: root.anotherSectionCompleted()

			states: [
				State {
					name: "normal"
					PropertyChanges {
						target: txt
						color: dconstants.hoverColor
					}
					/* PropertyChanges { */
					/* 	target: sub */
					/* 	visible: false */
					/* } */
				},
				State {
					name: "selected"
					PropertyChanges {
						target: txt
						color: isParent ? dconstants.hoverColor : dconstants.activeColor
					}
					/* PropertyChanges { */
					/* 	target: sub */
					/* 	visible: isParent ? true : false */
					/* } */
				}
			]

			property bool isParent: subSections.count != 0
            
            onStateChanged: {
                if (state == "selected" && !isParent) {
                    root.changeIndicatorPos(main_column.parent.mapToItem(root, main_column.x, main_column.y).y)
                }
            }

			Connections {
				target: root

				onCurrentSectionIdChanged: {
					if (sectionId == root.currentSectionId) {
						main_column.state = "selected"
					} else if (!main_column.isParent) {
						main_column.state = "normal"
					}
				}
			}

			Item {
				width: main_column.width
				height: listview.cellHeight

				Text {
					id: txt
					text: sectionName
                    elide: Text.ElideRight
					font.pixelSize: 16

                    anchors.left: parent.left
                    anchors.leftMargin: 10
					anchors.verticalCenter: parent.verticalCenter
				}

				MouseArea {
					id: main_column_mouse
					anchors.fill: parent

					onClicked: {
						root.currentSectionId = sectionId
                        root.sectionSelected(sectionId)
					}
				}
			}

			Loader {
				id: sub
				x: 10
				active: main_column.isParent
				source: "DPreferenceSectionList.qml"
				asynchronous: true
				onLoaded: {
					item.model = subSections
                    item.width = main_column.width - 10
				}
			}
		}
	}
}