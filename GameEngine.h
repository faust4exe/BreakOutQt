#ifndef GAMEENGINE_H
#define GAMEENGINE_H

#include <QObject>
#include <QTime>
#include <QTimer>
#include <QQuickItem>

class GameEngine : public QObject
{
	Q_OBJECT

	Q_PROPERTY(float fpsCount READ fpsCount WRITE setFpsCount NOTIFY fpsCountChanged)
	Q_PROPERTY(bool running READ running WRITE setRunning NOTIFY runningChanged)
public:
	explicit GameEngine(QObject *parent);
	virtual ~GameEngine();

	float fpsCount() const;

	bool running() const;

	Q_INVOKABLE void start();
	Q_INVOKABLE void stop();
	Q_INVOKABLE void registerItem(QQuickItem * item);

public slots:
	void setFpsCount(float arg);

	void setRunning(bool arg);

signals:
	void fpsCountChanged(float arg);

	void runningChanged(bool arg);

private slots:
	void onTimer();

private:
	void mainLoop();

	void calculateFPS();

	void moveObjects();

	void checkCollisions();

private:
	QTime m_lastFrameTime;
	float m_fpsCount;
	bool  m_running;
	QTimer m_timer;
};

#endif // GAMEENGINE_H
