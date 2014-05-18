#include "GameEngine.h"

#include <QDebug>

#include <math.h>

GameEngine::GameEngine(QObject *parent)
	: QObject(parent)
	, m_lastFrameTime(QTime::currentTime())
	, m_mediumFPSTime(QTime::currentTime())
	, m_fpsCount(0)
	, m_running(false)
	, m_rowsCount(0)
	, m_colsCount(0)
	, m_player(0)
//	, m_playfield(0)
	, m_frames(0)
	, m_mediumFSP(60)
	, m_score(0)
{
	qDebug() << Q_FUNC_INFO;

	m_timer.setInterval(1000/m_mediumFSP);
	m_timer.setSingleShot(false);
	connect(&m_timer, SIGNAL(timeout()),
			SLOT(onTimer()));
}

GameEngine::~GameEngine()
{
	qDebug() << Q_FUNC_INFO;
}

float GameEngine::fpsCount() const
{
	return m_fpsCount;
}

bool GameEngine::running() const
{
	return m_running;
}

void GameEngine::start()
{
	setRunning(true);
}

void GameEngine::stop()
{
	setRunning(false);
}

void GameEngine::registerItem(ElasticItem *item)
{
//	qDebug() << "-----------------------";
//	qDebug() << "Got Item";
//	qDebug() << "height : " << item->height();
//	qDebug() << "width : " << item->width();

//	if (m_playfield)
//		item->setParentItem(m_playfield);

	const int row = item->property("row").toInt();
	const int col = item->property("column").toInt();
//	qDebug() << "pos (r,c) : " << row << "," << col;

	m_allItems.append(item);
	if (row == m_rowsCount - 1) {
		m_activeItems.append(item);
		item->setActive(true);
//		qDebug() << "ACTIVE Item pos (r,c) : " << row << "," << col;
	}
}

void GameEngine::registerWall(ElasticItem *item)
{
	m_walls.append(item);
}

void GameEngine::registerBall(MoveableItem *item)
{
	int xv = item->x();
	int yv = item->y();
	m_balls.append(item);
}

void GameEngine::registerPlayer(MoveableElasticItem *item)
{
	m_player = item;
}

//void GameEngine::registerPlayfield(QQuickItem *item)
//{
//	m_playfield = item;

//	foreach (ElasticItem * item, m_allItems) {
//		QPointF oldPos = item->mapToScene(item->position());
//		QPointF newPosition = m_playfield->mapFromScene(oldPos);

////		item->setParentItem(m_playfield);
////		item->setPosition(newPosition);
//	}
//}

//void GameEngine::registerPlayer(ElasticItem *item)
//{
//	m_playerElastic = item;
//}

void GameEngine::setColumnsCount(int arg)
{
	if (m_colsCount == arg)
		return;

	qDebug() << "new cols count = " << arg;

	m_colsCount = arg;

//	if (m_colsCount > 0 && m_rowsCount > 0)
//		resetActiveItems();
}

void GameEngine::setRowsCount(int arg)
{
	if (m_rowsCount == arg)
		return;

	qDebug() << "new rowss count = " << arg;

	m_rowsCount = arg;

//	if (m_colsCount > 0 && m_rowsCount > 0)
//		resetActiveItems();
}

void GameEngine::setFpsCount(float arg)
{
	if (m_fpsCount != arg) {
		m_fpsCount = arg;
		emit fpsCountChanged(arg);

//		qDebug() << "fps = " << arg;
	}
}

void GameEngine::setRunning(bool arg)
{
	if (m_running != arg) {
		m_running = arg;
		emit runningChanged(arg);

		if (m_running)
			m_timer.start();
		else
			m_timer.stop();
	}
}

void GameEngine::onTimer()
{
	int msecs = calculateFPS();

	if (msecs > 1000) {
		qDebug() << "Too many msecs";
		return;
	}

	moveObjects(msecs);

	checkCollisions();
}

void GameEngine::mainLoop()
{
	while (m_running) {
		calculateFPS();


		qDebug() << Q_FUNC_INFO;
	}
}

int GameEngine::calculateFPS()
{
	setFrames(m_frames+1);

	if (frames() % 100 == 0) {
		setFrames(0);
		int frames_msecs = m_mediumFPSTime.msecsTo(QTime::currentTime());
		m_mediumFPSTime = QTime::currentTime();
		setMediumFSP(1000.0/frames_msecs*100);
	}
	int timeFrame_msecs = m_lastFrameTime.msecsTo(QTime::currentTime());
	m_lastFrameTime = QTime::currentTime();
	setFpsCount(1000/timeFrame_msecs);

	return timeFrame_msecs;
}

