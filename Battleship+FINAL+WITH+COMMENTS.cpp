//Taylor Myers
//Battleship Final
//11/20/2018

#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <array>	
#include <iomanip>
using namespace std;

const int COLS = 10;
const int ROWS = 10;
const string shipNames[] = { "Carrier", "Battleship", "Cruiser", "Submarine", "Destroyer" };
const int shipLength[] = { 5, 4, 3, 3, 2 };

void placeComputerShips(char[][COLS]);
void displayBoard(char[][COLS]);
void reset(char[][COLS]);
void placePlayerShips(char[][COLS], char[][COLS], string[], string[]);
void screen(char[][COLS], char[][COLS], string[], string[]);
void playerTurn(char[][COLS], char[][COLS], string[], string[], char[][COLS]);
void computerTurn(char[][COLS], char[][COLS], string[], string[], char[][COLS], char[][COLS]);
bool allShipsSunk(string[]);
bool shipSunk(int, char[][COLS]);

int main()
{
	char playerShipsBoard[ROWS][COLS];
	char playerGuessingBoard[ROWS][COLS];
	char computerGuessingBoard[ROWS][COLS];
	char computerShipsBoard[ROWS][COLS];

	reset(playerShipsBoard);
	reset(playerGuessingBoard);
	reset(computerGuessingBoard);
	reset(computerShipsBoard);

    //these are to show the player what ships are left and judges when either the computer or player wins
	string playerShipsLeft[] = { "1. Carrier (5)", "2. Battleship (4)", "3. Cruiser (3)", "4. Submarine (3)", "5. Destroyer (2)" };
	string computerShipsLeft[] = { "1. Carrier (5)", "2. Battleship (4)", "3. Cruiser (3)", "4. Submarine (3)", "5. Destroyer (2)" };

	string winner = "No one yet.";


	placePlayerShips(playerShipsBoard, playerGuessingBoard, playerShipsLeft, computerShipsLeft);

	placeComputerShips(computerShipsBoard);
	cout << "The computer has placed its ships.";

	system("pause");

	system("cls");

    //this loops while there us still no winner
	while (winner == "No one yet.")
	{
		playerTurn(playerShipsBoard, playerGuessingBoard, playerShipsLeft, computerShipsLeft, computerShipsBoard);
		if (allShipsSunk(computerShipsLeft))
		{
			system("cls");


			winner = "player";
			cout << "CONGRATULATIONS!!! You have beaten the computer.";

			cout << "\n\nI8,        8        ,8I  88  888b      88  888b      88  88888888888  88888888ba   88  88  88 \n`8b       d8b       d8\'  88  8888b     88  8888b     88  88           88      \"8b  88  88  88  \n \"8,     ,8\"8,    , 8\"   88  88 `8b    88  88 `8b    88  88           88      ,8P  88  88  88 \n  Y8     8P Y8     8P    88  88  `8b   88  88  `8b   88  88aaaaa      88aaaaaa8P'  88  88  88  \n  `8b   d8\' `8b   d8\'    88  88   `8b  88  88   `8b  88  88\"\"\"\"\"      88\"\"\"\"88\'    88  88  88  \n  `\'8a a8'   `8a a8\'     88  88    `8b 88  88    `8b 88  88           88    `8b    \"\"  \"\"  \"\" \n    `8a8\'     `8a8\'      88  88     `8888  88     `8888  88           88     `8b   aa  aa  aa \n     `8\'       `8\'       88  88      `888  88      `888  88888888888  88      `8b  88  88  88	";
			exit(0);
		}

		computerTurn(playerShipsBoard, playerGuessingBoard, playerShipsLeft, computerShipsLeft, computerShipsBoard, computerGuessingBoard);
		if (allShipsSunk(playerShipsLeft))
		{
			system("cls");

			winner = "computer";
			cout << "Sorry the computer has beaten you. Better luck next time.";
		}
	}


	cout << "\n\n";
	system("pause");
	return 0;
}

