#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

using namespace std;

const int WIDTH = 20;
const int HEIGHT = 10;

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

bool gameOverFlag;
Direction dir;
vector<Position> snake;
Position food;
int scoreVal;

// Simple helper to clear screen for online compilers
void ClearScreen() {
    // Print many newlines so the old board scrolls away
    for (int i = 0; i < 30; i++) {
        cout << "\n";
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

void Setup() {
    gameOverFlag = false;
    dir = STOP;
    snake.clear();

    Position start;
    start.x = WIDTH / 2;
    start.y = HEIGHT / 2;
    snake.push_back(start);

    scoreVal = 0;
    srand(static_cast<unsigned int>(time(nullptr)));

    GenerateFood();
}

void Draw() {
    ClearScreen();

    cout << "Simple Snake Game C plus plus\n";
    cout << "Use W A S D then Enter to move   Q then Enter to quit\n";
    cout << "Score " << scoreVal << "\n\n";

    // top border
    for (int i = 0; i < WIDTH + 2; i++) {
        cout << "#";
    }
    cout << "\n";

    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            if (x == 0) {
                cout << "#";
            }

            Position current{ x, y };
            if (snake[0] == current) {
                cout << "O"; // head
            } else if (food == current) {
                cout << "F"; // food
            } else {
                bool bodyPrinted = false;
                for (size_t i = 1; i < snake.size(); i++) {
                    if (snake[i] == current) {
                        cout << "o";
                        bodyPrinted = true;
                        break;
                    }
                }
                if (!bodyPrinted) {
                    cout << " ";
                }
            }

            if (x == WIDTH - 1) {
                cout << "#";
            }
        }
        cout << "\n";
    }

    // bottom border
    for (int i = 0; i < WIDTH + 2; i++) {
        cout << "#";
    }
    cout << "\n";
}

void Input() {
    cout << "Next move W A S D or Q to quit  then press Enter  ";
    char key;
    cin >> key;

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
        case 'q':
        case 'Q':
            gameOverFlag = true;
            break;
        default:
            // keep current direction if input is invalid
            break;
    }
}

void Logic() {
    if (dir == STOP) {
        // before first move   do nothing
        return;
    }

    Position head = snake[0];

    // move head
    switch (dir) {
        case LEFT_DIR:
            head.x -= 1;
            break;
        case RIGHT_DIR:
            head.x += 1;
            break;
        case UP_DIR:
            head.y -= 1;
            break;
        case DOWN_DIR:
            head.y += 1;
            break;
        default:
            break;
    }

    // wall collision
    if (head.x < 0 || head.x >= WIDTH || head.y < 0 || head.y >= HEIGHT) {
        gameOverFlag = true;
        return;
    }

    // self collision
    for (size_t i = 0; i < snake.size(); i++) {
        if (snake[i] == head) {
            gameOverFlag = true;
            return;
        }
    }

    // insert new head
    snake.insert(snake.begin(), head);

    // check food
    if (head == food) {
        scoreVal += 10;
        GenerateFood();
    } else {
        // remove tail
        snake.pop_back();
    }
}

int main() {
    Setup();

    while (!gameOverFlag) {
        Draw();
        Input();
        Logic();
    }

    ClearScreen();
    cout << "Game Over\n";
    cout << "Final score " << scoreVal << "\n";
    return 0;
}
