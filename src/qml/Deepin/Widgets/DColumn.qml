import QtQuick 2.0

Item {
    id:root
    property list<QtObject> datas
    default property alias _onlyWorkaround_: root.datas
    width:parent.width
    Component {
        id:seperator
        DSeparatorHorizontal{}
    }
    Column {
        Repeater {
            model: datas
            delegate: Component {
                Column {
                    width: root.width
                    id: i
                    Component.onCompleted: {
                        if (root.datas[index].active !== false) {
                            //Let Loader(or other Component) with empty content work with this trick
                            i.children = root.datas[index]
                            if (root.datas[index].width != 0) {
                                var o = seperator.createObject(i)
                                o.width = root.width
                            }
                        }
                    }
                }
            }
        }
    }
    function getMethods(obj)
    {
        var res = [];
        for(var m in obj) {
            if(typeof obj[m] == "function") {
                res.push(m)
            }
        }
        return res;
    }
}
