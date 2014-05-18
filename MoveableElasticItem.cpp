#include "MoveableElasticItem.h"

MoveableElasticItem::MoveableElasticItem(QQuickItem *parent)
	: MoveableItem(parent)
{

}

MoveableElasticItem::~MoveableElasticItem()
{

}

bool MoveableElasticItem::checkCollision(MoveableItem *item)
{
	if (opacity() == 0)
		return false;

	m_elasticComponent.setX(x());
	m_elasticComponent.setY(y());
	m_elasticComponent.setWidth(width());
	m_elasticComponent.setHeight(height());

	return m_elasticComponent.checkCollision(item);
}
