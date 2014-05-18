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
	, m_frames(0)
	, m_mediumFSP(60)
	, m_score(0)
	, m_fpsLimit(60)
{
	qDebug() << Q_FUNC_INFO;

	m_timer.setInterval(1000/m_fpsLimit);
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
	const int row = item->property("row").toInt();

	m_allItems.append(item);

	if (row == m_rowsCount - 1) {
		m_activeItems.append(item);
		item->setActive(true);
	}
}

void GameEngine::registerWall(ElasticItem *item)
{
	m_walls.append(item);
}

void GameEngine::registerBall(MoveableItem *item)
{
	m_balls.append(item);
}

void GameEngine::registerPlayer(MoveableElasticItem *item)
{
	m_player = item;
}

void GameEngine::setColumnsCount(int arg)
{
	if (m_colsCount == arg)
		return;

	m_colsCount = arg;
}

void GameEngine::setRowsCount(int arg)
{
	if (m_rowsCount == arg)
		return;

	m_rowsCount = arg;
}

void GameEngine::setFpsCount(float arg)
{
	if (m_fpsCount != arg) {
		m_fpsCount = arg;
		emit fpsCountChanged(arg);

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

void GameEngine::setFpsLimit(int arg)
{
	if (m_fpsLimit != arg) {
		m_fpsLimit = arg;
		emit fpsLimitChanged(arg);

		m_timer.setInterval(1000/m_fpsLimit);

		qDebug() << "FPS limit : " << m_fpsLimit;
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

		if (m_player->checkCollision(ball))
			continue;

		foreach (ElasticItem * wall, m_walls) {
			if (wall->checkCollision(ball)) {
				// here logic of hit
				if (wall->isDeadly()) {
					ball->setSpeedX(0);
					ball->setSpeedY(0);
					ball->setOpacity(0.1);
				}
				continue;
			}
		}
	}

	foreach (MoveableItem * ball, m_balls) {
		foreach (ElasticItem * item, m_activeItems) {
			const int row = item->property("row").toInt();
			const int col = item->property("column").toInt();

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
	, m_active(false)
	, m_isDeadly(false)
{

}

ElasticItem::~ElasticItem()
{

}

bool ElasticItem::checkCollision(MoveableItem *item)
{
	if (opacity() == 0)
		return false;

	QRect r = QRect(0, 0,
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
		item->setSpeedY(-fabs(item->speedY()));
		return true;
	}
	if (bottomZone.contains(ballCenter, true)) {
		item->setSpeedY(fabs(item->speedY()));
		return true;
	}
	if (leftZone.contains(ballCenter, true)) {
		item->setSpeedX(-fabs(item->speedX()));
		return true;
	}
	if (rightZone.contains(ballCenter, true)) {
//		qDebug() << "ball on right";
		item->setSpeedX(fabs(item->speedX()));
		return true;
	}

	return false;
}

////////////////////////////////////////////

MoveableElasticItem::MoveableElasticItem(QQuickItem *parent)
	: MoveableItem(parent)
{

}

MoveableElasticItem::~MoveableElasticItem()
{

}

bool MoveableElasticItem::checkCollision(MoveableItem *item)
{
	m_elasticComponent.setX(x());
	m_elasticComponent.setY(y());
	m_elasticComponent.setWidth(width());
	m_elasticComponent.setHeight(height());

	return m_elasticComponent.checkCollision(item);
}
