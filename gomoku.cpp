#include<iostream>
#include<windows.h>
#include<conio.h>
#include<ctime>
using namespace std;

#define MAXCOL 100
#define MAXROW 100

void gotoRowCol(int rpos, int cpos)
{
    COORD scrn;
    HANDLE hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
    scrn.X = cpos;
    scrn.Y = rpos;
    SetConsoleCursorPosition(hOutput, scrn);
}

void print_box(int sr, int sc, int re, int ce)
{
    char sym = (char)219; // Block character (solid box)

    for (int r = 0; r < re; r++)
    {
        for (int c = 0; c < ce; c++)
        {
            if (r == 0 || r == re - 1 || c == 0 || c == ce - 1)
            {
                gotoRowCol(sr + r, sc + c);
                cout << sym;
            }
        }
    }
}

void color(int k)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, k);
}

void displaymessage(char NAME[], char SYMBOL)
{
    color(14);
    cout << NAME << " (" << SYMBOL << ") Turn: ";
    color(7);
}

void getRowColbyLeftClick(int& rpos, int& cpos)
{
    HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);
    DWORD Events;
    INPUT_RECORD InputRecord;
    SetConsoleMode(hInput, ENABLE_PROCESSED_INPUT | ENABLE_MOUSE_INPUT |
        ENABLE_EXTENDED_FLAGS);
    do
    {
        ReadConsoleInput(hInput, &InputRecord, 1, &Events);
        if (InputRecord.EventType == MOUSE_EVENT &&
            InputRecord.Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED)
        {
            cpos = InputRecord.Event.MouseEvent.dwMousePosition.X;
            rpos = InputRecord.Event.MouseEvent.dwMousePosition.Y;
            break;
        }
    } while (true);
}

void selection_of_postion(int& r, int& c)
{
    getRowColbyLeftClick(r, c);
}

void turn_change(int& turn, int NOP)
{
    turn = (turn + 1) % NOP;
}

void display_board(char b[][MAXCOL], int R, int C)
{
    for (int ri = 0; ri < R; ri++)
    {
        for (int ci = 0; ci < C; ci++)
        {
            if (b[ri][ci] == 'X')      color(12); // Red for X
            else if (b[ri][ci] == 'O') color(11); // Cyan for O
            else                        color(8);  // Dark gray for empty

            cout << b[ri][ci];
        }
        color(7);
        cout << endl;
    }
}

void update_board(char b[][MAXCOL], int r, int c, char SYMBOL)
{
    b[r][c] = SYMBOL;
}

// FIX: Validity check now also ensures clicked cell matches a board cell (1:1 col mapping)
bool validity(char b[][MAXCOL], int r, int c, int R, int C)
{
    if (r < 0 || r >= R || c < 0 || c >= C)
    {
        return false;
    }
    return true;
}

void initialization(char b[][MAXCOL], char P_sym[], char P_names[][MAXCOL],
    int& NOP, int& turn, int& WIN_COUNT, int& R, int& C)
{
    color(14);
    cout << "\n  ============================" << endl;
    cout << "       GOMOKU - CONSOLE GAME   " << endl;
    cout << "  ============================" << endl;
    color(12);
    cout << "\n  Enter WIN_COUNT ROWS COLUMNS: ";
    color(7);
    cin >> WIN_COUNT >> R >> C;

    // Clamp board size to safe limits
    if (R > MAXROW) R = MAXROW;
    if (C > MAXCOL) C = MAXCOL;
    if (WIN_COUNT < 2) WIN_COUNT = 2;

    NOP = 2;

    // Player 1: Human
    P_names[0][0] = 'H'; P_names[0][1] = 'u'; P_names[0][2] = 'm';
    P_names[0][3] = 'a'; P_names[0][4] = 'n'; P_names[0][5] = '\0';

    // Player 2: Computer
    P_names[1][0] = 'C'; P_names[1][1] = 'o'; P_names[1][2] = 'm';
    P_names[1][3] = 'p'; P_names[1][4] = 'u'; P_names[1][5] = 't';
    P_names[1][6] = 'e'; P_names[1][7] = 'r'; P_names[1][8] = '\0';

    P_sym[0] = 'X';
    P_sym[1] = 'O';

    turn = rand() % 2;

    for (int ri = 0; ri < R; ri++)
        for (int ci = 0; ci < C; ci++)
            b[ri][ci] = '-';

    system("cls");
}

