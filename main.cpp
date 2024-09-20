#include <iostream>
#define sq(x) x*x

using namespace std;

const int WIDTH = 50, HEIGHT = 25;

void board(){
    for (int i=0; i<HEIGHT ; i++){
        cout << "\t\t#";
        for (int j=0; j<WIDTH; j++){
            if (i==0 || i== HEIGHT-1)
                cout << "#";
            else    
                cout << " ";
        }
        cout << "#" << endl;
    }
} 

int main(){
    board();
    return 0;
}