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

