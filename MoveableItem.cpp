#include "MoveableItem.h"


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
