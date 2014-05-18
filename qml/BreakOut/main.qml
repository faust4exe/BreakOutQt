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
		width: parent.width - 200

		border.color: "black"
		border.width: 5

		focus: true

//		Component.onCompleted: engine.registerPlayfield(playfield)

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
//				anchors.fill: parent
				anchors.centerIn: parent
				spacing: 5

				Repeater {
					model: grid.rowsCount * grid.colsCount

//					Rectangle {
					ElasticItem {
						id: itemRect

						Rectangle {
							anchors.fill: parent
							color: itemRect.active ? "red" : "black" /*{
								var yGlobal = mapToItem(grid, x, y).y
								return Qt.hsla(yGlobal/playfield.height, 1, 0.5, 1)
							}*/
						}
						width: grid.colWidth
						height: grid.rowHeight

//						offsetX: grid.x
//						offsetY: grid.y

//						property int centerX: x + width/2
//						property int centerY: y + height/2

						property int row: index / grid.colsCount
						property int column: index % grid.colsCount

						Component.onCompleted: engine.registerItem(itemRect)

						function test()
						{
							console.log("ITEM HIT")
						}

						/*Rectangle {
							width: 1
							height: 1
							anchors.centerIn: parent
						}*/

						/*Item {
							id: itemDetector
							anchors.fill: parent
							anchors.margins: -ball.width/2

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

							Rectangle {
								anchors.fill: parent
								color: "transparent"
								border.width: 1
								border.color: "black"
							}
						}*/
					}
				}
			}

			Rectangle {
				anchors.fill: parent
				border.color: "red"
				border.width: 1
				color: "transparent"
			}
		}

		//walls
//		Rectangle {
		ElasticItem {
			id: topWall
			width: parent.width
			height: 100
			anchors.bottom: parent.top
			anchors.bottomMargin: -5
			Component.onCompleted: engine.registerWall(topWall)

			Rectangle {
				anchors.fill: parent
				color: "red"
			}
			function onHit()
			{
				console.log("Top HIT")
			}
		}
//		Rectangle {
		ElasticItem {
			id: bottomWall
			width: parent.width
			height: 100
			anchors.top: parent.bottom
			anchors.topMargin: -5
			Component.onCompleted: engine.registerWall(bottomWall)

			Rectangle {
				anchors.fill: parent
				color: "red"
			}
			function onHit()
			{
				console.log("Bottom HIT")
			}
		}
		ElasticItem {
//		Rectangle {
			id: leftWall
			width: 100
			height: parent.height
			anchors.right: parent.left
			anchors.rightMargin: -5
			Component.onCompleted: engine.registerWall(leftWall)

			Rectangle {
				anchors.fill: parent
				color: "red"
			}

			function onHit()
			{
				console.log("Left HIT")
			}
		}
		ElasticItem {
//		Rectangle {
			id: rightWall
			width: 100
			height: parent.height
			anchors.left: parent.right
			anchors.leftMargin: -5
			Component.onCompleted: engine.registerWall(rightWall)

			Rectangle {
				anchors.fill: parent
				color: "red"
			}
			function onHit()
			{
				console.log("Right HIT")
			}
		}


//		Rectangle {
		MoveableItem {
			id: ball

			width: 50/2
			height: width

			Component.onCompleted: engine.registerBall(ball)

			Rectangle {
				anchors.fill: parent
				color: "black"
				radius: width/2
				gradient: Gradient {
				GradientStop {
					position: 0.00;
					color: "#4687e2";
				}
				GradientStop {
					position: 0.01;
					color: "#afb8ee";
				}
				GradientStop {
					position: 1.00;
					color: "#071897";
				}
				}

			}
			x: playfield.width/2
			y: playfield.height - ball.height * 2

//			onXChanged: console.log("x="+x)
//			onYChanged: console.log("y="+y)



			signal checkCollision(var sender, int senderX, int senderY)

			speedX: 100*2
			speedY: -100*2
			property double xSpeed: 10/2
			property double ySpeed: -10/2

			property int centerX: x + width/2
			property int centerY: y + height/2

			MouseArea {
				anchors.fill: parent
				onClicked: {
					ball.xSpeed = 5
					ball.ySpeed = -5
					ball.x = 50
					ball.y = playfield.height - 50
					moveTimer.running = true
				}
			}

			/*Rectangle {
				width: 1
				height: 1
				anchors.centerIn: parent
			}*/

			function step()
			{
				x += xSpeed
				y += ySpeed

				if (x <= 0)
					xSpeed = -xSpeed
				if (x + ball.width > playfield.width)
					xSpeed = -xSpeed

				if (y <= 0)
					ySpeed = -ySpeed

				if (y + ball.height > playfield.height) {
					ySpeed = 0
					xSpeed = 0
					moveTimer.running = false
					console.log("GAME OVER")

					playfield.resetGame()
				}
			}

			/*function checkCollisions()
			{
				ball.checkCollision(ball, centerX, centerY)
			}*/

			function hitOn(posX, posY)
			{
				console.log("Ball on  " + centerX + " " + centerY)
				console.log("Hit  on  " + posX + " " + posY)
				console.log("speed  " + xSpeed + " " + ySpeed)

				var xdiff = centerX - posX
				var ydiff = posY - centerY

				console.log("diff on  " + xdiff + " " + ydiff)

				// get angle and make correction
				var theAngle = Math.atan(Math.abs(ydiff) / Math.abs(xdiff))
				var theAngleGrage = theAngle * 180 / 3.1415
				if (ydiff < 0)
					theAngleGrage = -theAngleGrage
				if (xdiff < 0)
					theAngleGrage = 180 - theAngleGrage

				var theSpeed = Math.sqrt(Math.pow(xSpeed, 2)
										 + Math.pow(ySpeed, 2))

				console.log("speed  " + theSpeed)
				console.log("angle grade " + theAngleGrage)

				ball.ySpeed = -Math.sin(theAngleGrage/180*3.1415) * theSpeed
				ball.xSpeed = Math.cos(theAngleGrage/180*3.1415) * theSpeed

				console.log("New speed  " + xSpeed + " " + ySpeed)
				console.log("------------------------------------------------------")
			}
		}

