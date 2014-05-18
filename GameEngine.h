#ifndef GAMEENGINE_H
#define GAMEENGINE_H

#include <QObject>
#include <QTime>
#include <QTimer>
#include <QQuickItem>

class MoveableItem;
class ElasticItem;
class MoveableElasticItem;

class GameEngine : public QObject
{
	Q_OBJECT

	Q_PROPERTY(int mediumFSP READ mediumFSP WRITE setMediumFSP NOTIFY mediumFSPChanged)
	Q_PROPERTY(int frames READ frames WRITE setFrames NOTIFY framesChanged)
	Q_PROPERTY(float fpsCount READ fpsCount WRITE setFpsCount NOTIFY fpsCountChanged)
	Q_PROPERTY(bool running READ running WRITE setRunning NOTIFY runningChanged)
	Q_PROPERTY(int score READ score WRITE setScore NOTIFY scoreChanged)
	Q_PROPERTY(int fpsLimit READ fpsLimit WRITE setFpsLimit NOTIFY fpsLimitChanged)
public:
	explicit GameEngine(QObject *parent);
	virtual ~GameEngine();

	float fpsCount() const;

	bool running() const;

	Q_INVOKABLE void start(bool forse = false);
	Q_INVOKABLE void stop(bool forse = false);
	Q_INVOKABLE void restartGame();

	Q_INVOKABLE void registerItem(ElasticItem * item);

	Q_INVOKABLE void registerWall(ElasticItem * item);

	Q_INVOKABLE void registerBall(MoveableItem * item);

	Q_INVOKABLE void registerPlayer(MoveableElasticItem * item);

	Q_INVOKABLE void setColumnsCount(int arg);
	Q_INVOKABLE void setRowsCount(int arg);

	int frames() const
	{
		return m_frames;
	}

	int mediumFSP() const
	{
		return m_mediumFSP;
	}

	int score() const
	{
		return m_score;
	}

	int fpsLimit() const
	{
		return m_fpsLimit;
	}

public slots:
	void setFpsCount(float arg);

	void setRunning(bool arg);

	void setFrames(int arg)
	{
		if (m_frames != arg) {
			m_frames = arg;
			emit framesChanged(arg);

		}
	}

	void setMediumFSP(int arg)
	{
		if (m_mediumFSP != arg) {
			m_mediumFSP = arg;
			emit mediumFSPChanged(arg);
		}
	}

	void setScore(int arg)
	{
		if (m_score != arg) {
			m_score = arg;
			emit scoreChanged(arg);
		}
	}

	void setFpsLimit(int arg);

signals:
	void fpsCountChanged(float arg);

	void runningChanged(bool arg);

	void framesChanged(int arg);

	void mediumFSPChanged(int arg);

	void scoreChanged(int arg);

	void fpsLimitChanged(int arg);

private slots:
	void onTimer();

private:
	int calculateFPS();

	void moveObjects(int msecs);

	void checkCollisions();

	void resetActiveItems();

	void onItemHit();

	ElasticItem *itemAt(int row, int col);

private:
	QTime m_lastFrameTime;
	QTime m_mediumFPSTime;
	float m_fpsCount;
	bool  m_running;
	QTimer m_timer;

	int m_rowsCount;
	int m_colsCount;

	QList<ElasticItem *> m_allItems;
	QList<ElasticItem *> m_activeItems;

	QList<ElasticItem *> m_walls;

	QList<MoveableItem *> m_balls;
	MoveableElasticItem * m_player;

	int m_frames;
	int m_mediumFSP;
	int m_score;
	int m_fpsLimit;
};

#endif // GAMEENGINE_H
