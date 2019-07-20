#pragma once
#include <random>

class RandomGen{
    std::default_random_engine generator;
public:
    RandomGen();
    int getNumber(std::uniform_int_distribution<int> &dist);
};

extern RandomGen gen;
extern std::uniform_int_distribution<int> move_dist;
