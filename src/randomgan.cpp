#include "randomgan.hpp"
#include <unistd.h>

using namespace std;

RandomGen gen;
uniform_int_distribution<int> move_dist(-1, 1);

RandomGen::RandomGen(){
    generator.seed(getpid());   
}

int RandomGen::getNumber(std::uniform_int_distribution<int> &dist){
    return dist(generator);
}
