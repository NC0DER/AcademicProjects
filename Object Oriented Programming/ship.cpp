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
