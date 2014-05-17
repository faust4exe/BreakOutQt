import QtQuick 2.0

Rectangle {
	id: root

	width: 800
	height: 600

	border.color: "black"
	border.width: 5

	focus: true

	Item {
		id: field

		width: parent.width
		height: parent.height/2.0

		Grid {
			id: grid
			property int colWidth: 100
			property int rowHeight: 25
			property int colsCount: grid.width/colWidth
			property int rowCount: grid.height/rowHeight

			columns: colsCount

			anchors.fill: parent
			spacing: 5

			Repeater {
				model: grid.rowCount * grid.colsCount

				Rectangle {
					id: itemRect
					color: {
						var yGlobal = mapToItem(grid, x, y).y
						console.log("y " + yGlobal/root.height)
						return Qt.hsla(yGlobal/root.height, 1, 0.5, 1)
					}
					width: 95
					height: 20

					property int centerX: x + width/2
					property int centerY: y + height/2

					Rectangle {
						width: 1
						height: 1
						anchors.centerIn: parent
					}

					Item {
						id: itemDetector
						anchors.fill: parent
						anchors.margins: -ball.width/2

						Connections {
							target: ball
							onCheckCollision: {
								if (itemRect.opacity == 0)
									return

								var ballOnPlayer = itemDetector.mapFromItem(root, senderX, senderY)

								if (itemDetector.contains(Qt.point(ballOnPlayer.x, ballOnPlayer.y))) {

									sender.ySpeed = Math.abs(sender.ySpeed)

									itemRect.opacity = 0.0
								}
							}
						}
					}
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

	Rectangle {
		id: ball

		width: 50/2
		height: width
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

		x: player.x + player.width/2
		y: root.height - 50

		color: "black"

		signal checkCollision(var sender, int senderX, int senderY)

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
				ball.y = root.height - 50
				moveTimer.running = true
			}
		}

		Rectangle {
			width: 1
			height: 1
			anchors.centerIn: parent
		}

		function step()
		{
			x += xSpeed
			y += ySpeed

			if (x <= 0)
				xSpeed = -xSpeed
			if (x + ball.width > root.width)
				xSpeed = -xSpeed

			if (y <= 0)
				ySpeed = -ySpeed

			if (y + ball.height > root.height) {
				ySpeed = 0
				xSpeed = 0
				moveTimer.running = false
				console.log("GAME OVER")

				resetGame()
			}

		}

		function checkCollisions()
		{
			ball.checkCollision(ball, centerX, centerY)
		}

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

	Rectangle {
		id: player

		width: 100
		height: 25

		x: parent.height/2

		color: "black"

		anchors.bottom: parent.bottom
		anchors.bottomMargin: root.border.width

		property int moveDirection: 0
		property int moveSpeed: root.width/moveTimer.interval/4

		property int centerX: x + width/2
		property int centerY: y + height/2

		MouseArea {
			anchors.fill: parent
			drag.threshold: 0
			drag.target: player
			drag.axis: Drag.XAxis
			drag.minimumY: 0
			drag.maximumX: root.width - player.width
		}

		Rectangle {
			width: 1
			height: 1
			anchors.centerIn: parent
		}

		Item {
			id: playerDetector
			anchors.fill: parent
			anchors.margins: -ball.width/2

			Connections {
				target: ball
				onCheckCollision: {
					var ballOnPlayer = playerDetector.mapFromItem(root, senderX, senderY)

					if (playerDetector.contains(Qt.point(ballOnPlayer.x, ballOnPlayer.y))) {
						sender.hitOn(player.centerX, player.centerY)
					}
				}
			}
		}


		function step()
		{
			x = checkBound(x + moveSpeed * moveDirection)
		}

		function checkBound(x)
		{
			return Math.max(0, Math.min(parent.width - player.width, x))
		}
	}

	function resetGame ()
	{
		ball.x = player.x + player.width/2
		ball.y = player.y - ball.height
		ball.xSpeed = 5
		ball.ySpeed = -5
		moveTimer.running = true
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

		if (event.text == "P"
				|| event.text == "p") {
			moveTimer.running = !moveTimer.running
			return
		}

		if (event.text == "F"
				|| event.text == "f") {
			moveTimer.interval = moveTimer.interval / 10
			return
		}

		if (event.text == "S"
				|| event.text == "s") {
			moveTimer.interval = moveTimer.interval * 10
			return
		}

		if (event.key == Qt.Key_Left) {
			player.moveDirection = -1
			event.accepted = true
		}

		if (event.key == Qt.Key_Right) {
			player.moveDirection = 1
			event.accepted = true
		}
	}

	Keys.onReleased: {
		if (event.isAutoRepeat)
			return

		if (event.text == "F"
				|| event.text == "f") {
			moveTimer.interval = moveTimer.interval * 10
			return
		}

		if (event.text == "S"
				|| event.text == "s") {
			moveTimer.interval = moveTimer.interval / 10
			return
		}

		if (event.key == Qt.Key_Left
				|| event.key == Qt.Key_Right) {
			player.moveDirection = 0
			event.accepted = true
		}
	}

	property int fps: 0

	Timer {
		id: moveTimer
		interval: 1000/60
		running: true
		repeat: true

		signal step()

		onTriggered: {
			moveTimer.step()

			player.step()

			ball.step()

			ball.checkCollision(ball, ball.centerX, ball.centerY)
		}
	}

	MouseArea {
		anchors.fill: parent
		hoverEnabled: true
		onMouseXChanged: player.x = mouseX - player.width/2
	}
}
