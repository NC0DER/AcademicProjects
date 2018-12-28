#include "../include/ship.h"

 int Ship::id = 0;//Total Number of ships
 int Sea::count_ref = 0;//Number of Sea tiles destroyed

std::string Ship::getName()const
{
	return name;
}
std::string Ship::getType()const
{
	return type;
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
int Ship::getTotalDmgDealt()const
{
	 return TotalDmgDealt;
}
void Ship::setTotalDmgDealt(int x)
{
	TotalDmgDealt += x;
}
int Sea::getWeather()const
{
	return weather;
}
bool Sea::getisDock()const
{
	return isDock;
}
bool Sea::gethasShip()const
{
	if (place == NULL)
		return false;
	else return true;
}
void Sea::setisDock(bool i)
{
	isDock = i;
}
int Sea::getLoot()const
{
	return loot;
}
void Sea::setLoot(int loot)
{
	this->loot = loot;
}
void Sea::setWeather(int weather){
	this->weather = std::abs(weather);
}
Sea& Sea::operator++()
{
	if ((++weather) == 11)
		weather-=5;
	return *this;
}
Sea& Sea::operator--()
{
	if ((--weather) <= 0)
		weather++;
	return *this;
}
std::ostream &operator<<(std::ostream &output, Ship &S)
{
	output << S.name << " (" << S.type << " type) " << std::endl
		<< "Id: " << S.curr_id << std::endl
		<< "\tCoordinates: (" << S.x << "," << S.y << ") " << std::endl
		<< "\tHealth Points (" << S.HP << "/" << S.maxHP << ")" << std::endl;
	if (S.type == "Pirate")
	{
		output << "\tPower: " << 0.5*CUSTOM_HP << std::endl
			   << "\tTotal damage dealt: " << S.getTotalDmgDealt() << std::endl
			   << "\tNumber of Moves per round: " << (S.AP)-1 << std::endl;
	}
	else output << "\tNumber of Moves per round: " << S.AP << std::endl;
	output<< "\tTotal Number of moves: " << S.TotalMov << std::endl
		<< "\tTotal damage taken: " << S.TotalDmgTaken << std::endl
		<< "\tGold: " << S.gold << std::endl
		<< "\tTotal Gold: " << S.TotalGold << std::endl << std::endl;

	return output;
}
std::istream &operator>>(std::istream &input, Ship &S)
{
	std::string in = " ";
	std::cout << "Set New Coordinates: " << std::endl;
	std::cout << "\t X = ";
	while (true) {
		std::getline(std::cin, in);
		// This code converts from string to number safely.
		std::stringstream myStream(in);
		if ((myStream >> S.x) && (S.x >= 0))
			break;
		std::cout << std::endl << "Invalid number, please try again" << std::endl;
		std::cout << "\t X = ";
	}
	std::cout << "\t Y = ";
	while (true) {
		std::getline(std::cin, in);
		// This code converts from string to number safely.
		std::stringstream myStream(in);
		if ((myStream >> S.y) && (S.y >= 0))
			break;
		std::cout << std::endl << "Invalid number, please try again" << std::endl;
		std::cout << "\t Y = ";
	}
	std::cout << std::endl;
	return input;
}
std::ostream& operator<<(std::ostream& output, Sea &s)
{
	if (s.isDock)
		output << "This area has a harbour";
	else if (s.gethasShip())
	{
		output << "This area is occupied by "
			<< s.place->getName() << " of "
			<< s.place->getType() << " type ";
	}
	else output << "This area is free ";

	output << std::endl << std::endl;
	output << "\tWeather: " << s.weather << std::endl;
	output << "\tLoot: " << s.loot << std::endl << std::endl;
	return output;
}
std::istream& operator>>(std::istream& input, Sea &s)
{
	std::string in = " ";
	int choice = 0;
	std::cout << "Choose Modification for this area: " << std::endl;
	std::cout << "\t1. Build Port" << std::endl;
	std::cout << "\t2. Remove Port" << std::endl;
	std::cout << "\t3. Add Treasure box" << std::endl;
	std::cout << "\t4. Remove Treasure box" << std::endl << std::endl;

	std::cout << "Input your number of choice: ";
	while (true) {
		std::getline(std::cin, in);
		// This code converts from string to number safely.
		std::stringstream myStream(in);
		if ((myStream >> choice) && (choice <=4) && (choice>0))
			break;
		std::cout << std::endl << "Invalid number, please try again" << std::endl;
		std::cout << "Input your number of choice: ";
	}
	std::cout << std::endl;


	switch (choice)
	{
	case 1: if (s.isDock)
				std::cout << "This area already has a Port" << std::endl << std::endl;
			else if (s.gethasShip())
				std::cout << "This area has a Ship" << std::endl << std::endl;
			else (s.isDock = true);
			break;
	case 2: if (s.isDock)
				(s.isDock = false);
			else std::cout << "This area doesn't have a Port" << std::endl << std::endl;
			break;
	case 3: if (s.loot > 0)
	{
		std::cout << "This area has a treasure box with +" << s.loot << " gold" << std::endl;
		std::cout << "Input the new amount of gold for the treasure box: ";
		while (true) {
			std::getline(std::cin, in);
			// This code converts from string to number safely.
			std::stringstream myStream(in);
			if ((myStream >> s.loot) && (s.loot > 0))
				break;
			std::cout << std::endl << "Invalid number, please try again" << std::endl;
			std::cout << "Input the new amount of gold for the treasure box: ";
		}
		std::cout << std::endl;
	}
		else
		{
			std::cout << "This area has no treasure box" << std::endl;
			std::cout << "Input the new amount of gold for the treasure box: ";
			while (true) {
				std::getline(std::cin, in);
				// This code converts from string to number safely.
				std::stringstream myStream(in);
				if ((myStream >> s.loot) && (s.loot > 0))
					break;
				std::cout << std::endl << "Invalid number, please try again" << std::endl;
				std::cout << "Input the new amount of gold for the treasure box: ";
			}
			std::cout << std::endl;
		}
			break;
	case 4: if (s.loot == 0)
				std::cout << "This area has no treasure box " << std::endl << std::endl;
			else{
				std::cout << "Treasure box with +" << s.loot << " gold " << "has been removed " << std::endl << std::endl;
					s.loot = 0;
				}
			break;
	}
	return input;
}

int mod(int a, int b)
{
	int result;
	result = a % b;
	return result >= 0 ? result : result += b;
}
