#include "GameEngine.h"

#include <QDebug>

GameEngine::GameEngine(QObject *parent)
	: QObject(parent)
	, m_lastFrameTime(QTime::currentTime())
	, m_fpsCount(0)
	, m_running(false)
{
	qDebug() << Q_FUNC_INFO;

	m_timer.setInterval(1000/60);
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

void GameEngine::registerItem(QQuickItem *item)
{
	qDebug() << "-----------------------";
	qDebug() << "Got Item";
	qDebug() << "objectName : " << item->objectName();
	qDebug() << "height : " << item->height();
	qDebug() << "width : " << item->width();
	qDebug() << "row : " << item->property("row");
	qDebug() << "column : " << item->property("column");
	qDebug() << "color : " << item->property("color");
}

void GameEngine::setFpsCount(float arg)
{
	if (m_fpsCount != arg) {
		m_fpsCount = arg;
		emit fpsCountChanged(arg);

		qDebug() << "fps = " << arg;
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
	calculateFPS();

	moveObjects();

	checkCollisions();
}

void GameEngine::mainLoop()
{
	while (m_running) {
		calculateFPS();


		qDebug() << Q_FUNC_INFO;
	}
}

void GameEngine::calculateFPS()
{
	int timeFrame_msecs = m_lastFrameTime.msecsTo(QTime::currentTime());
	m_lastFrameTime = QTime::currentTime();
	setFpsCount(1000/timeFrame_msecs);
}

void GameEngine::moveObjects()
{
	// distance = timeFrame * speed
}

void GameEngine::checkCollisions()
{

}
