Made by Tarik Merzkani 5/6/2022

To Run:
Open a linux terminal on this directory, and use the provided
makefile to compile the c code. 
Use commands make clean and then make to obtain the executable.
Afterwards, running the executable checkers should be employed
with any of the two following commands:
Usage:
./checkers 
	//This is for Human vs AI game. You will be prompted
	//to choose difficulty (same as ply depth).
	//Try out in between 9 and 15!

Second Usage:
./checkers <anyarg>
	//If one argument is given, then it will simulate
	//AI vs AI games. You can choose how many games.
	//You won't be prompted to change the difficulty of the two
	//AIs (not implemented yet). This has to be modified
	//in line 268: 'run = normalGame(9, 9, &totaltime);'
	//Just need to change the int value parameters

Thank you!