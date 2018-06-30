#include "ship.h"

 int Ship::id = 0;//Total Number of ships
 int Sea::count_ref = 0;//Number of Sea tiles destroyed

std::string Ship::getName()const
{
    try{
        return name;
    }
    catch (std::bad_alloc& ba){}
}
std::string Ship::getType()const
{
    try{
        return type;
    }
    catch (std::bad_alloc& ba){}
}
int Ship::getID()const
{
    return curr_id;
}
int Ship::getTotalGold()const
{
    return TotalGold;
}
int Ship::getGold()const
{
    return gold;
}
int Ship::getTotalDmgTaken()const
{
    return TotalDmgTaken;
}
int Ship::getTotalMov()const
{
    return TotalMov;
}
int Ship::getAP()const
{
    return AP;
}
int Ship::getX()const
{
    return x;
}
int Ship::getY()const
{
    return y;
}
void Ship::setX(int x)
{
    this->x = x;
}
void Ship::setY(int y)
{
    this->y = y;
}
void Ship::setTotalMov(int mov)
{
    TotalMov += mov;
}
void Ship::setTotalDmgTaken(int h)
{
    TotalDmgTaken+=h;
}
void Ship::setTotalGold(int g)
{
    TotalGold+=g;
}
void Ship::setGold(int g)
{
    gold+=g;
}
int Ship::getHP()const
{
    return HP;
}
int Ship::getmaxHP()const
{
    return maxHP;
}
void Ship::setHP(int HP)
{
     this->HP += HP;
}
