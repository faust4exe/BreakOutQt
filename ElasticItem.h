#ifndef ELASTICITEM_H
#define ELASTICITEM_H

#include <QObject>
#include <QQuickItem>

class MoveableItem;

class ElasticItem : public QQuickItem
{
	Q_OBJECT

	Q_PROPERTY(bool active READ active WRITE setActive NOTIFY activeChanged)
	Q_PROPERTY(bool isDeadly READ isDeadly WRITE setIsDeadly NOTIFY isDeadlyChanged)

public:
	explicit ElasticItem(QQuickItem *parent = 0);
	virtual ~ElasticItem();

	virtual bool checkCollision(MoveableItem * item);

	bool active() const;
	bool isDeadly() const;

public slots:
	void setActive(bool arg);
	void setIsDeadly(bool arg);

signals:
	void activeChanged(bool arg);
	void isDeadlyChanged(bool arg);

protected:
	QRect topSensor(int depth);
	QRect topLeftSensor(int depth);
	QRect topRightSensor(int depth);
	QRect bottomSensor(int depth);
	QRect bottomLeftSensor(int depth);
	QRect bottomRightSensor(int depth);
	QRect leftSensor(int depth);
	QRect rightSensor(int depth);

private:
	bool m_active;
	bool m_isDeadly;
};

#endif // ELASTICITEM_H
