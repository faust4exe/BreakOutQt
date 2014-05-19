#ifndef MOVEABLEELASTICITEM_H
#define MOVEABLEELASTICITEM_H

#include <QObject>
#include <MoveableItem.h>
#include <PlayerElasticItem.h>

class MoveableElasticItem : public MoveableItem
{
	Q_OBJECT
public:
	explicit MoveableElasticItem(QQuickItem *parent = 0);
	virtual ~MoveableElasticItem();

	bool checkCollision(MoveableItem * item);

private:
	PlayerElasticItem m_elasticComponent;
};

#endif // MOVEABLEELASTICITEM_H
