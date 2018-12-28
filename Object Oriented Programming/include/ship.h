#ifndef SHIP_H
#define SHIP_H

#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <cstdlib>
#include <ctime>

#define CUSTOM_HP 500
#define START_GOLD 100
#define _SIZE_ 4 //Recommended Size >= 4

class Sea;
class Ship{
public: static int id; //Number of all Ships created since start

protected:
	std::string name;
	std::string type;
	int HP;
	int maxHP;
private:
	int gold, AP;//All accumulative stats are zero upon creation except AP,Gold
	int	TotalMov, TotalGold, TotalDmgTaken, TotalDmgDealt;
	int x,y; //Coordinates of ship
	int curr_id;
public:
	Ship(std::string _name = "Ship"){
		gold = START_GOLD;
		TotalMov = 0;
		TotalDmgDealt = 0;
		x = y = 0;
		TotalGold = START_GOLD;
		TotalDmgTaken = 0;
		AP = 2;
		std::ostringstream tmp;
		std::string temp;
		tmp << _name << ++id;
		name = tmp.str();
		curr_id = id;
	}
	virtual void func(Sea** Map)=0;
	virtual void mov(Sea** Map)=0;

	~Ship(){
		std::cout << name << " (" << type << " type) has been destroyed!!" << std::endl << std::endl;
	}
public:
	std::string getName()const;
	std::string getType()const;
	int getID()const;
	int getTotalGold()const;
	int getGold()const;
	int getTotalDmgTaken()const;
	int getHP()const;
	int getmaxHP()const;
	void setHP(int HP);
	int getTotalMov()const;
	void setTotalMov(int);
	int getAP()const;
	int getX()const;
	int getY()const;
	void setTotalGold(int);
	void setGold(int);
	void setX(int);
	void setY(int);
	void setTotalDmgTaken(int);
	int getTotalDmgDealt()const;
	void setTotalDmgDealt(int);
	friend std::istream& operator>>(std::istream& input , Ship &S);
	friend std::ostream& operator<<(std::ostream& output, Ship &S);//delete keyword after following declaration is C11 syntax
	Ship(const Ship& that) = delete;//Non-copyable -> Protection from object slicing when casting
	Ship& operator=(const Ship& that) = delete;//Non-reassignable -> Protection from object slicing when casting
};

class Sea{
private:
	int weather; //1-10
	int loot;
	bool isDock;
public:
	static int count_ref;
	Ship* place;
	Sea(){
		weather = loot = 0;
		isDock = 0;
		place = NULL;
	}
	~Sea(){
		std::cout<< "Sea Tile #" << std::setw(2) << ++count_ref << " has been destroyed!" << std::endl;
	}
	int getWeather()const;
	int getLoot()const;
	bool gethasShip()const;
	bool getisDock()const;
	void setisDock(bool);
	void setLoot(int);
	void setWeather(int);
	friend std::ostream& operator<<(std::ostream& output, Sea &s);
	friend std::istream& operator>>(std::istream& input, Sea &s);
	Sea& operator++();
	Sea& operator--();
};
int mod(int a, int b);
#endif
