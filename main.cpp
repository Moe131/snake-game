#include <iostream>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <chrono>
#include <thread>
#include <vector>
#include <cstdlib> 

using namespace std;

const int WIDTH = 50, HEIGHT = 25;
const int VERTICAL_SPEED = 200;   // Speed for vertical movement (in ms)
const int HORIZONTAL_SPEED = 100; // Speed for horizontal movement (in ms)

struct SnakeSegment {
    int x, y;
};

vector<SnakeSegment> snake; // Snake body
int foodX, foodY;
string movement = "UP";
bool gameOver = false;

void placeFood() {
    bool validFoodPosition = false;
    while (!validFoodPosition) {
        // Randomly place food within bounds
        foodX = rand() % (HEIGHT - 2) + 1; // Avoid placing food on the border
        foodY = rand() % (WIDTH - 2) + 1;  // Avoid placing food on the border

        // Check if food is placed on the snake
        validFoodPosition = true;
        for (size_t k = 0; k < snake.size(); k++) {
            if (foodX == snake[k].x && foodY == snake[k].y) {
                validFoodPosition = false;
                break; // Try placing food again
            }
        }
    }
}

int kbhit() {
    struct termios oldt, newt;
    int ch;
    int oldf;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

    ch = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);

    if(ch != EOF) {
        ungetc(ch, stdin);
        return 1;
    }

    return 0;
}

void board() {
    cout << "\033[H"; // Move the cursor to the top-left corner
    cout << endl;
    for (int i = 0; i < HEIGHT; i++) {
        cout << "\t\t#";
        for (int j = 0; j < WIDTH; j++) {
            bool isSnake = false;

            // Check if the current position is part of the snake
            for (size_t k = 0; k < snake.size(); k++) {
                if (i == snake[k].x && j == snake[k].y) {
                    cout << "*";
                    isSnake = true;
                    break;
                }
            }

            // Draw the food
            if (i == foodX && j == foodY && !isSnake) {
                cout << "F";
            } else if (!isSnake) {
                // Draw borders and empty space
                if (i == 0 || i == HEIGHT - 1 || j == 0 || j == WIDTH - 1)
                    cout << "#";
                else
                    cout << " ";
            }
        }
        cout << "#" << endl;
    }
}

void updateSnake() {
    // Move the snake body
    SnakeSegment newHead = snake[0]; // The current head of the snake

    if (movement == "UP") newHead.x--;
    else if (movement == "DOWN") newHead.x++;
    else if (movement == "LEFT") newHead.y--; // Move left by 1
    else if (movement == "RIGHT") newHead.y++; // Move right by 1

    // Check for collisions with walls or self
    if (newHead.x <= 0 || newHead.x >= HEIGHT - 1 || newHead.y <= 0 || newHead.y >= WIDTH - 1) {
        gameOver = true;
        return;
    }
    for (size_t k = 0; k < snake.size(); k++) {
        if (newHead.x == snake[k].x && newHead.y == snake[k].y) {
            gameOver = true;
            return;
        }
    }

    // Insert new head at the front of the snake
    snake.insert(snake.begin(), newHead);

    // Check if the snake eats the food
    if (newHead.x == foodX && newHead.y == foodY) {
        placeFood(); // Place new food
    } else {
        snake.pop_back(); // Remove the last segment (moving the snake)
    }
}

int main() {
    // Initialize snake with one segment
    SnakeSegment initialSegment = {10, 10};
    snake.push_back(initialSegment);

    placeFood(); // Initial food placement

    while (!gameOver) {
        board();

        // Check for user input (non-blocking)
        if (kbhit()) {
            char ch = getchar();  // Get the pressed key
            if (ch == 'w' && movement != "DOWN") 
                movement = "UP";
            else if (ch == 's' && movement != "UP") 
                movement = "DOWN";
            else if (ch == 'a' && movement != "RIGHT") 
                movement = "LEFT";            
            else if (ch == 'd' && movement != "LEFT") 
                movement = "RIGHT";
        }

        updateSnake();

        // Adjust the loop delay based on movement direction
        if (movement == "LEFT" || movement == "RIGHT") {
            this_thread::sleep_for(chrono::milliseconds(HORIZONTAL_SPEED)); // Faster horizontal movement
        } else {
            this_thread::sleep_for(chrono::milliseconds(VERTICAL_SPEED));   // Slower vertical movement
        }
    }

    cout << "\nGame Over! Your snake length was: " << snake.size() << endl;
    return 0;
}
