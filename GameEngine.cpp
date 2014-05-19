#include "GameEngine.h"

#include <QDebug>
#include <BonusItem.h>
#include <ElasticItem.h>
#include <MoveableItem.h>
#include <MoveableElasticItem.h>
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
	, m_lifesCounter(3)
{
	m_timer.setInterval(1000/m_fpsLimit);
	m_timer.setSingleShot(false);
	connect(&m_timer, SIGNAL(timeout()),
			SLOT(onTimer()));
}

GameEngine::~GameEngine()
{
}

float GameEngine::fpsCount() const
{
	return m_fpsCount;
}

bool GameEngine::running() const
{
	return m_running;
}

void GameEngine::start(bool forse)
{
	setLifesCounter(lifesCounter()-1);

	if (forse)
		setRunning(false);
	setRunning(true);
}

void GameEngine::stop(bool forse)
{
	if (forse)
		setRunning(true);
	setRunning(false);
}

void GameEngine::restartGame()
{
	foreach (ElasticItem * item, m_allItems) {
		item->setOpacity(1.0);
		item->setActive(false);
	}

	resetActiveItems();

	setLifesCounter(3);
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

void GameEngine::registerBonus(BonusItem *item)
{
	m_bonus.append(item);
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
	QTime start = QTime::currentTime();
	int msecs = calculateFPS();

	if (msecs > 1000) {
		qDebug() << "Too many msecs";
		return;
	}

	moveObjects(msecs);

	checkCollisions();
	qDebug() << "frame in " << start.msecsTo(QTime::currentTime());
}

int GameEngine::calculateFPS()
{
	setFrames(m_frames+1);

	if (frames() % 100 == 0) {
		setFrames(0);
		int frames_msecs = m_mediumFPSTime.msecsTo(QTime::currentTime());
		frames_msecs = qMax(frames_msecs, 1);
		m_mediumFPSTime = QTime::currentTime();
		setMediumFSP(1000.0/frames_msecs*100);
	}

	int timeFrame_msecs = m_lastFrameTime.msecsTo(QTime::currentTime());

	timeFrame_msecs = qMax(timeFrame_msecs, 1);

	m_lastFrameTime = QTime::currentTime();

	setFpsCount(1000/timeFrame_msecs);

	return timeFrame_msecs;
}

void GameEngine::moveObjects(int msecs)
{
	// distance = timeFrame * speed
	if (m_player)
		m_player->step(msecs);

	foreach (MoveableItem *bonus, m_bonus) {
		bonus->step(msecs);
	}

	foreach (MoveableItem *ball, m_balls) {
		ball->step(msecs);
	}
}

void GameEngine::checkCollisions()
{
	m_player->setX(qMax(0, (int)m_player->x()));
	m_player->setX(qMin((int)m_player->parentItem()->width() - (int)m_player->width(),
						(int)m_player->x()));

	// collision on balls with player and walls
	foreach (MoveableItem * ball, m_balls) {
		if (ball->opacity() == 0)
			continue;

		if (m_player->checkCollision(ball))
			continue;

		foreach (ElasticItem * wall, m_walls) {
			if (wall->checkCollision(ball)) {
				// ball hitting deadly wall dies
				if (wall->isDeadly()) {
					ball->setSpeedX(0);
					ball->setSpeedY(0);
					ball->setOpacity(0);

					if (lifesCounter() == 0)
						qDebug() << "GAME OVER";
				}
				break;
			}
		}
	}

	// collisions of bonus with player and walls
	foreach (BonusItem *bonus, m_bonus) {
		if (bonus->opacity() == 0)
			continue;

		if (m_player->checkCollision(bonus)) {
			bonus->setSpeedX(0);
			bonus->setSpeedY(0);
			bonus->setOpacity(0.0);
			applyBonus(bonus);
			continue;
		}

		foreach (ElasticItem * wall, m_walls) {
			if (wall->checkCollision(bonus)) {
				// ball hitting deadly wall dies
				if (wall->isDeadly()) {
					bonus->setSpeedX(0);
					bonus->setSpeedY(0);
					bonus->setOpacity(0.0);
				}
				break;
			}
		}
	}

	foreach (MoveableItem * ball, m_balls) {
		if (ball->opacity() == 0)
			continue;

		foreach (ElasticItem * item, m_activeItems) {
			if (item->opacity() == 0)
				continue;

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
					if (!newActive->active())
						m_activeItems.append(newActive);
					newActive->setActive(true);
				}
				newActive = itemAt(row-1, col);
				if (newActive != 0){
					if (!newActive->active())
						m_activeItems.append(newActive);
					newActive->setActive(true);
				}
				newActive = itemAt(row, col+1);
				if (newActive != 0){
					if (!newActive->active())
						m_activeItems.append(newActive);
					newActive->setActive(true);
				}
				newActive = itemAt(row, col-1);
				if (newActive != 0){
					if (!newActive->active())
						m_activeItems.append(newActive);
					newActive->setActive(true);
				}

				onItemHit(item);
				break;
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
		item->setActive(true);
	}
}

void GameEngine::onItemHit(ElasticItem *item)
{
	// score ++
	setScore(score()+1);

	// check end of game
	if (m_activeItems.count() == 0) {
		qDebug() << "GAME WIN";
	}

	// increase speed a little
	foreach (MoveableItem * ball, m_balls) {
		ball->setSpeedX(ball->speedX()*1.01);
		ball->setSpeedY(ball->speedY()*1.01);
	}

	// check if bonus accessible
	foreach (BonusItem * bonusItem, m_bonus) {
		if (bonusItem->opacity() == 0) {

			if (qrand() % 100 < 50) {
				bonusItem->setX(item->x()+item->width()/2);
				bonusItem->setY(item->y()+item->height()/2);
				bonusItem->randomize();
				bonusItem->setOpacity(1.0);
				bonusItem->setSpeedY(75);
			}
		}
	}
}

void GameEngine::applyBonus(BonusItem *bonusItem)
{
	switch (bonusItem->bonus()) {
	case BonusItem::BiggerPlayer:
		m_player->setWidth(m_player->width()*1.5);
		break;
	case BonusItem::SlowerBall:
		foreach (MoveableItem * ball, m_balls) {
			ball->setSpeedX(ball->speedX()*0.75);
			ball->setSpeedY(ball->speedY()*0.75);
		}
		break;
	case BonusItem::ExtraLife:
		setLifesCounter(lifesCounter()+1);
		break;
	case BonusItem::MegaBall: {
		MoveableItem * genericBall = getActiveBall();
		if (genericBall == 0)
			return;

		int counter = 0;
		double theSpeed = sqrt(pow(genericBall->speedX(), 2)
							 + pow(genericBall->speedY(), 2));
		foreach (MoveableItem * ball, m_balls) {
			if (ball != genericBall) {
				ball->setX(genericBall->x());
				ball->setY(genericBall->y());
			}

			ball->setSpeedX(theSpeed * sin(3.1415*2*counter/m_balls.count()));
			ball->setSpeedY(theSpeed * cos(3.1415*2*counter/m_balls.count()));
			ball->setOpacity(1.0);
			counter++;
		}
		break;
	}
	case BonusItem::SmallerPlayer:
		m_player->setWidth(m_player->width()*0.75);
		break;
	case BonusItem::FasterBall:
		foreach (MoveableItem * ball, m_balls) {
			ball->setSpeedX(ball->speedX()*1.25);
			ball->setSpeedY(ball->speedY()*1.25);
		}
		break;
	case BonusItem::MinusLife:
		setLifesCounter(qMax(0,lifesCounter()-1));
		break;
	}
}

MoveableItem *GameEngine::getActiveBall()
{
	foreach (MoveableItem * ball, m_balls) {
		if (ball->opacity() == 1.0)
			return ball;
	}
	return 0;
}

ElasticItem *GameEngine::itemAt(int row, int col)
{
	if (row < 0 || row >= m_rowsCount )
		return 0;

	if (col < 0 || col >= m_colsCount )
		return 0;

	int index = col + row * m_colsCount;
	if (m_allItems.count() > index) {
		ElasticItem * item = m_allItems.at(index);
		if (item->opacity() == 0)
			return 0;
		return item;
	} else
		return 0;
}

