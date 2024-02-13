/*
    - HiLCoE college bridging program(2023)
    - Course: Computer programming I (CS530)

    -project title: Sudoku game

    -project participants
        * Helina Mogesse
        * Israel Kebede
        * Mathewos Kebede
        * Mesfin Belay
    -Project advissor
        * Ashenafi Fassil
*/

#include <iostream>
#include <random>
#include <ctime>
#include <string>

using namespace std;

bool game();
int gameplay(int[9][9]);
string inputCheck(string); //checks input and filters characters.
void printFrontPage();//prints the introduction page with the dificulty level.
void sdkPrt(int[9][9][2]);//Prints the Sudoku table.
void sudokuGenerator(int[9][9]);//Generates a complete Sudoku solution.
void sodukoPlacer(char, int[9][9]);//Removes numbers from a complete grid to create a playable puzzle.
int* posGen(char);//Generates random positions for number removal based on difficulty.
bool checkRep(int, int*, int);//Checks for repeated numbers in an array.
bool checkPlacer(int[9][9], int, int, int);//Check number validation during generation.
bool checkPlayer(int[9][9][2], int, int, int);//Checks if a player's input is valid.

/*
    main- a function that Controls
        *the game loop,
        *handling difficulty selection,
        *input validation,
        *grid display, and
        *win/loss conditions.
*/

int main()
{
    do {
    } while (game());
    cout << "\n\t\t\"Thank you for playing.\"\n";
    return 0;
}
bool game() {
    char play;
    system("cls");
    int sudokuInitial[9][9] = { 0 };
    char lvl;

    printFrontPage();
    cout << "\n\n\tPlease input the difficulty level you want to play or 'x' to quit: ";

    do {
        cin >> lvl;
        cin.ignore();

        if (lvl == 'x' || lvl == 'X') {
            return 0;
        }
        else if (lvl != '1' && lvl != '2' && lvl != '3') {
            system("cls");
            printFrontPage();
            cout << "\t\033[31mIncorrect input please select only from the option listed.\033[0m";
            cout << "\n\n\tPlease input the difficulty level you want to play or 'x' to quit: ";
        }
        else
            break;
    } while (true);

    //calls sudukoPlacer which fills the table based on the level into sudukuinitial
    sodukoPlacer(lvl, sudokuInitial);

    if (gameplay(sudokuInitial) == 0)
        return false;

    cout << "\n\t\tCongratulation You have Finished the game successfully.\n\n";
    cout << "\tIf you want to play again Press 'Y' or anything else to exit: ";
    cin >> play;
    return (play == 'y' || play == 'Y');
}

int gameplay(int sudokuInitial[9][9]) {
    int sudokuPlayer[9][9][2] = { 0 };
    system("cls");

    /*
        copies the origional generated sudoku array to another array
        so that the origional will not be alterd during game play
    */
    for (int i = 0; i < 9; i++)
        for (int j = 0; j < 9; j++) {
            sudokuPlayer[i][j][0] = sudokuInitial[i][j];
            if (sudokuInitial[i][j] > 0)
                sudokuPlayer[i][j][1] = 1;
            else
                sudokuPlayer[i][j][1] = 0;
        }

    int inloop = 0;
    cout << "\n\t\tGood Luck!! You can start\n\n";
    cout << "\tYou have to fill the empty spaces to Finish the game!\n";

    sdkPrt(sudokuPlayer);   //prints the sudoku player onto the console
    do {
        string input;
        do {
            cout << "'z' to reset the table or 'x' to quit"
                << "\n\nPlease input the coordinate followed by the value ";

            string ans;
            getline(cin, ans);


            if (ans[0] == 'z' || ans[0] == 'Z') {
                gameplay(sudokuInitial);
                return 0;
            }
            else if (ans[0] == 'x' || ans[0] == 'X') {
                game();
                return 0;
            }

            input = inputCheck(ans);

            if (input[0] == '0') {
                system("cls");
                cout << "\033[31m\n\n\t\tIncorrect input. Try again\n\n\033[0m";
                sdkPrt(sudokuPlayer);
            }
            else
                break;
        } while (true);

        int m = input[0] - 'a';
        int n = input[1] - 'j';
        int val = input[2] - '0';

        system("cls");

        if (sudokuPlayer[m][n][1] > 0)
            cout << "\033[31m\n\tThe location is already filled by the game. Try again\n\n\033[0m";
        else if (checkPlayer(sudokuPlayer, m, n, val)) {
            sudokuPlayer[m][n][0] = val;
            sudokuPlayer[m][n][1] = 0;
            cout << "\033[32m\n\tInput: " << val
                << " at position (" << input[0] << "," << input[1] << ")\n\n\033[0m";
        }
        else {
            sudokuPlayer[m][n][0] = val;
            if (val == 0) {
                sudokuPlayer[m][n][1] = 0;
                cout << "\033[32m\n\tPosition (" << input[0] << "," << input[1]
                    << ") is cleared\n\n\033[0m";
            }
            else {
                sudokuPlayer[m][n][1] = -1;
                cout << "\033[31m\n\t" << val << " is not the correct value for position ("
                    << input[0] << "," << input[1] << ")\n\n\033[0m";
            }
        }

        inloop = 1;
        bool redNum = false;
        for (int i = 0; i < 9; i++)
            for (int j = 0; j < 9; j++)
                if (sudokuPlayer[i][j][0] == 0 || sudokuPlayer[i][j][1] < 0) {
                    if (sudokuPlayer[i][j][1] < 0) {
                        cout << "\033[31m\tYou have to fill the empty spaces and the red numbers!\n\033[0m";
                        redNum = true;
                        i = 9;
                        inloop = 0;
                        break;
                    }
                    inloop = 0;
                }
        if (redNum != true)
            cout << "\tYou have to fill the empty spaces to Finish the game!\n";

        sdkPrt(sudokuPlayer);

    } while (inloop == 0);
    return 1;
}

