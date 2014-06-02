import QtQuick 2.1

GridView {
    id: select_view

    interactive: false
    cellWidth: width / columns
    cellHeight: height / rows
    property bool singleSelectionMode: false

    property int rows: 1
    property int columns: 3
    property int viewWidth: 80
    property int viewHeight: 24

    property var selectedIndexs: []
    property var selectedItems: []

    signal select(int index, var item)
    signal deselect(int index, var item)

    function selectItem(index) {
        select_view.getDelegateInstanceAt(index).select()
    }

    function deselectItem(index) {
        select_view.getDelegateInstanceAt(index).deselect()
    }

    function clear() {
        for (var index in selectedItems) {
            selectedItems[index].deselect()
        }
        selectedItems = []
        selectedIndexs = []
    }

    function getDelegateInstanceAt(index) {
        for(var i = 0; i < contentItem.children.length; ++i) {
            var item = contentItem.children[i];
            if ((typeof item.delegateIndex != "undefined") && item.delegateIndex == index) {
                return item
            }
        }
        return undefined
    }

    delegate: DSelectViewDelegate{}
}
