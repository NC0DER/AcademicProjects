#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
#include <random>
#include <functional>
#include <array>
#include <new>

#include "pirate.h"
#include "medic.h"
#include "recon.h"
#include "trade.h"
#define ZzZ 800 //Sleep Time in msec for all msleep() calls
#define Reach_Gold 500 //Amount of gold to end simulation

#ifdef __gnu_linux__
#include <unistd.h> //Usleep()
#endif

#ifdef _WIN32
#include <windows.h>//sleep()
#endif

void msleep(int sleep_time);
void mpause();
void addShip(std::vector<Ship *>& vec, Sea **map);
void deleteShip(std::vector<Ship *>& vec, Sea **map);
void Menu(std::vector<Ship *>& vec, Sea **map);

bool Exit_flag = false;
int main()
{
		return 0;
}

void msleep(int sleep_time)
{
#ifdef __gnu_linux__

	usleep(sleep_time * 1000);   // usleep accepts sleep time in microseconds (1 thousandth of a millisecond)

#endif
#ifdef _WIN32

	Sleep(sleep_time);

#endif
}

void mpause()
{
	std::cout << "Press Enter <-' to continue..." << std::endl;
	std::cin.ignore(200, '\n');
}

void addShip(std::vector<Ship *>& vec, Sea **map)
{
	std::string name = " ";
	int type = 0;
	int count = 0;
	int loop_occur = 0;
	int tempX = 0;
	int tempY = 0;
	std::string input = " ";
	bool found = false;
	std::default_random_engine generatorXY(time(0));
	std::uniform_int_distribution<int> range(0, _SIZE_ - 1);
	auto diceXY = std::bind(range, generatorXY);
	std::cout << "Input name and and number of ship type: " << std::endl;
	std::cout << "Available types of ship:" << std::endl << "\t1.Pirate" << std::endl << "\t2.Trade" << std::endl
		<< "\t3.Recon" << std::endl << "\t4.Medic" << std::endl;

	do{
		std::cout << "Name> ";
		while(std::getline(std::cin,name))break;//Capture String despite whitespaces
		if (name == " " || name == "\n" || name == "")
			std::cout << "Invalid name, please renter" << std::endl << std::endl;
	} while (name == " " || name == "\n" || name == "");

	std::cout << "Type> ";
	while (true) {
		std::getline(std::cin, input);
		// This code converts from string to number safely.
		std::stringstream myStream(input);
		if ((myStream >> type) && (type > 0))
			break;
		std::cout << std::endl << "Invalid number, please try again" << std::endl;
		std::cout << "Type> ";
	}
	std::cout << std::endl;
	type = ((std::abs(type) % (_SIZE_+1))); //Correct user input (number out of range)
	if (type == 0)
		++type;

	switch (type)
	{
	case 1:
	{

		Ship * s = new Pirate(name);

		while (found != true)
		{
			tempX = diceXY();
			tempY = diceXY();
			std::cout << "Testing ( " << tempX << "," << tempY << " )" << std::endl;
			for (int i = 0; i < vec.size(); i++)
				if ((tempX != vec[i]->getX()) || (tempY != vec[i]->getY()))
					++count;
			std::cout << "Tested coordinates differ for #" << count << " out of total #" << vec.size() << " coordinate sets" << std::endl;
			if (count == vec.size())
			{
				std::cout << "The coordinates are not occupied by any ship" << std::endl;
				found = true;
			}
			if (map[tempX][tempY].getisDock())
			{
				std::cout << "...but they belong to a port" << std::endl << "Reselecting..." << std::endl << std::endl;
				found = false;
			}
			count = 0;
			mpause();
			++loop_occur;
			if (loop_occur >= 10)
			{
				std::cout << "Not enough space to add ship." << std::endl << "Aborting..." << std::endl;
				delete s;
				goto Error_Exit;
			}
		}
		s->setX(tempX);
		s->setY(tempY);
		map[tempX][tempY].place = s;
		vec.push_back(s);
		break;
	}
	case 2:
	{
		Ship * s = new Trade(name);

		while (found != true)
		{
			tempX = diceXY();
			tempY = diceXY();
			std::cout << "Testing ( " << tempX << "," << tempY << " )" << std::endl;
			for (int i = 0; i < vec.size(); i++)
				if ((tempX != vec[i]->getX()) || (tempY != vec[i]->getY()))
					++count;
			std::cout << "Tested coordinates differ for #" << count << " out of total #" << vec.size() << " coordinate sets" << std::endl;
			if (count == vec.size())
			{
				std::cout << "The coordinates are not occupied by any ship" << std::endl;
				found = true;
			}
			if (map[tempX][tempY].getisDock())
			{
				std::cout << "...but they belong to a port" << std::endl << "Reselecting..." << std::endl << std::endl;
				found = false;
			}
			count = 0;
			mpause();
			++loop_occur;
			if (loop_occur >= 10)
			{
				std::cout << "Not enough space to add ship." << std::endl << "Aborting..." << std::endl;
				delete s;
				goto Error_Exit;
			}
		}
		s->setX(tempX);
		s->setY(tempY);
		map[tempX][tempY].place = s;
		vec.push_back(s);
		break;
	}
	case 3:
	{
		Ship * s = new Recon(name);

		while (found != true)
		{
			tempX = diceXY();
			tempY = diceXY();
			std::cout << "Testing ( " << tempX << "," << tempY << " )" << std::endl;
			for (int i = 0; i < vec.size(); i++)
				if ((tempX != vec[i]->getX()) || (tempY != vec[i]->getY()))
					++count;
			std::cout << "Tested coordinates differ for #" << count << " out of total #" << vec.size() << " coordinate sets" << std::endl;
			if (count == vec.size())
			{
				std::cout << "The coordinates are not occupied by any ship" << std::endl;
				found = true;
			}
			if (map[tempX][tempY].getisDock())
			{
				std::cout << "...but they belong to a port" << std::endl << "Reselecting..." << std::endl << std::endl;
				found = false;
			}
			count = 0;
			mpause();
			++loop_occur;
			if (loop_occur >= 10)
			{
				std::cout << "Not enough space to add ship." << std::endl << "Aborting..." << std::endl;
				delete s;
				goto Error_Exit;
			}
		}
		s->setX(tempX);
		s->setY(tempY);
		map[tempX][tempY].place = s;
		vec.push_back(s);
		break;
	}
	case 4:
	{
		Ship * s = new Medic(name);

		while (found != true)
		{
			tempX = diceXY();
			tempY = diceXY();
			std::cout << "Testing ( " << tempX << "," << tempY << " )" << std::endl;
			for (int i = 0; i < vec.size(); i++)
				if ((tempX != vec[i]->getX()) || (tempY != vec[i]->getY()))
					++count;
			std::cout << "Tested coordinates differ for #" << count << " out of total #" << vec.size() << " coordinate sets" << std::endl;
			if (count == vec.size())
			{
				std::cout << "The coordinates are not occupied by any ship" << std::endl;
				found = true;
			}
			if (map[tempX][tempY].getisDock())
			{
				std::cout << "...but they belong to a port" << std::endl << "Reselecting..." << std::endl << std::endl;
				found = false;
			}
			count = 0;
			mpause();
			++loop_occur;
			if (loop_occur >= 10)
			{
				std::cout << "Not enough space to add ship." << std::endl << "Aborting..." << std::endl;
				delete s;
				goto Error_Exit;
			}
		}
		s->setX(tempX);
		s->setY(tempY);
		map[tempX][tempY].place = s;
		vec.push_back(s);
		break;
	}
	default:
	{
		std::cout << "Execution of addShip() was not supposed to be here " << std::endl;
		break;
	}
	Error_Exit:;
	}
}