//		Rectangle {
//		MoveableItem {
		MoveableElasticItem {
			id: player

			width: 100
			height: 25

			x: parent.height/2

			Rectangle {
				anchors.fill: parent
				color: "black"
			}

			/*ElasticItem {
				id: playerElsatic
				anchors.fill: parent
				Component.onCompleted: engine.registerPlayer(playerElsatic)
			}*/

			Component.onCompleted: engine.registerPlayer(player)

			anchors.bottom: parent.bottom
			anchors.bottomMargin: playfield.border.width

//			property int moveDirection: 0
			property int moveSpeed: 500//playfield.width/moveTimer.interval/4

//			property int centerX: x + width/2
//			property int centerY: y + height/2

			/*MouseArea {
				anchors.fill: parent
				drag.threshold: 0
				drag.target: player
				drag.axis: Drag.XAxis
				drag.minimumY: 0
				drag.maximumX: playfield.width - player.width
			}*/

			/*Rectangle {
				width: 1
				height: 1
				anchors.centerIn: parent
			}*/

			/*Item {
				id: playerDetector
				anchors.fill: parent
				anchors.margins: -ball.width/2

				Connections {
					target: ball
					onCheckCollision: {
						var ballOnPlayer = playerDetector.mapFromItem(playfield, senderX, senderY)

						if (playerDetector.contains(Qt.point(ballOnPlayer.x, ballOnPlayer.y))) {
							sender.hitOn(player.centerX, player.centerY)
						}
					}
				}

				Rectangle {
					anchors.fill: parent
					color: "transparent"
					border.width: 1
					border.color: "black"
				}
			}*/


			/*function step()
			{
				console.log("Player step")
				x = checkBound(x + moveSpeed * moveDirection)
			}*/

			/*function checkBound(x)
			{
				return Math.max(0, Math.min(parent.width - player.width, x))
			}*/
		}

		function resetGame ()
		{
			ball.x = player.x + player.width/2
			ball.y = player.y - ball.height * 2
			ball.xSpeed = 5
			ball.ySpeed = -5
			engine.start()
//			moveTimer.running = true
		}

		Keys.onPressed: {
			if (event.isAutoRepeat)
				return

			if (event.text == "R"
					|| event.text == "r"
					|| event.key == Qt.Key_Space) {
				resetGame ()
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
//				moveTimer.running = !moveTimer.running
				if (engine.running)
					engine.stop()
				else
					engine.start()
				return
			}

			if (event.text == "F"
					|| event.text == "f") {

//				moveTimer.interval = moveTimer.interval / 10
				return
			}

			if (event.text == "S"
					|| event.text == "s") {
//				moveTimer.interval = moveTimer.interval * 10
				return
			}

			if (event.key == Qt.Key_Left) {
//				player.moveDirection = -1
				player.speedX = -player.moveSpeed//-100
				event.accepted = true
			}

			if (event.key == Qt.Key_Right) {
//				player.moveDirection = 1
				player.speedX = player.moveSpeed//100
				event.accepted = true
			}
		}

		Keys.onReleased: {
			if (event.isAutoRepeat)
				return

			if (event.text == "F"
					|| event.text == "f") {
//				moveTimer.interval = moveTimer.interval * 10
				return
			}

			if (event.text == "S"
					|| event.text == "s") {
//				moveTimer.interval = moveTimer.interval / 10
				return
			}

			if (event.key == Qt.Key_Left
					|| event.key == Qt.Key_Right) {
//				player.moveDirection = 0
				player.speedX = 0
				event.accepted = true
			}
		}

//		Timer {
//			id: moveTimer
//			interval: 1000/60
//			running: true
//			repeat: true

//			signal step()

//			onTriggered: {
//				moveTimer.step()

//				player.step()

//				ball.step()

//				ball.checkCollision(ball, ball.centerX, ball.centerY)
//			}
//		}

		MouseArea {
			anchors.fill: parent
			hoverEnabled: true
			onMouseXChanged: player.x = mouseX - player.width/2
		}
	}

	Rectangle {
		height: parent.height
		width: 200

		anchors.right: parent.right

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
		}
	}

}
