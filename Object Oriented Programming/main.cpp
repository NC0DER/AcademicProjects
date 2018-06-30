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

void deleteShip(std::vector<Ship *>& vec, Sea **map)
{
	int ID = 0;
	std::string choice = " ";
	std::string input = " ";
	int found = false;
	std::cout << "Which of the following Ships you want to destroy? " << std::endl << std::endl;
	for (int i = 0; i < vec.size(); i++)
		std::cout << *vec[i];
	Choice:
	std::cout << std::endl << "Select Ship by order of appearance e.g (1 = first): " << std::endl;
	std::cout << "Destroy> ";
	while (true) {
		std::getline(std::cin, input);
		// This code converts from string to number safely.
		std::stringstream myStream(input);
		if (myStream >> ID && ID > 0)
			break;
		std::cout << std::endl << "Invalid number, please try again" << std::endl;
		std::cout << "Destroy> ";
	}
	std::cout << std::endl;

	ID = std::abs(ID); //Worst case: negative input
	if (!vec.empty() && ID <= vec.size())
	{
		map[vec[ID - 1]->getX()][vec[ID - 1]->getY()].place = NULL;
		std::cout << map[vec[ID - 1]->getX()][vec[ID - 1]->getY()];
		delete vec[ID-1];
		vec.erase(vec.begin() + (ID-1));
		vec.resize(vec.size());
		found = true;
	}
	else if (!found)
	{
		std::cout << "Error: Invalid ID given, would you like to try again? |(y)es/(n)o|"
			<< std::endl << "> ";
			std::cin >> choice;
			if (choice == "yes" || choice == "(y)" || choice =="y")
			{
				std::cin.get();
				goto Choice;
			}
			else std::cout << std::endl;
	}
}

