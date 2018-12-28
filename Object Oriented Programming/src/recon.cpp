#include "../include/recon.h"

void Recon::mov(Sea** Map)
{
	int tempX = 0;
	int tempY = 0;
	int count_scan = 0;

	int x_ref = getX();
	int y_ref = getY();
	int AP_ref = getAP();
	bool found = 0;
	bool taken = 0;

	while ((!found) && (AP_ref > 0))
	{

		int choiceX = 0;
		int choiceY = 0;

		choiceX = rand() % 3;
		choiceY = rand() % 3;

		switch (choiceX)
		{
		case 0:
			tempX = x_ref - 1;
			break;
		case 1:
			tempX = x_ref + 1;
			break;
		case 2:
			tempX = x_ref;
			break; //same value
		default: std::cout << "Error::Recon mov() execution was not supposed to be here";
			break;
		}
		switch (choiceY)
		{
		case 0:
			tempY = y_ref - 1;
			break;
		case 1:
			tempY = y_ref + 1;
			break;
		case 2:
			tempY = y_ref;
			break; //same value
		default: std::cout << "Error::Recon mov() execution was not supposed to be here";
			break;
		}
		tempX = mod(tempX, _SIZE_);
		tempY = mod(tempY, _SIZE_);
		taken = (Map[tempX][tempY].gethasShip()) || (Map[tempX][tempY].getisDock());
		if (((x_ref != tempX) || (y_ref != tempY)) && (taken == false))
		{
			int posX = tempX;
			int posY = tempY;
			found = 1;
			setX((posX));
			setY((posY));
			do{
				Map[posX][posY].place = Map[x_ref][y_ref].place;
				Map[x_ref][y_ref].place = NULL;
			} while (Map[posX][posY].place == NULL);
			std::cout << name << " has moved from (" << x_ref << " , " << y_ref << ") to (" << posX << " , " << posY << ") " << std::endl;
			AP_ref--;
			setTotalMov(1); //+1
		}
	}
}

void Recon::func(Sea** Map)
{
	int x_ref = getX();
	int y_ref = getY();

	if (Map[x_ref][y_ref].getLoot() > 0)
	{
		setGold(Map[x_ref][y_ref].getLoot());
		std::cout << name << " has discovered a treasure box (+" << Map[x_ref][y_ref].getLoot() << " gold)" << std::endl;
		setTotalGold(Map[x_ref][y_ref].getLoot());
		Map[x_ref][y_ref].setLoot(0);
	}
	if (Map[x_ref][y_ref].getWeather() >= 6)
	{
		setHP(-CUSTOM_HP);
		std::cout << name << " has taken " << CUSTOM_HP << " damage from large waves and strong winds!" << std::endl;
		setTotalDmgTaken(CUSTOM_HP);
	}
	std::cout << std::endl;
}