bool is_draw(char b[][MAXCOL], int R, int C)
{
    for (int ri = 0; ri < R; ri++)
        for (int ci = 0; ci < C; ci++)
            if (b[ri][ci] == '-')
                return false;
    return true;
}

// ---------------------------------------------------------------
// WIN-CHECK FUNCTIONS  (all boundary bugs fixed)
// ---------------------------------------------------------------

bool h_right(char b[][MAXCOL], int WIN_COUNT, int R, int C, int r, int c, char P_sym)
{
    // Need c + WIN_COUNT - 1 < C  =>  c + WIN_COUNT <= C
    if (c + WIN_COUNT > C) return false;
    for (int i = 0; i < WIN_COUNT; i++)
        if (b[r][c + i] != P_sym) return false;
    return true;
}

// FIX: was (c - WIN_COUNT < 0) which is off-by-one.
// Starting at c going left WIN_COUNT steps: last index = c-(WIN_COUNT-1)
// Must be >= 0  =>  c >= WIN_COUNT - 1
bool h_left(char b[][MAXCOL], int WIN_COUNT, int R, int C, int r, int c, char P_sym)
{
    if (c - (WIN_COUNT - 1) < 0) return false;
    for (int i = 0; i < WIN_COUNT; i++)
        if (b[r][c - i] != P_sym) return false;
    return true;
}

// FIX: was (r - WIN_COUNT < 0) -- off-by-one
// Starting at r going up WIN_COUNT steps: last index = r-(WIN_COUNT-1) >= 0
bool v_up(char b[][MAXCOL], int WIN_COUNT, int R, int C, int r, int c, char P_sym)
{
    if (r - (WIN_COUNT - 1) < 0) return false;
    for (int i = 0; i < WIN_COUNT; i++)
        if (b[r - i][c] != P_sym) return false;
    return true;
}

bool v_down(char b[][MAXCOL], int WIN_COUNT, int R, int C, int r, int c, char P_sym)
{
    if (r + WIN_COUNT > R) return false;
    for (int i = 0; i < WIN_COUNT; i++)
        if (b[r + i][c] != P_sym) return false;
    return true;
}

bool d_ldown(char b[][MAXCOL], int WIN_COUNT, int R, int C, int r, int c, char P_sym)
{
    if (r + WIN_COUNT > R || c + WIN_COUNT > C) return false;
    for (int i = 0; i < WIN_COUNT; i++)
        if (b[r + i][c + i] != P_sym) return false;
    return true;
}

// FIX: was (c - WIN_COUNT < -1) -- wrong comparison
// Last col index = c-(WIN_COUNT-1) must be >= 0
bool d_rdown(char b[][MAXCOL], int WIN_COUNT, int R, int C, int r, int c, char P_sym)
{
    if (r + WIN_COUNT > R || c - (WIN_COUNT - 1) < 0) return false;
    for (int i = 0; i < WIN_COUNT; i++)
        if (b[r + i][c - i] != P_sym) return false;
    return true;
}

// FIX: diagonal up-left: r-(WIN_COUNT-1)>=0 AND c+(WIN_COUNT-1)<C
bool d_lup(char b[][MAXCOL], int WIN_COUNT, int R, int C, int r, int c, char P_sym)
{
    if (r - (WIN_COUNT - 1) < 0 || c + (WIN_COUNT - 1) >= C) return false;
    for (int i = 0; i < WIN_COUNT; i++)
        if (b[r - i][c + i] != P_sym) return false;
    return true;
}

// FIX: was (c - WIN_COUNT < -1) -- wrong comparison
bool d_rup(char b[][MAXCOL], int WIN_COUNT, int R, int C, int r, int c, char P_sym)
{
    if (r - (WIN_COUNT - 1) < 0 || c - (WIN_COUNT - 1) < 0) return false;
    for (int i = 0; i < WIN_COUNT; i++)
        if (b[r - i][c - i] != P_sym) return false;
    return true;
}

