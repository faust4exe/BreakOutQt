#ifndef BONUSITEM_H
#define BONUSITEM_H

#include <QQuickItem>
#include <MoveableItem.h>

class BonusItem : public MoveableItem
{
	Q_OBJECT
	Q_PROPERTY(int bonus READ bonus WRITE setBonus NOTIFY bonusChanged)
	Q_PROPERTY(QString text READ text NOTIFY textChanged)

public:
	enum BonusType {
		BiggerPlayer = 0,
		SlowerBall,
		ExtraLife,
		MegaBall, // 3

		SmallerPlayer,
		FasterBall,
		MinusLife,
		BonusCount
	};

	explicit BonusItem(QQuickItem *parent = 0);
	virtual ~BonusItem();

	int bonus() const
	{
		return m_bonus;
	}

	Q_INVOKABLE void randomize();

	QString text() const;

signals:
	void bonusChanged(int arg);

	void textChanged(QString arg);

public slots:
	void setBonus(int arg);

private:
	int m_bonus;
	QString m_text;
};

#endif // BONUSITEM_H