/*
    inputCheck - checkes wether the user input has both coordinate and the value
    converts the user input into the correct format (e.g., "aj8").
    Returns a string with the validated input or "0" if invalid.
*/

string inputCheck(string str)
{
    bool x = false, y = false, v = false;
    char input[4] = { '0' };


    for (char ch : str) {
        if (ch >= 'a' && ch <= 'i' && y == false) {
            input[0] = ch;
            y = true;
        }
        else if (ch >= 'j' && ch <= 'r' && x == false) {
            input[1] = ch;
            x = true;
        }
        else if (ch >= '0' && ch <= '9' && v == false) {
            input[2] = ch;
            v = true;
        }
    }
    if (x == false || y == false || v == false)
        input[0] = '0';
    return (input);
}

/*
    sudokuGenerator - Generates a complete Sudoku solution.
    uses random_device from <random> library to generate a true random object --rd
    random_device when used repeatedly decreases entropy, so we used additional pseudorandom number generator
    mersenne twist (mt19937)  that accepts random number generated from rd and generates pseudorandom number
    and uniform_int_distribution to give equal probabilities for the numbers 1-9 to be generated equally
*/

void sudokuGenerator(int sudokuFull[9][9])
{
    int count = 0;

    for (int i = 0; i < 9; i++)
        for (int j = 0; j < 9; j++) {
            int y = 0;
            do {
                random_device rd;
                mt19937 gen(rd());
                uniform_int_distribution<>dis(1, 9);
                int x = dis(gen);

                count++;
                if (checkPlacer(sudokuFull, i, j, x)) {
                    y = 1, count = 0;
                    sudokuFull[i][j] = x;
                }
                if (count > 35)
                    i = 0, j = 0;
            } while (y == 0);
        }
}

/*
      posGen-   Generates random positions for number removal based on difficulty.
      uses rand() which is a pseudorandom number generator,
      to make the random number more variable(close to true random) we used time as the initial value for the rand() generator.
*/

int* posGen(char lvl)
{
    srand(time(NULL));
    int numPos = 0;
    if (lvl == '1')
        numPos = rand() % 10 + 22;
    else if (lvl == '2')
        numPos = rand() % 10 + 35;
    else if (lvl == '3')
        numPos = rand() % 10 + 50;

    int x;
    int* arr = 0;
    delete[] arr;
    arr = new int[numPos];
    for (int i = 0; i < numPos;) {
        x = rand() % 81 + 1;
        if (checkRep(i, arr, x)) {
            arr[i] = x;
            i++;
        }
    }
    for (int i = 0; i < numPos - 1; i++)
        for (int j = 0; j < numPos - 1 - i; j++) {
            int lrg = arr[j];
            if (arr[j] > arr[j + 1]) {
                arr[j] = arr[j + 1];
                arr[j + 1] = lrg;
            }
        }

    return arr;
}

/*
    sodukoPlacer- Removes numbers from a complete grid to create a playable puzzle
    uses sudokuGenerator and posGen to delete certain values from the generated sudoku array based on the level
*/

void sodukoPlacer(char lvl, int sudokuRefer[9][9])
{
    sudokuGenerator(sudokuRefer);
    int* pos = posGen(lvl);
    int n = 0;
    for (int i = 0; i < 9; i++)
        for (int j = 0; j < 9; j++)
            if (i == (pos[n] - 1) / 9 && j == (pos[n] - 1) % 9) {
                sudokuRefer[i][j] = 0;
                n++;
            }
    delete[] pos;
}

/*
    -checkRep: Checks for repeated numbers in an array.
    - It works in the generation of array places that are for deletion based on level size
*/