//This is the computer taking its turn
void computerTurn(char playerShipsBoard[][COLS], char playerGuessingBoard[][COLS], string playerShips[], string computerShips[], char computerShipsBoard[][COLS], char computerGuessingBoard[][COLS])
{
	system("cls");
	int rowNum = 0, col = 0, whichShip = 0;

    //generates a random cordinate to guess whether it hits a ship or not, also no cordinate will guessed twice
	do {
		rowNum = rand() % 10;
		col = rand() % 10;
	} while (computerGuessingBoard[rowNum][col] == 'H' || computerGuessingBoard[rowNum][col] == 'M');

    //this happends if there is a miss
	if (playerShipsBoard[rowNum][col] == 'O')
	{
		if (computerGuessingBoard[rowNum][col] != 'H')
			computerGuessingBoard[rowNum][col] = 'M';
		playerShipsBoard[rowNum][col] = '#';
		screen(playerShipsBoard, playerGuessingBoard, playerShips, computerShips);
		cout << "\nThe computer missed.\n";

	}

    //occurs when there is a hit
	else
	{
		computerGuessingBoard[rowNum][col] = 'H';
		whichShip = playerShipsBoard[rowNum][col];
		whichShip -= 48;
		playerShipsBoard[rowNum][col] = 'X';
		if (shipSunk(whichShip, playerShipsBoard))
		{
			playerShips[whichShip - 1] = "\t\t";
			screen(playerShipsBoard, playerGuessingBoard, playerShips, computerShips);
			cout << "Sorry. The computer sunk you " << shipNames[(whichShip - 1)] << " ship.\n";
		}
		else
		{
			screen(playerShipsBoard, playerGuessingBoard, playerShips, computerShips);
			cout << "The computer hit one of your ships.\n";
		}
	}
	system("pause");

}

//this is the player taking its turn
void playerTurn(char playerShipsBoard[][COLS], char playerGuessingBoard[][COLS], string playerShips[], string computerShips[], char computerShipsBoard[][COLS])
{
	system("cls");

	screen(playerShipsBoard, playerGuessingBoard, playerShips, computerShips);


	char row;
	int rowNum = 0, col = 0, whichShip = 0;

    //allows the player to guess where a computer ship might be
	do {
		cout << "Please enter the row of your guess (A-J): ";
		cin >> row;
		cin.ignore();
		row = toupper(row);
	} while (int(row) < 'A' || int(row) > 'J');

	do {
		cout << "Please enter the column of your guess (0-9): ";
		cin >> col;
		cin.ignore();
	} while (!(col >= 0 || col <= 9));

    //converts from ascii to int
	rowNum = row - 65;

    //already guessed cordinate
	if (playerGuessingBoard[rowNum][col] == 'H' || playerGuessingBoard[rowNum][col] == 'M')
		cout << "\nYou have already guessed that cordinate.\n";

    //missed cordinate
	else if (computerShipsBoard[rowNum][col] == 'O')
	{
		screen(playerShipsBoard, playerGuessingBoard, playerShips, computerShips);

		cout << "\nNice try.\n";
		if (playerGuessingBoard[rowNum][col] != 'H')
			playerGuessingBoard[rowNum][col] = 'M';
	}

    //player his the ship
	else
	{
		playerGuessingBoard[rowNum][col] = 'H';
		whichShip = computerShipsBoard[rowNum][col];
		whichShip -= 48;
		computerShipsBoard[rowNum][col] = 'O';

		if (shipSunk(whichShip, computerShipsBoard))
		{
			screen(playerShipsBoard, playerGuessingBoard, playerShips, computerShips);
			cout << "Congrats you sunk the computers " << shipNames[(whichShip - 1)] << " ship.\n";
			computerShips[whichShip - 1] = "\t\t";
		}
		else
		{
			screen(playerShipsBoard, playerGuessingBoard, playerShips, computerShips);
			cout << "\nGood Job! You hit a ship.\n";
		}
	}
	system("pause");

}

