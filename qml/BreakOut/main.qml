import QtQuick 2.0

Rectangle {
	id: root

	width: 800
	height: 600

	border.color: "black"
	border.width: 5

	focus: true

    Text {
        text: qsTr("Hello World")
        anchors.centerIn: parent
    }

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
					color: "blue"
					width: 95
					height: 20

					Item {
						id: itemDetector
						anchors.fill: parent
						anchors.margins: -ball.width/2

						Connections {
							target: ball
							onCheckCollision: {
								if (itemRect.opacity == 0)
									return
								var ballOnPlayer = itemDetector.mapFromItem(root, ball.centerX, ball.centerY)
//								console.log("   ball pos player : " + ballOnPlayer.x + "," + ballOnPlayer.y)
								if (itemDetector.contains(Qt.point(ballOnPlayer.x, ballOnPlayer.y))) {
									ball.ySpeed = -ball.ySpeed
									itemRect.opacity = 0.0
									console.log("collision w item")
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

		x: 50
		y: root.height - 50

		color: "green"

		signal checkCollision()

		property int xSpeed: 10/2
		property int ySpeed: -10/2
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
			}

		}

		function checkCollisions()
		{
			var centerPoint = Qt.point(centerX, centerY)
//			var ballOnRoot = ball.mapToItem(root, centerX, centerY)
			var ballOnPlayer = playerDetector.mapFromItem(root, centerX, centerY)
//			console.log("   ball pos player : " + ballOnPlayer.x + "," + ballOnPlayer.y)
			if (playerDetector.contains(Qt.point(ballOnPlayer.x, ballOnPlayer.y))) {
				ball.ySpeed = -ball.ySpeed
				var playerCenterX = player.x + player.width/2
				var xDiff = (centerX - playerCenterX)
				ball.xSpeed = xDiff * 2.0 / player.width * 10.0
				console.log("collision w xDiff = " + xDiff)
			}

			ball.checkCollision()
		}
	}

	Rectangle {
		id: player
		width: 100
		height: 25

		x: parent.height/2

		color: "red"

		anchors.bottom: parent.bottom
		anchors.bottomMargin: root.border.width

		property int moveDirection: 1
		property int moveSpeed: 0
		property int maxSpeed: root.width/moveTimer.interval/4

		MouseArea {
			anchors.fill: parent
			drag.threshold: 0
			drag.target: player
			drag.axis: Drag.XAxis
			drag.minimumY: 0
			drag.maximumX: root.width - player.width
		}

		Item {
			id: playerDetector
			anchors.fill: parent
			anchors.margins: -ball.width/2
		}

		Timer {
			id: playerTimer
			interval: 100
			repeat: true
			onTriggered: {
				player.moveSpeed = Math.min(player.maxSpeed, player.moveSpeed * 2)
				console.log("move speed = " + player.moveSpeed)
			}
		}

		function step()
		{
			console.log("using move speed = " + player.moveSpeed)
			x = checkBound(x + maxSpeed * moveDirection)
//			x = checkBound(x + moveSpeed * moveDirection)

		}

		function increaseSpeed()
		{
//			player.moveSpeed = Math.min(player.maxSpeed, player.moveSpeed * 2)
//			console.log("move speed increase to = " + player.moveSpeed + "    while max = " + player.maxSpeed)
		}

		function checkBound(x)
		{
			return Math.max(0, Math.min(parent.width - player.width, x))
		}
	}

	property int posIncrement: 1

	Keys.onPressed: {
		if (event.text == "S" || event.text == "s") {
			ball.xSpeed = 5
			ball.ySpeed = -5
			ball.x = 50
			ball.y = root.height - 50
			moveTimer.running = true
			return
		}

		if (player.moveSpeed == 0 && (event.key == Qt.Key_Left || event.key == Qt.Key_Right)) {
			player.moveSpeed = 1
			console.log("Speed reset")
		}

		if (event.key == Qt.Key_Left) {
			console.log("Left Press")
			player.moveDirection = -1
//			player.increaseSpeed()
			playerTimer.running = true
//			player.moveSpeed = 1
//			playerTimer.running = true
			event.accepted = true
		}

		if (event.key == Qt.Key_Right) {
			console.log("Right Press")
			player.moveDirection = 1
//			player.increaseSpeed()
			playerTimer.running = true
//			player.moveSpeed = 1
//			playerTimer.running = true
			event.accepted = true
		}
	}

	Keys.onReleased: {
		if (event.isAutoRepeat)
			return
		if (event.key == Qt.Key_Left || event.key == Qt.Key_Right) {
			console.log("Release  autorepeat = " + event.isAutoRepeat)
			player.moveSpeed = 0
			playerTimer.running = false
			event.accepted = true
		}
	}

	Timer {
		id: moveTimer
		interval: 1000/60
		running: true
		repeat: true

		onTriggered: {

			if (player.moveSpeed > 0)
				player.step()

			ball.step()

			ball.checkCollisions()
		}
	}

	function mainLoop()
	{
		player.step()

		ball.step()

		mainLoop()
	}

	MouseArea {
		anchors.fill: parent
		hoverEnabled: true
		onMouseXChanged: {
			var newPlayerPos = mouseX - player.width/2
			player.x = newPlayerPos
		}
	}
}
