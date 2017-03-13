#include <iostream>
using namespace std;


int search(int v[], int size, int key) {
    for (int i = 0; i < size; ++i) {
        if (v[i] == key) {
            return i;
        }
    }

    return -1;
}


int main(int argc, char const *argv[])
{
    cout << "hello" << endl;
    return 0;
}