#include "BonusItem.h"

BonusItem::BonusItem(QQuickItem *parent) :
	MoveableItem(parent)
  , m_bonus(-1)
{
	randomize();
}

BonusItem::~BonusItem()
{

}

void BonusItem::randomize()
{
	int type = qrand() % BonusCount;
	setBonus(type);
}

QString BonusItem::text() const
{
	switch (m_bonus) {
	case BonusItem::BiggerPlayer:
		return "<>";
	case BonusItem::SlowerBall:
		return "Slow";
	case BonusItem::ExtraLife:
		return "+1";
	case BonusItem::MegaBall:
		return "x3";
	case BonusItem::SmallerPlayer:
		return "><";
	case BonusItem::FasterBall:
		return "Fast";
	case BonusItem::MinusLife:
		return "-1";
	}
	return "";
}

void BonusItem::setBonus(int arg)
{
	if (m_bonus != arg) {
		m_bonus = arg;
		emit bonusChanged(arg);
		emit textChanged(text());
	}
}
