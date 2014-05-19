import QtQuick 2.0
import GameEngine 1.0

MoveableItem {
	id: ball
	
	opacity: 0.0
	
	Component.onCompleted: engine.registerBall(ball)
	
	Rectangle {
		anchors.fill: parent
		color: "grey"
		radius: width/2
	}
}
