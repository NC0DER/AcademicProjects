#ifndef TRADE_H
#define TRADE_H

#include "ship.h"
class Trade : public Ship
{
	public:
		Trade():Ship("Trade Ship")
		{
			maxHP = 3 * CUSTOM_HP;
			HP = 3 * CUSTOM_HP;
			type = "Trade";
		}
		Trade(std::string name):Ship(name)
		{
			this->name = name;
			maxHP = 3 * CUSTOM_HP;
			HP = 3 * CUSTOM_HP;
			type = "Trade";
		}
	protected:
		void func(Sea** Map);
		void mov(Sea** Map);
};
#endif
