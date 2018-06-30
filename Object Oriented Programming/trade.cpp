#include "trade.h"

void Trade::func(Sea** Map)
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
	if (Map[x_ref][y_ref].getWeather() >= 9)
	{
		setHP((int)-CUSTOM_HP);
		std::cout << name << " has taken " << CUSTOM_HP << " damage from humongous waves near a turbulent hurricane!" << std::endl;
		setTotalDmgTaken((int)CUSTOM_HP);
	}
	for (int i = -1; i <= 1; i++)
	{
		for (int j = -1; j <= 1; j++)
		{
			posX = mod(x_ref + i, _SIZE_);
			posY = mod(y_ref + j, _SIZE_);
			if (Map[posX][posY].getisDock())
			{
				setGold(static_cast<int>(0.1 * START_GOLD));
				setTotalGold(static_cast<int>(0.1 * START_GOLD));
				std::cout << name << " traded in Dock (" << posX << "," << posY << ") " << "current gold:" << getGold() << std::endl;
				goto Trade_Once;//Trades with only one dock then exits nested search loop
			}
		}
	}
	Trade_Once:;
	std::cout << std::endl;
}

