# Object Oriented Programming
I made this project for Object Oriented Programming lab in university.  
Its a terminal text-adventure game that can be run on both `Windows` & `Linux`  
The teaching goal of the project was to familiarize with OOP concepts in C++ such as:
* Classes
* Objects
* Polymorphism
* Encapsulation
* Abstraction
* Inheritance

The game consists of small square grid in which 4 types of ships move and interact as described below: 

| Ship Type  | Description |
| ------------- | ------------- |
| Pirate  | Steals a small amount of gold and reduces the hp of a nearby ship and moves 1 square.  |
| Recon  | Moves 2 squares.  |
| Trade  | Trades with a nearby dock and moves 1 square.  |
| Medic  | Repairs a nearby ship by increasing its hp and moves 1 square.  |

Every square is practically a map tile in which a ship can be located.  
Every square has a weather intensity index and can have a treasure chest with some gold in it.  
If the square has bad weather the ship can get damaged depending on the type of ship and the intensity of the weather.  
A square can either have a ship on it, or be free, or be a dock.  
Every destroyed ship is removed from the game.  
The game runs automatically as a simulation, and the user can intervene every second round.  
The game is finished once all ships are destroyed or any ship reaches the predefined gold target.  

## In-Game Features
Every 2nd round the player can pause the game and:
* Get information for a specific map area, by inputting its coordinates.
* Modify the weather or gold for a specific map area.
* Get information for all ships.
* Get Information for all ship of a specific type.
* Add a new ship, by inputting all its stats.
* Delete an existing ship.
* Display some general info about the game and the map.

## In-Game example 
```
		[ Turn 2 ]

Merchant Of Venice has moved from (1 , 2) to (2 , 3) 
Merchant Of Venice has discovered a treasure box (+30 gold)
Merchant Of Venice traded in Dock (3 , 3) current gold:150

Medic Ship2 has moved from (2 , 1) to (3 , 2) 
Medic Ship2 has discovered a treasure box (+50 gold)

The Flying Dutch has moved from (1 , 3) to (1 , 0) 
The Flying Dutch has discovered a treasure box (+40 gold)
The Flying Dutch dealt 250 damage to Recon Ship4 (1 , 1) 
The Flying Dutch stole 10 gold from Recon Ship4 (1 , 1) 

In Area(1 , 1) a destroyed ship has dropped 120 gold
This area is free 

	Weather: 3
	Loot: 120
  
Recon Ship4 (Recon type) has been destroyed!!
Press M to enter user menu or Enter <-' to continue: 
```

## Compilation && Execution
For Linux, you can type in terminal while inside the project directory:
```bash
$ g++ -std=c++11 src/*.cpp -o game
$ ./game
```
For Windows, you can import the source files in any IDE, such as Visual Studio, and click `build and run`.