void GameEngine::moveObjects(int msecs)
{
	// distance = timeFrame * speed
	if (m_player)
		m_player->step(msecs);

	foreach (MoveableItem *ball, m_balls) {
		ball->step(msecs);
	}
}

void GameEngine::checkCollisions()
{
	foreach (MoveableItem * ball, m_balls) {
//		QPoint p = ball->centerPoint();


//		QRect pr = QRect(m_player->x(),
//						 m_player->y(),
//						 m_player->width(),
//						 m_player->height());


//		if (pr.adjusted(-ball->width()/2,
//					   -ball->width()/2,
//					   ball->width()/2,
//					   ball->width()/2).contains(p, true)) {
//			qDebug() << "ball on Player ";
			m_player->checkCollision(ball);
//		}// else
//			qDebug() << "p: " << p << " playerr: " << pr ;

		foreach (ElasticItem * wall, m_walls) {


//			QRect r = QRect(wall->x(), wall->y(),
//							wall->width(),
//							wall->height());

			/*QRect topZone = r.adjusted(0,
									   -ball->width()/2,
									   0,
									   -ball->width()/2);

			QRect bottomZone = r.adjusted(0,
										  ball->width()/2,
										  0,
										  ball->width()/2);

			QRect leftZone = r.adjusted(-ball->width()/2,
										0,
										-ball->width()/2,
										0);

			QRect rightZone = r.adjusted(ball->width()/2,
										 0,
										 ball->width()/2,
										 0);*/

//			qDebug() << "p: " << p << " r: " << r ;
//			if (r.adjusted(-ball->width()/2,
//						   -ball->width()/2,
//						   ball->width()/2,
//						   ball->width()/2).contains(p, true)) {
//				qDebug() << "ball on wall ";
				if (wall->checkCollision(ball)) {
//					if (wall->metaObject()->indexOfMethod("onHit") > -1)
						QMetaObject::invokeMethod(wall,
											  "onHit");
				}

//			} //else
//				qDebug() << "p: " << p << " r: " << r ;
			/*if (topZone.contains(p, true)) {
				qDebug() << "ball on TOP ";
				ball->setSpeedY(-fabs(ball->speedY()));
			}
			if (bottomZone.contains(p, true)) {
				qDebug() << "ball on bottom ";
				ball->setSpeedY(fabs(ball->speedY()));
			}
			if (leftZone.contains(p, true)) {
				qDebug() << "ball on left";
				ball->setSpeedX(-fabs(ball->speedX()));
			}
			if (rightZone.contains(p, true)) {
				qDebug() << "ball on right";
				ball->setSpeedX(fabs(ball->speedX()));
			}*/
		}
	}

	foreach (MoveableItem * ball, m_balls) {
		foreach (ElasticItem * item, m_activeItems) {
			const int row = item->property("row").toInt();
			const int col = item->property("column").toInt();

//			QPoint p = ball->centerPoint();
//			QRect r = QRect(item->x(),
//							item->y(),
//							item->width(),
//							item->height());

//			QRect mr = item->mapRectToItem(m_playfield, QRectF(r)).toRect();

//			if (r.adjusted(-ball->width()/2,
//						   -ball->width()/2,
//						   ball->width()/2,
//						   ball->width()/2).contains(p, true)) {
//				qDebug() << "ball on ITEM ";
				if (item->checkCollision(ball)) {

					// hide current
					item->setOpacity(0.0);

					// remove from active
					m_activeItems.removeAll(item);

					// add neighbors to list
					ElasticItem * newActive = 0;
					newActive = itemAt(row+1, col);
					if (newActive != 0){
						m_activeItems.append(newActive);
						newActive->setActive(true);
					}
					newActive = itemAt(row-1, col);
					if (newActive != 0){
						m_activeItems.append(newActive);
						newActive->setActive(true);
					}
					newActive = itemAt(row, col+1);
					if (newActive != 0){
						m_activeItems.append(newActive);
						newActive->setActive(true);
					}
					newActive = itemAt(row, col-1);
					if (newActive != 0){
						m_activeItems.append(newActive);
						newActive->setActive(true);
					}

					onItemHit();
				}

//			} else {
//				if (mr.adjusted(-ball->width()/2,
//							   -ball->width()/2,
//							   ball->width()/2,
//							   ball->width()/2).contains(p, true)) {
////					qDebug() << "ball on Mapped Item ";
////					item->checkCollision(ball);
//				}
//			}
//			qDebug() << "p: " << p << " r: " << r ;
		}
	}
}

void GameEngine::resetActiveItems()
{
	m_activeItems.clear();

	for (int col = 0; col < m_colsCount; col++) {
		int index = col + (m_rowsCount-1) * m_colsCount;
		ElasticItem * item = m_allItems.at(index);
		m_activeItems.append(item);
//		qDebug() << "ACTIVE Item pos (r,c) : " << item->property("row").toInt() << "," << item->property("column").toInt();
	}
}

