#ifndef PIRATE_H
#define PIRATE_H

#include "ship.h"
class Pirate : public Ship
{
	public:
		Pirate():Ship("Pirate Ship")
		{
			maxHP = 2 * CUSTOM_HP;
			HP = 2 * CUSTOM_HP;
			type = "Pirate";
		}
		Pirate(std::string name)
		{
			this->name = name;
			maxHP = 2 * CUSTOM_HP;
			HP = 2 * CUSTOM_HP;
			type = "Pirate";
		}
	protected:
		void func(Sea** Map);
		void mov(Sea** Map);
};
#endif
