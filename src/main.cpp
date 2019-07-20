#include <iostream>
#include "cc3k.hpp"

using namespace std;

int main()
{
    CC3K cc3k;
    if (-1 == cc3k.gameInitialize()){
        cerr << "game init failed" << endl;
    }
}
