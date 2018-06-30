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