//tests if all the ships have been sunk
bool allShipsSunk(string ships[])
{
	for (int i = 0; i < 5; i++)
	{
		if (ships[i] != "\t\t")
			return false;
	}
	return true;
}

//tests is a ship has been sunk
//goes through every element in the array to see if there is another spot in the ship left
bool shipSunk(int whichShip, char ary[][COLS])
{
	int currentCheck = 0;

	for (int r = 0; r < ROWS; r++)
	{
		for (int c = 0; c < COLS; c++)
		{
			if (isdigit(ary[r][c]))
			{
				currentCheck = int(ary[r][c]);
				currentCheck -= 48;
				if (currentCheck == whichShip)
					return false;
			}
		}
	}
	return true;
}

//this randomly places the computers ships
void placeComputerShips(char ary[][COLS])
{
	string ship = "";
	char numShip = 49;
	int col, rowNum, direction;
	static int shipLengthInd = 0;
	bool usedSpace = false;
    
    //sets up random 
	unsigned seed = time(0);
	srand(seed);

    //goes through each ship and places them 
	for (string ship : shipNames)
	{
		for (int i = 0; i < 5; i++)
		{
			if (ship == shipNames[i])
				shipLengthInd = shipLength[i];
		}

        //orients the ships
		direction = rand() % 2;

		if (direction == 0)		//horisontal 
		{
			do {
				rowNum = rand() % 10;
				col = rand() % 10;

				usedSpace = false;

                //tests if there are any ships that are already there and makes dure that there are no overlaping ships
				for (int i = 0; i < shipLengthInd; i++)
				{
					if (ary[rowNum][i + col] != 'O' || i + col > 9)
					{
						usedSpace = true;
					}
				}

                //places ship is all spots are aviable 
				if (!(usedSpace))
				{
					for (int i = 0; i < shipLengthInd; i++)
					{
						ary[rowNum][i + col] = numShip;
					}
					numShip += 1;
				}
			} while (usedSpace);

		}
		else					//vertical
		{
			do {
				rowNum = rand() % 10;
				col = rand() % 10;

				usedSpace = false;

                //tests if there are any ships that are already there and makes dure that there are no overlaping ships
				for (int i = 0; i < shipLengthInd; i++)
				{
					if (ary[i + rowNum][col] != 'O' || i + col > 9)
					{
						usedSpace = true;
					}
				}

                //places ship is all spots are aviable 
				if (!(usedSpace))
				{
					for (int i = 0; i < shipLengthInd; i++)
					{
						ary[i + rowNum][col] = numShip;
					}
					numShip += 1;
				}
			} while (usedSpace);

		}
	}

}

//this sets all of the arrays to 0's
void reset(char ary[][COLS])
{
	for (int r = 0; r < ROWS; r++)
	{
		for (int c = 0; c < COLS; c++)
		{
			ary[r][c] = 'O';
		}
	}
}

//this displays the two arrays that the player needs to see along with the scoreboard
void screen(char playerShipsBoard[][COLS], char playerGuessingBoard[][COLS], string playerShips[], string computerShips[])
{
	system("cls");

	int r = 0;
	cout << "    0 1 2 3 4 5 6 7 8 9  \t\t\t\t\t";
	cout << "    0 1 2 3 4 5 6 7 8 9" << endl;
	cout << "  ------------------------\t\t\t\t\t";
	cout << "  ------------------------" << endl;

    //displays the two arrays
	for (int l = 'A'; l < 'K'; l++)
	{
		cout << (char)l << " | ";
		for (int i = 0; i < COLS; i++)
		{
			cout << playerShipsBoard[r][i] << " ";
		}
		cout << "|\t\t\t\t\t";

		cout << (char)l << " | ";
		for (int i = 0; i < COLS; i++)
		{
			cout << playerGuessingBoard[r][i] << " ";
		}
		cout << "|" << endl;
		r++;
	}

    // displays the scoreboard
	cout << "\t   -----Scoreboard-----" << "\n\n";
	cout << "Enemy Ships Left:" << setw(25) << "Your Ships Left:\n";
	for (int i = 0; i < 5; i++)
	{
		cout << computerShips[i] << setw(25) << playerShips[i] << endl;
	}

}

