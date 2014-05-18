#include "ElasticItem.h"

#include <MoveableItem.h>
#include <math.h>

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

	QPoint ballCenter = item->centerPoint();
	QPoint mappedCenter = mapFromItem(item->parentItem(), ballCenter).toPoint();
	ballCenter = mappedCenter;

	if (topSensor(item->width()/2).contains(ballCenter, true)) {
		item->setSpeedY(-fabs(item->speedY()));
		return true;
	}
	if (bottomSensor(item->width()/2).contains(ballCenter, true)) {
		item->setSpeedY(fabs(item->speedY()));
		return true;
	}
	if (leftSensor(item->width()/2).contains(ballCenter, true)) {
		item->setSpeedX(-fabs(item->speedX()));
		return true;
	}
	if (rightSensor(item->width()/2).contains(ballCenter, true)) {
		item->setSpeedX(fabs(item->speedX()));
		return true;
	}

	if (topLeftSensor(item->width()/2).contains(ballCenter, true)) {
		item->setSpeedX(-fabs(item->speedX()));
		item->setSpeedY(-fabs(item->speedY()));
		return true;
	}
	if (topRightSensor(item->width()/2).contains(ballCenter, true)) {
		item->setSpeedX(fabs(item->speedX()));
		item->setSpeedY(-fabs(item->speedY()));
		return true;
	}

	if (bottomLeftSensor(item->width()/2).contains(ballCenter, true)) {
		item->setSpeedX(-fabs(item->speedX()));
		item->setSpeedY(fabs(item->speedY()));
		return true;
	}
	if (bottomRightSensor(item->width()/2).contains(ballCenter, true)) {
		item->setSpeedX(fabs(item->speedX()));
		item->setSpeedY(fabs(item->speedY()));
		return true;
	}

	return false;
}

bool ElasticItem::active() const
{
	return m_active;
}

bool ElasticItem::isDeadly() const
{
	return m_isDeadly;
}

void ElasticItem::setActive(bool arg)
{
	if (m_active != arg) {
		m_active = arg;
		emit activeChanged(arg);
	}
}

void ElasticItem::setIsDeadly(bool arg)
{
	if (m_isDeadly != arg) {
		m_isDeadly = arg;
		emit isDeadlyChanged(arg);
	}
}

QRect ElasticItem::topSensor(int depth)
{
	return QRect(0,
				 0 - depth,
				 width(),
				 depth);
}

QRect ElasticItem::topLeftSensor(int depth)
{
	return QRect(0 - depth,
				 0 - depth,
				 depth,
				 depth);
}

QRect ElasticItem::topRightSensor(int depth)
{
	return QRect(0 + width(),
				 0 - depth,
				 depth,
				 depth);
}

QRect ElasticItem::bottomSensor(int depth)
{
	return QRect(0,
				 0+height(),
				 width(),
				 depth);
}

QRect ElasticItem::bottomLeftSensor(int depth)
{
	return QRect(0-depth,
				 0+height(),
				 depth,
				 depth);
}

QRect ElasticItem::bottomRightSensor(int depth)
{
	return QRect(0+width(),
				 0+height(),
				 depth,
				 depth);
}

QRect ElasticItem::leftSensor(int depth)
{
	return QRect(0-depth,
				 0,
				 depth,
				 height());
}

QRect ElasticItem::rightSensor(int depth)
{
	return QRect(0+width(),
				 0,
				 depth,
				 height());
}
