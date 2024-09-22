#include <iostream>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <chrono>
#include <thread>

using namespace std;

const int WIDTH = 50, HEIGHT = 25;

int x = 10, y = 10;
string movement = "UP";

// Function to check if a key was pressed
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
    for (int i = 0; i < HEIGHT; i++) {
        cout << "\t\t#";
        for (int j = 0; j < WIDTH; j++) {
            if (i == 0 || i == HEIGHT - 1)
                cout << "#";
            else if (i == x && j == y)
                cout << "*";
            else
                cout << " ";
        }
        cout << "#" << endl;
    }
}

int main() {
    while (true) {
        board();
        if (kbhit()) {
            char ch = getchar();  // Get the pressed key
            if (ch == 'w') 
                movement = "UP";
            else if (ch == 's') 
                movement = "DOWN";
            else if (ch == 'a') 
                movement = "LEFT";            
            else if (ch == 'd') 
                movement = "RIGHT";
        }
        if (movement.compare("UP") == 0)
            x--;
        else if (movement.compare("DOWN") == 0)
            x++;
        else if (movement.compare("LEFT") == 0)
            y-=2;
        else if (movement.compare("RIGHT") == 0)
            y+=2;
        this_thread::sleep_for(chrono::milliseconds(200)); // Slow down the loop
    }
    return 0;
}
