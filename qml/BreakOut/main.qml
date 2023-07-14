import QtQuick 2.0
import GameEngine 1.0

Rectangle {
	id: root

	width: 800
	height: 600

    readonly property bool isVertical: height > width

    Item {
        id: orientationStateHolder
        states: [
            State {
                name: "verticalOrientation"
                when: root.isVertical
                PropertyChanges {
                    target: playfield
                    anchors.right: root.right
                    anchors.top: infoPanel.bottom
                }
                PropertyChanges {
                    target: infoPanel
                    anchors.left: root.left
                    height: 160
                }
            }
        ]
    }

	Rectangle {
		id: playfield

		anchors.top: parent.top
		anchors.left: parent.left
        anchors.bottom: parent.bottom
        anchors.right: infoPanel.left

		border.color: "black"
		border.width: 5

		focus: true

		Item {
			id: field

			anchors.top: parent.top
			anchors.left: parent.left
			anchors.right: parent.right
			anchors.margins: playfield.border.width * 2

			height: parent.height * 0.40

			Grid {
				id: grid
				property int colWidth: 50
				property int rowHeight: 25

				property int colsCount: (field.width+spacing)/(colWidth+spacing)
				property int rowsCount: (field.height+spacing)/(rowHeight+spacing)

				onColsCountChanged: engine.setColumnsCount(colsCount)
				onRowsCountChanged: engine.setRowsCount(rowsCount)

				columns: colsCount

				width: colsCount * (colWidth+spacing)
				height: rowsCount * (rowHeight+spacing)

				anchors.centerIn: parent
				spacing: 5

				Repeater {
					model: grid.rowsCount * grid.colsCount

					Brick {
						id: brick

						width: grid.colWidth
						height: grid.rowHeight

						property int row: index / grid.colsCount
						property int column: index % grid.colsCount

						itemColor:{
							var yGlobal = root.mapToItem(grid, brick.x, brick.y).y
							return Qt.hsla(yGlobal/grid.height, 1, 0.5, 1)
						}
					}
				}
			}
		}

		// bonus
		Bonus {
			id: bonusItem

			width: 25
			height: width
			opacity: 0
		}

		//walls
		Wall {
			id: topWall

			width: parent.width
			height: 100

			anchors.bottom: parent.top
			anchors.bottomMargin: -5
		}
		Wall {
			id: bottomWall

			width: parent.width
			height: 100

			anchors.top: parent.bottom
			anchors.topMargin: -5

			isDeadly: true
		}
		Wall {
			id: leftWall

			width: 100
			height: parent.height

			anchors.right: parent.left
			anchors.rightMargin: -5
		}
		Wall {
			id: rightWall

			width: 100
			height: parent.height

			anchors.left: parent.right
			anchors.leftMargin: -5
		}

		// balls
		BallItem {
			id: ball

			width: 50/2
			height: width

			x: playfield.width/2
			y: playfield.height - player.height * 2
		}
		BallItem {
			width: 50/2
			height: width

			x: playfield.width/2
			y: playfield.height - player.height * 2
		}
		BallItem {
			width: 50/2
			height: width

			x: playfield.width/2
			y: playfield.height - player.height * 2
		}

		PlayerItem {
			id: player

			x: parent.height/2
			width: 100
			height: 25

			anchors.bottom: parent.bottom
			anchors.bottomMargin: playfield.border.width

			property int moveSpeed: 500
		}

		function resetGame ()
		{
			engine.restartGame()
			ball.opacity = 1.0
			ball.x = player.x + player.width/2
			ball.y = player.y - ball.height * 2
			ball.speedX = 100
			ball.speedY = -100
			engine.lifesCounter--
		}

        function startGame()
        {
            engine.lifesCounter--
            root.state = "playing"
            ball.opacity = 1.0
            ball.x = player.x + player.width/2
            ball.y = player.y - ball.height * 2
            ball.speedX = 100
            ball.speedY = -100
            engine.start(true)
        }

        Keys.onPressed: function(event) {
			if (event.isAutoRepeat)
				return

			if (event.text == "R"
					|| event.text == "r") {

				root.state = "playing"
                playfield.resetGame()
				engine.start(true)
				return
			}

			if (event.key == Qt.Key_Plus) {
				ball.speedX = ball.speedX * 1.25
				ball.speedY = ball.speedY * 1.25
				return
			}

			if (event.key == Qt.Key_Minus) {
				ball.speedX = ball.speedX / 1.25
				ball.speedY = ball.speedY / 1.25
				return
			}

			if (event.key == Qt.Key_Space) {
				if (root.state != "welcome"
						&& root.state != "balllosed")
					return

                playfield.startGame()
				return
			}

			if (event.text == "P"
					|| event.text == "p") {
				if (root.state != "paused" &&
						root.state != "playing")
					return

				if (engine.running) {
					root.state = "paused"
					engine.stop()
				}
				else {
					root.state = "playing"
					engine.start()
				}
				return
			}

			if (event.text == "F"
					|| event.text == "f") {
				engine.setFpsLimit(engine.fpsLimit*2)
				return
			}

			if (event.text == "G"
					|| event.text == "g") {
				engine.godMode = !engine.godMode
				return
			}

			if (event.text == "S"
					|| event.text == "s") {
				engine.setFpsLimit(engine.fpsLimit/2)
				return
			}

			if (event.key == Qt.Key_Left) {
				player.speedX = -player.moveSpeed
			}

			if (event.key == Qt.Key_Right) {
				player.speedX = player.moveSpeed
			}
		}

        Keys.onReleased: function(event) {
			if (event.isAutoRepeat)
				return

			if (event.key == Qt.Key_Left
					|| event.key == Qt.Key_Right) {
				player.speedX = 0
			}
		}

		MouseArea {
			anchors.fill: parent
			hoverEnabled: true
			onMouseXChanged: {
				if (!engine.running)
					return
				var newPos = mouseX - player.width/2
				newPos = Math.min(newPos, playfield.width - player.width)
				player.x = Math.max(0, newPos)
			}
            onPressed: {
                if (root.state == "welcome"
                        || root.state == "balllosed") {
                    playfield.startGame()
                    return
                }
                if (root.state == "gameover") {

                    root.state = "playing"
                    playfield.resetGame()
                    engine.start(true)
                    return
                }
            }
		}
	}

	InfoPanel {
		id: infoPanel

		height: parent.height
		width: 230

        anchors.right: root.right
	}

	Connections {
		target: engine
        function onGameOver(){ root.state = "gameover" }
        function onBallLosed(){ root.state = "balllosed" }
        function onYouWin(){ root.state = "youwin" }
	}

	Rectangle {
		id: infoScreen
		opacity: 0.0
		anchors.fill: parent

		property string capture: ""
		property string content: ""
		Text {
			id: textCapture
			anchors.centerIn: parent
			font.pointSize: 32
			text: parent.capture
		}

		Text {
			anchors.centerIn: parent
			anchors.verticalCenterOffset: textCapture.height
			font.pointSize: 16
			text: parent.content
		}
	}

	Component.onCompleted: state = "welcome"

	states: [
		State {
			name: "welcome"
			PropertyChanges {
				target: infoScreen
				opacity: 0.95
				capture: "BreakOut game"
				content: "Press [Space] to start the game"
			}
		},
		State {
			name: "playing"
		},
		State {
			name: "balllosed"
			PropertyChanges {
				target: infoScreen
				opacity: 0.95
				capture: "ball losed"
				content: "Press [Space] to try one more time"
			}
		},
		State {
			name: "paused"
			PropertyChanges {
				target: infoScreen
				opacity: 0.95
				capture: "paused"
				content: "Press [P] to continue the game"
			}
		},
		State {
			name: "gameover"
			PropertyChanges {
				target: infoScreen
				opacity: 0.95
				capture: "Game Over"
				content: "Press [R] to restart the game"
			}
		},
		State {
			name: "youwin"
			PropertyChanges {
				target: infoScreen
				opacity: 0.95
				capture: "You Win !!"
				content: "Press [R] to try again"
			}
		}
	]
}
