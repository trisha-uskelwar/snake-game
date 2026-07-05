#include <iostream>
#include <windows.h>
#include <conio.h>
#include <time.h>

using namespace std;

bool gameOver;

const int width = 20;
const int height = 20;

int x, y, fruitX, fruitY, score;

int tailX[100], tailY[100];
int nTail;

enum eDirection { STOP = 0, LEFT, RIGHT, UP, DOWN };
eDirection dir;

// Move cursor to top-left (prevents flicker)
void gotoxy(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void Setup() {
    gameOver = false;
    dir = STOP;

    x = width / 2;
    y = height / 2;

    fruitX = rand() % width;
    fruitY = rand() % height;

    score = 0;
    nTail = 0;
}

void Draw() {
    gotoxy(0, 0); // instead of clearing screen

    // Top border
    for (int i = 0; i < width + 2; i++)
        cout << "#";
    cout << endl;

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {

            if (j == 0)
                cout << "#";

            bool printed = false;

            // Print snake head
            if (i == y && j == x) {
                cout << "O";
                printed = true;
            }
            else if (i == fruitY && j == fruitX) {
                cout << "F";
                printed = true;
            }
            else {
                // Print tail
                for (int k = 0; k < nTail; k++) {
                    if (tailX[k] == j && tailY[k] == i) {
                        cout << "o";
                        printed = true;
                        break;
                    }
                }
            }

            if (!printed)
                cout << " ";

            if (j == width - 1)
                cout << "#";
        }
        cout << endl;
    }

    // Bottom border
    for (int i = 0; i < width + 2; i++)
        cout << "#";
    cout << endl;

    cout << "Score: " << score << endl;
}

void Input() {
    if (_kbhit()) {
        switch (_getch()) {
        case 'a': dir = LEFT; break;
        case 'd': dir = RIGHT; break;
        case 'w': dir = UP; break;
        case 's': dir = DOWN; break;
        case 'x': gameOver = true; break;
        }
    }
}

void Logic() {
    // Save previous position of head
    int prevX = x;
    int prevY = y;
    int prev2X, prev2Y;

    // Move body
    for (int i = 0; i < nTail; i++) {
        prev2X = tailX[i];
        prev2Y = tailY[i];
        tailX[i] = prevX;
        tailY[i] = prevY;
        prevX = prev2X;
        prevY = prev2Y;
    }

    // Move head
    switch (dir) {
    case LEFT: x--; break;
    case RIGHT: x++; break;
    case UP: y--; break;
    case DOWN: y++; break;
    default: break;
    }

    // Wall collision
    if (x >= width || x < 0 || y >= height || y < 0)
        gameOver = true;

    // Self collision
    for (int i = 0; i < nTail; i++) {
        if (tailX[i] == x && tailY[i] == y)
            gameOver = true;
    }

    // Eat fruit
    if (x == fruitX && y == fruitY) {
        score += 10;
        fruitX = rand() % width;
        fruitY = rand() % height;
        nTail++;
    }
}

int main() {
    srand(time(NULL));
    Setup();

    // Hide cursor (optional but smoother)
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    cursorInfo.bVisible = false;
    cursorInfo.dwSize = 1;
    SetConsoleCursorInfo(hConsole, &cursorInfo);

    while (!gameOver) {
        Draw();
        Input();
        Logic();
        Sleep(80); // smooth speed
    }

    return 0;
}