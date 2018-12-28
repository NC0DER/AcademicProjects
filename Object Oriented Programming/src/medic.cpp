#include "../include/medic.h"

void Medic::mov(Sea** Map)
{
	int tempX = 0;
	int tempY = 0;

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
		default: std::cout << "Error::Medic mov() execution was not supposed to be here";
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
		default: std::cout << "Error::Medic mov() execution was not supposed to be here";
			break;
		}
		tempX = mod(tempX, _SIZE_);
		tempY = mod(tempY, _SIZE_);
		taken = (Map[tempX][tempY].gethasShip()) || (Map[tempX][tempY].getisDock());
		if (((x_ref != tempX) || (y_ref != tempY)) && (taken == false))
		{
			int posX = mod(tempX, _SIZE_);
			int posY = mod(tempY, _SIZE_);
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
void Medic::func(Sea** Map)
{
	int x_ref = getX();
	int y_ref = getY();

	int posX = 0;
	int posY = 0;

	if (Map[x_ref][y_ref].getLoot() > 0)
	{
		setGold(Map[x_ref][y_ref].getLoot());
		std::cout << name << " has discovered a treasure box (+" << Map[x_ref][y_ref].getLoot() << " gold)" << std::endl;
		setTotalGold(Map[x_ref][y_ref].getLoot());
		Map[x_ref][y_ref].setLoot(0);
	}
	if (Map[x_ref][y_ref].getWeather() >= 8)
	{
		setHP(-CUSTOM_HP);
		std::cout << name << " has taken " << CUSTOM_HP << " damage from humongous waves inside a ferocious storm!" << std::endl;
		setTotalDmgTaken(CUSTOM_HP);
	}
	for (int i = -1; i <= 1; i++)
	{
		for (int j = -1; j <= 1; j++)
		{
			posX = mod(x_ref + i,_SIZE_);
			posY = mod(y_ref + j, _SIZE_);
			if (Map[posX][posY].gethasShip() == true)
			{
				if (((posX != x_ref) || (posY != y_ref)))
				{
					int HP_ref = Map[posX][posY].place->getHP();
					int maxHP_ref = Map[posX][posY].place->getmaxHP();
					int gold_ref = Map[posX][posY].place->getGold();
					if ((HP_ref < maxHP_ref) && (HP_ref >= 0) && (gold_ref >= (static_cast<int>(0.1 * START_GOLD))))
					{
						Map[posX][posY].place->setHP(static_cast<int>(0.5 * CUSTOM_HP));
						Map[posX][posY].place->setGold(static_cast<int>(-0.1 * START_GOLD));
						setGold(static_cast<int>(0.1 * START_GOLD));
						setTotalGold(static_cast<int>(0.1 * START_GOLD));
						std::cout << name << " repaired " << Map[posX][posY].place->getName() << " (" << posX << "," << posY << ") for " << static_cast<int>(0.5 * CUSTOM_HP) << " HP "
							 << "while redeeming " << static_cast<int>(0.1 * START_GOLD) <<" gold "<< std::endl;
						goto Repair_Once; //Repairs only one ship then exits nested search loop
					}
				}
			 }
		}
	}
	Repair_Once:
	std::cout << std::endl;
}