bool checkRep(int i, int* arr, int x)
{
    for (int j = 0; j < i; j++)
        if (x == arr[j])
            return 0;
    return 1;
}

/*
    - Check number validation during game-play.
    - it checks if a value is repeated on the same raw and column
    - it also checks if the value is repeated in the same nonant
    - checks the whole table
*/

bool checkPlayer(int sdkArr[9][9][2], int m, int n, int v)
{
    for (int i = 0; i < 9; i++)
        if (sdkArr[i][n][0] == v && i != m)
            return 0;
    for (int i = 0; i < 9; i++)
        if (sdkArr[m][i][0] == v && i != n)
            return 0;
    int a = m / 3, b = n / 3;
    for (int i = a * 3; i < 3 + (a * 3); i++)
        for (int j = b * 3; j < 3 + (b * 3); j++) {
            if (j == n || i == m)
                continue;
            if (sdkArr[i][j][0] == v)
                return 0;
        }
    return 1;
}

/*
    - Check number validation during generation.
    - it checks if a value is repeated on the same raw and column
    - it also checks if the value is repeated in the same nonant
    - checks only up to the currently being filled coordinate(to minimize unnecessary processing
    (sudoku table is segmented into nine 3*3 square tables, each 3*3 segment is called nonant)
*/

bool checkPlacer(int sdkArr[9][9], int m, int n, int v)
{
    for (int i = 0; i < m; i++)
        if (sdkArr[i][n] == v)
            return 0;
    for (int i = 0; i < n; i++)
        if (sdkArr[m][i] == v)
            return 0;
    int a = m / 3, b = n / 3;
    for (int i = a * 3; i < m; i++)
        for (int j = b * 3; j < 3 + (b * 3); j++) {
            if (j == n)
                continue;
            if (sdkArr[i][j] == v)
                return 0;
        }
    return 1;
}

/*
    -sdkPrt: prints the sudoku table
    -It gives foreground green colors for generated(game origin numbers)
    -For player originated numbers, it gives yellow and red for correct and incorrect values for the position respectively.
*/

void sdkPrt(int sdkArr[9][9][2])
{
    char p[9][9] = { " " };
    for (int i = 0; i < 9; i++)
        for (int j = 0; j < 9; j++)
            p[i][j] = sdkArr[i][j][0] == 0 ? ' ' : sdkArr[i][j][0] + '0';
    int col;

    cout << "     j       k       l        m       n       o        p       q       r\n"
        << "  _________________________________________________________________________\n";

    for (int i = 0; i < 9; i++) {
        cout << " |       " << "|" << "       " << "|" << "       " << "||"
            << "       " << "|" << "       " << "|" << "       " << "||"
            << "       " << "|" << "       " << "|" << "       " << "|" << endl
            << char('a' + i) << "|";
        for (int j = 0; j < 9; j++) {
            if (sdkArr[i][j][1] > 0)
                col = 32;
            else if (sdkArr[i][j][1] == 0)
                col = 33;
            else
                col = 31;
            cout << "   " << "\033[" << col << ";1m" << p[i][j] << "\033[0m" << "   " << "|";
            if (j == 2 || j == 5)
                cout << "|";
            else if (j == 8)
                cout << endl;
        }
        if (i == 2) {
            cout << " |       " << "|" << "       " << "|" << "       " << "||"
                << "       " << "|" << "       " << "|" << "       " << "||"
                << "       " << "|" << "       " << "|" << "       " << "|" << endl
                << " |=======================||================================================|\n";
        }
        else if (i == 5) {
            cout << " |       " << "|" << "       " << "|" << "       " << "||"
                << "       " << "|" << "       " << "|" << "       " << "||"
                << "       " << "|" << "       " << "|" << "       " << "|" << endl
                << " |================================================||=======================|\n";
        }
        else
            cout << " |_______|_______|_______||_______|_______|_______||_______|_______|_______|\n";
    }
}

void printFrontPage()
{
    cout << "\n\n\n\n\n"
        << "    ****     **    **    ******        ****      **     **   **    **\n"
        << "   ******    **    **    *******      ******     **    **    **    **\n"
        << "  **    **   **    **    **    **    **    **    **   **     **    **\n"
        << "  **         **    **    **    **    **    **    **  **      **    **\n"
        << "   *****     **    **    **    **    **    **    ** **       **    **\n"
        << "    *****    **    **    **    **    **    **    *****       **    **\n"
        << "        **   **    **    **    **    **    **    *** **      **    **\n"
        << "  **    **   **    **    **    **    **    **    **   **     **    **\n"
        << "   ******     ******     *******      ******     **    **     ******\n"
        << "    ****       ****      ******        ****      **     **     ****\n"
        << "\n\n\n\t\t\t\t1-Easy\n\t\t\t\t2-Medium\n\t\t\t\t3-Hard\n\n";
}