void Menu(std::vector<Ship *>& vec, Sea **map)
{
	std::string in = " ";
	std::string type = " ";
	int choice = 0;
	int X = 0;
	int Y = 0;
	std::cout << "\t~Simulation has been paused~" << std::endl << std::endl;
	std::cout << " ------------" << std::endl;
	std::cout << "|Menu Options|" << std::endl;
	std::cout << " ------------" << std::endl;

	std::cout << "\t1. Get Info of Map Area " << std::endl;
	std::cout << "\t2. Modify Map Area " << std::endl;
	std::cout << "\t3. Get Info of Ship " << std::endl;
	std::cout << "\t4. Get Info of speficic Ship Type " << std::endl;
	std::cout << "\t5. Add Ship " << std::endl;
	std::cout << "\t6. Delete Ship " << std::endl;
	std::cout << "\t7. General Info of Simulation " << std::endl;

	std::cout << std::endl << "\t 0.Exit Menu " << std::endl << std::endl;

	std::cout << "Input your number of choice: ";
	while (true) {
		std::getline(std::cin, in);
		// This code converts from string to number safely.
		std::stringstream myStream(in);
		if ((myStream >> choice) && (choice <= 7) && (choice >= 0))
			break;
		std::cout << std::endl << "Invalid number, please try again" << std::endl << std::endl;
		std::cout << "Input your number of choice: ";
	}
	std::cout << std::endl;

	if ((choice <= 2) && (choice > 0))
	{
		std::cout << "Select Map area by filling in the following coordinates: " << std::endl;
		std::cout << "\t X = ";
		while (true) {
			std::getline(std::cin, in);
			// This code converts from string to number safely.
			std::stringstream myStream(in);
			if ((myStream >> X) && (X >= 0) && (X < _SIZE_))
				break;
			std::cout << std::endl << "Invalid number, please try again" << std::endl;
			std::cout << "\t X = ";
		}
		std::cout << "\t Y = ";
		while (true) {
			std::getline(std::cin, in);
			// This code converts from string to number safely.
			std::stringstream myStream(in);
			if ((myStream >> Y) && (Y >= 0) && (Y < _SIZE_))
				break;
			std::cout << std::endl << "Invalid number, please try again" << std::endl;
			std::cout << "\t Y = ";
		}
		std::cout << std::endl;
	}

	if (choice == 4)
	{
		std::cout << "Specify one of available types: " << std::endl
			<< "Pirate" << std::endl
			<< "Medic" << std::endl
			<< "Recon" << std::endl
			<< "Trade" << std::endl << std::endl;
		std::cout << "Type> ";
		while (true) {
			std::getline(std::cin, in);
			// This code converts from string to number safely.
			std::stringstream myStream(in);
			if ((myStream >> type) && ((type == "Pirate") || (type == "Medic") || (type == "Recon") || (type == "Trade")))
				break;
			std::cout << std::endl << "Invalid type , please try again" << std::endl;
			std::cout << "Type> ";
		}
	}
	switch (choice)
	{
	case 0: Exit_flag = 1;
		break;
	case 1: std::cout << map[X][Y];
		break;
	case 2: std::cin >> map[X][Y];
		break;
	case 3:  if (!vec.empty()){
				for (int i = 0; i < vec.size(); i++)
					std::cout << *vec[i];
		}
		break;
	case 4: if (!vec.empty()){
		for (int i = 0; i < vec.size(); i++)
			if ((vec[i]->getType()) == type)
					std::cout << *vec[i];
	}
		break;
	case 5: addShip(vec, map);
		break;
	case 6: deleteShip(vec, map);
		break;
	case 7: if (!vec.empty()){
		int tempgold = 0;
		int tempdmg = 0;

		std::cout << "Number of gold collected by all ships: ";
		for (int i = 0; i < vec.size(); i++)
			tempgold += (vec[i]->getTotalGold());
		std::cout << tempgold << std::endl;
		tempgold = 0;
		std::cout << "Number of gold collected by Pirate ships: ";
		for (int i = 0; i < vec.size(); i++)
			if ((vec[i]->getType()) == "Pirate")
				tempgold += (vec[i]->getTotalGold());
		std::cout << tempgold << std::endl;
		tempgold = 0;
		std::cout << "Number of gold collected by Medic ships: ";
		for (int i = 0; i < vec.size(); i++)
			if ((vec[i]->getType()) == "Medic")
				tempgold += (vec[i]->getTotalGold());
		std::cout << tempgold << std::endl;
		tempgold = 0;
		std::cout << "Number of gold collected by Trade ships: ";
		for (int i = 0; i < vec.size(); i++)
			if ((vec[i]->getType()) == "Trade")
				tempgold += (vec[i]->getTotalGold());
		std::cout << tempgold << std::endl;
		tempgold = 0;
		std::cout << "Number of gold collected by Recon ships: ";
		for (int i = 0; i < vec.size(); i++)
			if ((vec[i]->getType()) == "Recon")
				tempgold += (vec[i]->getTotalGold());
		std::cout << tempgold << std::endl << std::endl;

		std::cout << "Number of Damage taken by all ships: ";
		for (int i = 0; i < vec.size(); i++)
			tempdmg += (vec[i]->getTotalDmgTaken());
		std::cout << tempdmg << std::endl;
		tempdmg = 0;
		std::cout << "Number of Damage taken by Pirate ships: ";
		for (int i = 0; i < vec.size(); i++)
			if ((vec[i]->getType()) == "Pirate")
				tempdmg += (vec[i]->getTotalDmgTaken());
		std::cout << tempdmg << std::endl;
		tempdmg = 0;
		std::cout << "Number of Damage taken by Medic ships: ";
		for (int i = 0; i < vec.size(); i++)
			if ((vec[i]->getType()) == "Medic")
				tempdmg += (vec[i]->getTotalDmgTaken());
		std::cout << tempdmg << std::endl;
		tempdmg = 0;
		std::cout << "Number of Damage taken by Trade ships: ";
		for (int i = 0; i < vec.size(); i++)
			if ((vec[i]->getType()) == "Trade")
				tempdmg += (vec[i]->getTotalDmgTaken());
		std::cout << tempdmg << std::endl;
		tempdmg = 0;
		std::cout << "Number of Damage taken by Recon ships: ";
		for (int i = 0; i < vec.size(); i++)
			if ((vec[i]->getType()) == "Recon")
				tempdmg += (vec[i]->getTotalDmgTaken());
		std::cout << tempdmg << std::endl;
		mpause();
	}
			break;
	default:
		std::cout << "Execution of User interface was not supposed to be here ";
		break;
	}
	if (Exit_flag)
		std::cout << "\t ~Simulation has been resumed~" << std::endl << std::endl;
}
