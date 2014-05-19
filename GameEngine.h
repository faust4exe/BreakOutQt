#ifndef GAMEENGINE_H
#define GAMEENGINE_H

#include <QObject>
#include <QTime>
#include <QTimer>
#include <QQuickItem>

class BonusItem;
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
	Q_PROPERTY(int lifesCounter READ lifesCounter WRITE setLifesCounter NOTIFY lifesCounterChanged)
	Q_PROPERTY(bool godMode READ godMode WRITE setGodMode NOTIFY godModeChanged)
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
	Q_INVOKABLE void registerBonus(BonusItem * item);
	Q_INVOKABLE void registerPlayer(MoveableElasticItem * item);

	Q_INVOKABLE void setColumnsCount(int arg);
	Q_INVOKABLE void setRowsCount(int arg);

	int frames() const;
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
	int lifesCounter() const
	{
		return m_lifesCounter;
	}

	bool godMode() const
	{
		return m_godMode;
	}

public slots:
	void setFpsCount(float arg);
	void setRunning(bool arg);
	void setFrames(int arg);
	void setMediumFSP(int arg);

	void setScore(int arg);
	void setFpsLimit(int arg);
	void setLifesCounter(int arg);

	void setGodMode(bool arg)
	{
		if (m_godMode != arg) {
			m_godMode = arg;
			emit godModeChanged(arg);
		}
	}

signals:
	void fpsCountChanged(float arg);
	void runningChanged(bool arg);
	void framesChanged(int arg);
	void mediumFSPChanged(int arg);
	void scoreChanged(int arg);
	void fpsLimitChanged(int arg);
	void lifesCounterChanged(int arg);

	void youWin();
	void gameOver();
	void ballLosed();

	void godModeChanged(bool arg);

private slots:
	void onTimer();

private:
	int calculateFPS();

	void moveObjects(int msecs);

	void checkCollisions();

	void resetActiveItems();

	void onItemHit(ElasticItem * item);

	void applyBonus(BonusItem *bonusItem);

	MoveableItem * getActiveBall();

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

	QList<BonusItem *> m_bonus;
	QList<MoveableItem *> m_balls;
	MoveableElasticItem * m_player;

	int m_frames;
	int m_mediumFSP;
	int m_score;
	int m_fpsLimit;
	int m_lifesCounter;
	bool m_godMode;
};

#endif // GAMEENGINE_H
