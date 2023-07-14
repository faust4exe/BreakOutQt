import QtQuick 2.0
import GameEngine 1.0

Rectangle {
    Grid {
        rows: height < width ? 1 : 3
        spacing: 5
		anchors.fill: parent
		anchors.margins: 5

        Column {
            Text { text: "Indicators:" ; font.bold: true}
            Text {
                text: "medium fps:   " + engine.mediumFSP
                width: 120
            }
            Text {
                text: "fps limit:   " + engine.fpsLimit
            }
            Item { height: 10; width: 10 }
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
                        color: "grey"
                    }
                }
            }
        }

		Item { width: 50; height: 50}

        Column {
            Text { text: "Instructions: " ; font.bold: true}

            Text { text: "[R] to restart game" }
            Text { text: "[P] to pause game" }
            Text { text: "[+] to increase speed" }
            Text { text: "[-] to decrease speed" }
            Text { text: "[F|S] to change fps limit" }
            Text { text: "[G] GOD mode"; font.bold: engine.godMode }
        }
	}
}
