import QtQuick 2.0
import GameEngine 1.0

MoveableElasticItem {
	id: player

	Component.onCompleted: engine.registerPlayer(player)
	
	Rectangle {
		anchors.fill: parent
		color: "black"
		radius: width/2
	}
}
