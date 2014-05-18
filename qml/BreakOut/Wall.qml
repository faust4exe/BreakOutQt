import QtQuick 2.0
import GameEngine 1.0

ElasticItem {
	id: wall

	Component.onCompleted: engine.registerWall(wall)
	
	Rectangle {
		anchors.fill: parent
		color: "black"
	}
}
