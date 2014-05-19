import QtQuick 2.0
import GameEngine 1.0

Rectangle {
	Column {
		spacing: 5
		anchors.fill: parent
		anchors.margins: 5
		Text {
			text: "fps:   " + engine.fpsCount
		}
		Text {
			text: "mfps:   " + engine.mediumFSP
		}
		Text {
			text: "frames: " + engine.frames
		}
		Text {
			text: "score: " + engine.score
		}
		Text {
			text: "lifes: "
		}
		Row {
			spacing: 5
			Repeater {
				model: engine.lifesCounter

				Rectangle {
					width: 15
					height: width
					radius: width/2
					color: "green"
				}
			}
		}

		
		Item { width: 50; height: 50}
		Text { text: "Instructions: " }

		Text { text: "Space to start" }
		Text { text: "R to restart game" }
		Text { text: "P to pause game" }
		Text { text: "+ to increase speed" }
		Text { text: "- to decrease speed" }
	}
}
