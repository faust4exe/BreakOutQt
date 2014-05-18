#ifndef PLAYERELASTICITEM_H
#define PLAYERELASTICITEM_H

#include <QObject>
#include <ElasticItem.h>

class PlayerElasticItem : public ElasticItem
{
	Q_OBJECT
public:
	explicit PlayerElasticItem(QQuickItem *parent = 0);
	virtual ~PlayerElasticItem();

	bool checkCollision(MoveableItem * item);
};

#endif // PLAYERELASTICITEM_H
