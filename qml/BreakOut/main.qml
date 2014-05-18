import QtQuick 2.0
import GameEngine 1.0

Rectangle {
	id: root

	width: 800
	height: 600

	Rectangle {
		id: playfield

		anchors.top: parent.top
		anchors.left: parent.left
		height: parent.height
		width: parent.width - infoPanel.width

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
				property int colWidth: 100
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

					ElasticItem {
						id: itemRect

						Rectangle {
							anchors.fill: parent
							color:  {
								var yGlobal = itemRect.mapToItem(grid, itemRect.x, itemRect.y).y
								return Qt.hsla(yGlobal/playfield.height, 1, 0.5, 1)
							}
							border.color: "black"
							border.width: itemRect.active ? 5 : 1
						}

						width: grid.colWidth
						height: grid.rowHeight

						property int row: index / grid.colsCount
						property int column: index % grid.colsCount

						Component.onCompleted: engine.registerItem(itemRect)

					/*Item {
							id: itemDetector

							Connections {
								target: ball
								onCheckCollision: {
									if (itemRect.opacity == 0)
										return

									var ballOnPlayer = itemDetector.mapFromItem(playfield, senderX, senderY)

									if (itemDetector.contains(Qt.point(ballOnPlayer.x, ballOnPlayer.y))) {

										sender.ySpeed = Math.abs(sender.ySpeed)

										itemRect.opacity = 0.0
									}
								}
							}

						}*/
					}
				}
			}
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

		MoveableItem {
			id: ball

			opacity: 0.0
			width: 50/2
			height: width

			Component.onCompleted: engine.registerBall(ball)

			Rectangle {
				anchors.fill: parent
				color: "grey"
				radius: width/2
			}

			x: playfield.width/2
			y: playfield.height - player.height * 2

			signal checkCollision(var sender, int senderX, int senderY)

			speedX: 100*2
			speedY: -100*2
		}

		MoveableElasticItem {
			id: player

			x: parent.height/2
			width: 100
			height: 25

			anchors.bottom: parent.bottom
			anchors.bottomMargin: playfield.border.width

			property int moveSpeed: 500

			Component.onCompleted: engine.registerPlayer(player)

			Rectangle {
				anchors.fill: parent
				color: "black"
			}
		}

		function resetGame ()
		{
			ball.opacity = 1.0
			ball.x = player.x + player.width/2
			ball.y = player.y - ball.height * 2
			ball.speedX = 100
			ball.speedY = -100
			engine.restartGame()
		}

		Keys.onPressed: {
			if (event.isAutoRepeat)
				return

			if (event.text == "R"
					|| event.text == "r") {
				resetGame ()
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
				ball.opacity = 1.0
				ball.x = player.x + player.width/2
				ball.y = player.y - ball.height * 2
				ball.speedX = 100
				ball.speedY = -100
				engine.start(true)
				return
			}

			if (event.text == "X"
					|| event.text == "x") {
				if (engine.running)
					engine.stop()
				else
					engine.start()
				return
			}

			if (event.text == "P"
					|| event.text == "p") {
				ball.opacity = 1.0
				if (engine.running)
					engine.stop()
				else
					engine.start()
				return
			}

			if (event.text == "F"
					|| event.text == "f") {
				engine.setFpsLimit(engine.fpsLimit*2)
				return
			}

			if (event.text == "S"
					|| event.text == "s") {
				engine.setFpsLimit(engine.fpsLimit/2)
				return
			}

			if (event.key == Qt.Key_Left) {
				player.speedX = -player.moveSpeed
				event.accepted = true
			}

			if (event.key == Qt.Key_Right) {
				player.speedX = player.moveSpeed
				event.accepted = true
			}
		}

		Keys.onReleased: {
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
				var newPos = mouseX - player.width/2
				newPos = Math.min(newPos, playfield.width - player.width)
				player.x = Math.max(0, newPos)
			}
		}
	}

	InfoPanel {
		id: infoPanel

		height: parent.height
		width: 250

		anchors.right: parent.right
	}
}