//this shows one array at a time
void displayBoard(char ary[][COLS])
{
	for (int i = 0; i < ROWS; i++)
	{
		for (int q = 0; q < COLS; q++)
		{
			cout << ary[i][q] << " ";
		}
		cout << endl;
	}
}

//this allows the player to place their ships
void placePlayerShips(char ary[][COLS], char compAry[][COLS], string pSLeft[], string cSLeft[])
{
	char direction, numShip = 49, row;
	int col, rowNum;
	string ship = "";
	static int shipLengthInd = 0;
	bool usedSpace = false;

    //loops through each ship
	for (string ship : shipNames)
	{
		system("cls");
		screen(ary, compAry, pSLeft, cSLeft);

        //gets the ship length
		for (int i = 0; i < 5; i++)
		{
			if (ship == shipNames[i])
				shipLengthInd = shipLength[i];
		}

        //goes through once and continues to loop until the player enters a vaild cordinate
		do {

			usedSpace = false;

            //asks the usser verticle to horizontal
			do {
				cout << "Would you like your " << ship << " to be horizontal(H) or vertical(V)? ";
				cin >> direction;
				cin.ignore();
				direction = toupper(direction);
			} while (toupper(direction) != 'H' && toupper(direction) != 'V');

			if (direction == 'H')
			{
                //gets exact starting location of the ship
				do {
					cout << "What row would you like to put your " << ship << " in (A-J)? ";
					cin >> row;
					cin.ignore();
					row = toupper(row);
				} while (int(row) < 'A' || int(row) > 'J');

				do {
					cout << "What column would you like your " << ship << " to start in (0-9)? ";
					cin >> col;
					cin.ignore();
				} while (col < 0 || col > 9);

				rowNum = row - 65;

                //tests is the space is used 
				for (int i = 0; i < shipLengthInd; i++)
				{
					if (ary[rowNum][col + i] != 'O' || col + i >= 10)
					{
						system("cls");
						screen(ary, compAry, pSLeft, cSLeft);

						cout << "\nThe entered horisontal space " << row << col << " is invaled.\n\n";
						usedSpace = true;
					}
				}

				if (!(usedSpace))
				{
					for (int i = 0; i < shipLengthInd; i++)
					{
						ary[rowNum][i + col] = numShip;
					}
					numShip += 1;
				}

			}

            //gets exact starting location of the ship
			else {
				do {
					cout << "What row would you like your " << ship << " to start in (A-J)? ";
					cin >> row;
					cin.ignore();
					row = toupper(row);
				} while (row < 'A' || row > 'J');

				do {
					cout << "What collum would you like to put your " << ship << " in (0-9)? ";
					cin >> col;
					cin.ignore();
				} while (col < 0 || col > 9);

				rowNum = row - 65;

				for (int i = 0; i < shipLengthInd; i++)
				{
					if (ary[rowNum + i][col] != 'O' || rowNum + i >= 10)
					{
						system("cls");
						screen(ary, compAry, pSLeft, cSLeft);

						cout << "\nThe entered vertical space " << row << col << " is invaled.\n\n";
						usedSpace = true;
					}
					//rowNum += 1;

				}

				rowNum = row - 65;

				if (!(usedSpace))
				{
					for (int i = 0; i < shipLengthInd; i++)
					{
						ary[rowNum + i][col] = numShip;
					}
					numShip++;
				}
			}

		} while (usedSpace);
	}

	system("cls");
	screen(ary, compAry, pSLeft, cSLeft);

}

