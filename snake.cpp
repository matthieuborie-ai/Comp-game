#include <iostream>
#include <vector>
#include <conio.h>    // _kbhit, _getch  Windows only
#include <windows.h>  // Sleep, system   Windows only
#include <cstdlib>
#include <ctime>

using namespace std;

const int WIDTH = 30;
const int HEIGHT = 20;

enum Direction {
    STOP = 0,
    LEFT_DIR,
    RIGHT_DIR,
    UP_DIR,
    DOWN_DIR
};

struct Position {
    int x;
    int y;

    bool operator==(const Position& other) const {
        return x == other.x && y == other.y;
    }
};

bool gameOver;
Direction dir;
vector<Position> snake;
Position food;
int score;
int speedMs = 120; // lower is faster

void Setup() {
    gameOver = false;
    dir = STOP;
    snake.clear();

    // Start snake in the center
    Position start;
    start.x = WIDTH / 2;
    start.y = HEIGHT / 2;
    snake.push_back(start);

    score = 0;
    srand(static_cast<unsigned int>(time(nullptr)));

    food.x = rand() % WIDTH;
    food.y = rand() % HEIGHT;
}

void Draw() {
    // Clear the console
    system("cls");

    cout << "Snake Game in C plus plus\n";
    cout << "Controls W A S D to move  X to quit\n";
    cout << "Score " << score << "\n\n";

    // Top wall
    for (int i = 0; i < WIDTH + 2; i++) {
        cout << "#";
    }
    cout << "\n";

    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            if (x == 0) {
                cout << "#"; // left wall
            }

            Position current{ x, y };
            if (snake[0] == current) {
                cout << "O"; // head
            } else if (food == current) {
                cout << "F"; // food
            } else {
                bool printBody = false;
                for (size_t i = 1; i < snake.size(); i++) {
                    if (snake[i] == current) {
                        cout << "o"; // body
                        printBody = true;
                        break;
                    }
                }

                if (!printBody) {
                    cout << " ";
                }
            }

            if (x == WIDTH - 1) {
                cout << "#"; // right wall
            }
        }
        cout << "\n";
    }

    // Bottom wall
    for (int i = 0; i < WIDTH + 2; i++) {
        cout << "#";
    }
    cout << "\n";
}

void Input() {
    if (_kbhit()) {
        char key = _getch();
        switch (key) {
            case 'a':
            case 'A':
                if (dir != RIGHT_DIR) dir = LEFT_DIR;
                break;
            case 'd':
            case 'D':
                if (dir != LEFT_DIR) dir = RIGHT_DIR;
                break;
            case 'w':
            case 'W':
                if (dir != DOWN_DIR) dir = UP_DIR;
                break;
            case 's':
            case 'S':
                if (dir != UP_DIR) dir = DOWN_DIR;
                break;
            case 'x':
            case 'X':
                gameOver = true;
                break;
            default:
                break;
        }
    }
}

void GenerateFood() {
    while (true) {
        food.x = rand() % WIDTH;
        food.y = rand() % HEIGHT;

        bool onSnake = false;
        for (const auto& part : snake) {
            if (part == food) {
                onSnake = true;
                break;
            }
        }

        if (!onSnake) {
            break;
        }
    }
}

void Logic() {
    if (dir == STOP) {
        return;
    }

    // Current head position
    Position head = snake[0];

    // Move head
    switch (dir) {
        case LEFT_DIR:
            head.x--;
            break;
        case RIGHT_DIR:
            head.x++;
            break;
        case UP_DIR:
            head.y--;
            break;
        case DOWN_DIR:
            head.y++;
            break;
        default:
            break;
    }

    // Check wall collision
    if (head.x < 0 || head.x >= WIDTH || head.y < 0 || head.y >= HEIGHT) {
        gameOver = true;
        return;
    }

    // Check self collision
    for (size_t i = 0; i < snake.size(); i++) {
        if (snake[i] == head) {
            gameOver = true;
            return;
        }
    }

    // Insert new head
    snake.insert(snake.begin(), head);

    // Check food
    if (head == food) {
        score += 10;

        // Speed up slightly
        if (speedMs > 40) {
            speedMs -= 2;
        }

        GenerateFood();
    } else {
        // Remove tail segment
        snake.pop_back();
    }
}

int main() {
    Setup();

    while (!gameOver) {
        Draw();
        Input();
        Logic();
        Sleep(speedMs); // milliseconds
    }

    system("cls");
    cout << "Game Over\n";
    cout << "Final Score " << score << "\n";
    cout << "Press any key to exit\n";
    _getch();

    return 0;
}
