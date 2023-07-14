import QtQuick 2.0
import GameEngine 1.0

ElasticItem {
	id: root

	property color itemColor: "red"
	
	Rectangle {
		anchors.fill: parent
		color: itemColor

		border.color: "black"
		border.width: 1
	}

	Component.onCompleted: engine.registerItem(root)
    Component.onDestruction: engine.unregisterItem(root)
}
