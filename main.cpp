#include <iostream>
#include <cstdlib>
#include <thread>

using namespace std;

const int WIDTH = 50, HEIGHT = 25;

int x = 0, y = 10;

void board() {
    system("clear");
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

// g++ main.cpp -o main
int main() {
    while (true) {
        board();
        x++;
        if (x>25)
            x=0;
        this_thread::sleep_for(chrono::milliseconds(100)); // Pause for a short time
    }
    return 0;
}
