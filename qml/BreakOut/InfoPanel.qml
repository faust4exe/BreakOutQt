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
		
		Item { width: 50; height: 50}
		Text { text: "Instructions: " }

		Text { text: "Space to start" }
		Text { text: "R to restart game" }
		Text { text: "P to pause game" }
		Text { text: "+ to increase speed" }
		Text { text: "- to decrease speed" }
	}
}