bool winning(char b[][MAXCOL], int WIN_COUNT, int R, int C, int r, int c, char P_sym)
{
    return (
        h_right(b, WIN_COUNT, R, C, r, c, P_sym) ||
        h_left (b, WIN_COUNT, R, C, r, c, P_sym) ||
        v_up   (b, WIN_COUNT, R, C, r, c, P_sym) ||
        v_down (b, WIN_COUNT, R, C, r, c, P_sym) ||
        d_ldown(b, WIN_COUNT, R, C, r, c, P_sym) ||
        d_rdown(b, WIN_COUNT, R, C, r, c, P_sym) ||
        d_rup  (b, WIN_COUNT, R, C, r, c, P_sym) ||
        d_lup  (b, WIN_COUNT, R, C, r, c, P_sym)
    );
}

// ---------------------------------------------------------------
// IMPROVED AI: Win > Block > Center > First Empty
// ---------------------------------------------------------------
void computer_move(char b[][MAXCOL], int R, int C, int WIN_COUNT,
    char P_sym[], int turn, int& Sr, int& Sc)
{
    int humanTurn = (turn + 1) % 2; // FIX: compute opponent index dynamically

    // Priority 1: Win if possible
    for (int i = 0; i < R; i++)
        for (int j = 0; j < C; j++)
            if (b[i][j] == '-')
            {
                b[i][j] = P_sym[turn];
                if (winning(b, WIN_COUNT, R, C, i, j, P_sym[turn]))
                {
                    b[i][j] = '-';
                    Sr = i; Sc = j; return;
                }
                b[i][j] = '-';
            }

    // Priority 2: Block human from winning
    for (int i = 0; i < R; i++)
        for (int j = 0; j < C; j++)
            if (b[i][j] == '-')
            {
                b[i][j] = P_sym[humanTurn];
                if (winning(b, WIN_COUNT, R, C, i, j, P_sym[humanTurn]))
                {
                    b[i][j] = '-';
                    Sr = i; Sc = j; return;
                }
                b[i][j] = '-';
            }

    // Priority 3: Take center if free
    int cr = R / 2, cc = C / 2;
    if (b[cr][cc] == '-') { Sr = cr; Sc = cc; return; }

    // Priority 4: First available cell
    for (int i = 0; i < R; i++)
        for (int j = 0; j < C; j++)
            if (b[i][j] == '-') { Sr = i; Sc = j; return; }
}

void GAMELOOP(int& NOP, int& turn, int& WinCont, int& Sr, int& Sc,
    int& R, int& C, char b[][MAXCOL], char P_sym[], char P_names[][MAXCOL])
{
    initialization(b, P_sym, P_names, NOP, turn, WinCont, R, C);

    display_board(b, R, C);

    do
    {
        // Show whose turn it is below the board
        gotoRowCol(R + 1, 0);
        displaymessage(P_names[turn], P_sym[turn]);

        if (turn == 0) // Human
        {
            do
            {
                selection_of_postion(Sr, Sc);
            } while (!validity(b, Sr, Sc, R, C) || b[Sr][Sc] != '-');
        }
        else // Computer
        {
            Sleep(500); // Brief pause so the player can see the AI "thinking"
            computer_move(b, R, C, WinCont, P_sym, turn, Sr, Sc);
        }

        update_board(b, Sr, Sc, P_sym[turn]);

        system("cls");
        display_board(b, R, C);

        if (winning(b, WinCont, R, C, Sr, Sc, P_sym[turn]))
        {
            system("cls");
            color(9);
            print_box(3, 5, 7, 36);
            gotoRowCol(5, 10);
            color(12);
            cout << P_names[turn] << " (" << P_sym[turn] << ") is the Winner!";
            color(7);
            break;
        }

        if (is_draw(b, R, C))
        {
            system("cls");
            color(9);
            // FIX: draw box coordinates corrected so text fits inside
            print_box(3, 5, 7, 24);
            gotoRowCol(6, 10);
            color(14);
            cout << "  GAME IS A DRAW!";
            color(7);
            break;
        }

        turn_change(turn, NOP);

    } while (true);
}

int main()
{
    srand((unsigned int)time(0));
    int NOP, Turn, WinCont, Sr, Sc, R, C;

    char b[MAXROW][MAXCOL], P_sym[MAXROW], P_names[MAXROW][MAXCOL];

    GAMELOOP(NOP, Turn, WinCont, Sr, Sc, R, C, b, P_sym, P_names);

    gotoRowCol(15, 5);
    color(7);
    cout << "\n  Press any key to exit...";
    _getch();
    return 0;
}
