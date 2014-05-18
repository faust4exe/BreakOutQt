#ifndef MOVEABLEITEM_H
#define MOVEABLEITEM_H

#include <QQuickItem>

class MoveableItem : public QQuickItem
{
	Q_OBJECT

	Q_PROPERTY(double speedX READ speedX WRITE setSpeedX NOTIFY speedXChanged)
	Q_PROPERTY(double speedY READ speedY WRITE setSpeedY NOTIFY speedYChanged)

public:
	explicit MoveableItem(QQuickItem *parent = 0);
	virtual ~MoveableItem();

	void step(double msecs = 0);

	double speedX() const;
	double speedY() const;

	QPoint centerPoint() const;

public slots:
	void setSpeedX(double arg);
	void setSpeedY(double arg);

signals:
	void speedXChanged(double arg);
	void speedYChanged(double arg);

private:
	double m_speedX;
	double m_speedY;
};

#endif // MOVEABLEITEM_H
