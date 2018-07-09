# Data Structures

I build this project in C++, for the Data Structures lab in university.
It is a terminal app that manages thousands of companies records from a `.csv` file.  
It can be build and run, both in `Windows` & `Linux`.  
The user can choose to Display a speficic company by id using either Linear Search or Binary Search or Interpolated Search or AVL Access.

## Features
- [x] Open / save file
- [x] Add / remove company
- [x] Linear Search
- [x] Binary Search
- [x] Interpolated Search
- [x] AVL Tree Access
- [x] Automatic sorting based on company's id

## Main Menu
```
		 ------------
		|Menu Options|
		 ------------
	1. Load companies from file
	2. Save companies to file
	3. Add a company
	4. Delete a company by id
	5. Display a company by id
	6. Display companies
	7. Display companies by surname search

	 8.Exit Program

Select a Menu Number, Input a positive integer: 
```

## Search menu
```
----------------
|Search Submenu|
----------------

	Choose method: 
		1. Linear Search
		2. AVL Access
		3. Binary Search
		4. Interpolation Search
		5. Exit
Input a positive integer: 
```

## Company Browsing Menu
```
Company Name: Roerig
Id: 26
Summary: at dolor quis odio consequat varius integer ac leo pellentesque ultrices mattis
Number of employeers: 5
Employee 1 Theresa Wells
Employee 2 Debra Morales
Employee 3 Julia Butler
Employee 4 Jean Gordon
Employee 5 Eugene Lopez

Press 0 to exit or enter to continue: 
```

## Compilation && Execution
In `Linux`, you can navigate via terminal to the project's folder and:
```bash
$ g++ *.cpp -o app  
$ ./app
```
In `Windows`, you can import the source files and include in project the data.csv file, to any IDE, and then click `Build & Run`.
