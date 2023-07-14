#include "PlayerElasticItem.h"

#include <MoveableItem.h>
#include <math.h>

PlayerElasticItem::PlayerElasticItem(QQuickItem *parent)
	: ElasticItem(parent)
{
}

PlayerElasticItem::~PlayerElasticItem()
{

}

bool PlayerElasticItem::checkCollision(MoveableItem *item)
{
	if (opacity() == 0)
		return false;

	QPoint ballCenter = item->centerPoint();
	int depth = item->width()/2;
    QRect detector(-depth + x(),
                   -depth + y(),
				   width() + depth,
				   height() + depth);

	if (detector.contains(ballCenter, true)) {
        double posX = x() + width()/2.0;
        double posY = y() + height()/2.0;

		double xdiff = ballCenter.x() - posX;
		double ydiff = posY - ballCenter.y();

		// get angle and make correction
		double theAngle = atan(fabs(ydiff) / fabs(xdiff));
		double theAngleGrage = theAngle * 180.0 / 3.1415;
		if (ydiff < 0)
			theAngleGrage = -theAngleGrage;
		if (xdiff < 0)
			theAngleGrage = 180.0 - theAngleGrage;

		double theSpeed = sqrt(pow(item->speedX(), 2)
							 + pow(item->speedY(), 2));

		item->setSpeedX(cos(theAngleGrage/180.0*3.1415) * theSpeed);
		item->setSpeedY(-sin(theAngleGrage/180.0*3.1415) * theSpeed);

		return true;
	}

	return false;
}
