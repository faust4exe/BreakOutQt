import QtQuick 2.0
import GameEngine 1.0

BonusItem {
	id: root

	Rectangle {
		anchors.fill: parent
		radius: width/4
		color: root.bonus > 3 ? "red" : "green"
		
		Text {
			anchors.centerIn: parent
			text: root.text
		}
	}
	
	Component.onCompleted: engine.registerBonus(root)
}
