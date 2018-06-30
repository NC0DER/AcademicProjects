#ifndef RECON_H
#define RECON_H
#include "ship.h"
class Recon : public Ship
{
    public:
        Recon():Ship("Recon Ship")
        {
            maxHP = CUSTOM_HP;
            HP = CUSTOM_HP;
            type = "Recon";
        }
        Recon(std::string name)
        {
            this->name = name;
            maxHP = CUSTOM_HP;
            HP = CUSTOM_HP;
            type = "Recon";
        }
    protected:
		void func(Sea** Map);
		void mov(Sea** Map);
};
#endif
