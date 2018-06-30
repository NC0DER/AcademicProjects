#ifndef MEDIC_H
#define MEDIC_H
#include "ship.h"
class Medic : public Ship
{
    public:
        Medic():Ship("Medic Ship")
        {
            maxHP = 2 * CUSTOM_HP;
            HP = 2 * CUSTOM_HP;
            type = "Medic";
        }
        Medic(std::string name):Ship(name)
        {
            this->name = name;
            maxHP = 2 * CUSTOM_HP;
            HP= 2 * CUSTOM_HP;
            type = "Medic";
        }
	protected:
		void func(Sea** Map);
		void mov(Sea** Map);
};
#endif