void GameEngine::onItemHit()
{
	// score ++
	setScore(score()+1);

	// check end of game
	if (m_activeItems.count() == 0) {
		qDebug() << "GAME WIN";
	}

	foreach (MoveableItem * ball, m_balls) {
		ball->setSpeedX(ball->speedX()*1.01);
		ball->setSpeedY(ball->speedY()*1.01);
	}

}

ElasticItem *GameEngine::itemAt(int row, int col)
{
	if (row < 0 || row >= m_rowsCount )
		return 0;

	if (col < 0 || col >= m_colsCount )
		return 0;

	int index = col + row * m_colsCount;
	if (m_allItems.count() > index)
		return m_allItems.at(index);
	else
		return 0;
}

////////////////////////////////////////////////////////////

MoveableItem::MoveableItem(QQuickItem *parent)
	: QQuickItem(parent)
	, m_speedX(0)
	, m_speedY(0)
{

}

MoveableItem::~MoveableItem()
{

}

void MoveableItem::step(double msecs)
{
	qreal newX = speedX() * msecs/1000.0 + x();
	qreal newY = speedY() * msecs/1000.0 + y();

	setX(newX);
	setY(newY);
}

double MoveableItem::speedX() const
{
	return m_speedX;
}

double MoveableItem::speedY() const
{
	return m_speedY;
}

QPoint MoveableItem::centerPoint() const
{
	int xv = x();
	int yv = y();
	int wv = width();
	int hv = height();
	return QPoint(x() + width()/2,
				  y() + height()/2);
}

void MoveableItem::setSpeedX(double arg)
{
	if (m_speedX != arg) {
		m_speedX = arg;
		emit speedXChanged(arg);
	}
}

void MoveableItem::setSpeedY(double arg)
{
	if (m_speedY != arg) {
		m_speedY = arg;
		emit speedYChanged(arg);
	}
}

//////////////////////////////////////////////////////////

ElasticItem::ElasticItem(QQuickItem *parent)
	: QQuickItem(parent)
	, m_offsetX(0)
	, m_offsetY(0)
	, m_active(false)
{

}

ElasticItem::~ElasticItem()
{

}

bool ElasticItem::checkCollision(MoveableItem *item)
{
	if (opacity() == 0)
		return false;

	QRect r = QRect(0, //x() + offsetX(),
					0, //y() + offsetY(),
					width(),
					height());

	QRect topZone = r.adjusted(0,
							   -item->width()/2,
							   0,
							   -item->width()/2);

	QRect bottomZone = r.adjusted(0,
								  item->width()/2,
								  0,
								  item->width()/2);

	QRect leftZone = r.adjusted(-item->width()/2,
								0,
								-item->width()/2,
								0);

	QRect rightZone = r.adjusted(item->width()/2,
								 0,
								 item->width()/2,
								 0);

	QPoint ballCenter = item->centerPoint();
	QPoint mappedCenter = mapFromItem(item->parentItem(), ballCenter).toPoint();
	ballCenter = mappedCenter;

	if (topZone.contains(ballCenter, true)) {
//		qDebug() << "ball on TOP ";
		item->setSpeedY(-fabs(item->speedY()));
//		QMetaObject::invokeMethod(this, "test");
		return true;
	}
	if (bottomZone.contains(ballCenter, true)) {
//		qDebug() << "ball on bottom ";
		item->setSpeedY(fabs(item->speedY()));
//		QMetaObject::invokeMethod(this, "test");
		return true;
	}
	if (leftZone.contains(ballCenter, true)) {
//		qDebug() << "ball on left";
		item->setSpeedX(-fabs(item->speedX()));
//		QMetaObject::invokeMethod(this, "test");
		return true;
	}
	if (rightZone.contains(ballCenter, true)) {
//		qDebug() << "ball on right";
		item->setSpeedX(fabs(item->speedX()));
//		QMetaObject::invokeMethod(this, "test");
		return true;
	}

	return false;


//	metaObject()->invokeMethod("test");
}

////////////////////////////////////////////

MoveableElasticItem::MoveableElasticItem(QQuickItem *parent)
	: MoveableItem(parent)
{

}

MoveableElasticItem::~MoveableElasticItem()
{

}

void MoveableElasticItem::checkCollision(MoveableItem *item)
{
	m_elasticComponent.setX(x());
	m_elasticComponent.setY(y());
	m_elasticComponent.setWidth(width());
	m_elasticComponent.setHeight(height());

	m_elasticComponent.checkCollision(item);
}
