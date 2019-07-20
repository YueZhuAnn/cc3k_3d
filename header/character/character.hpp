#pragma once
#include <string>
#include <thread>
#include <vector>
#include <random>
#include <chrono> 
#include "definition.hpp"

class Room;
class Shade;
class Human;

class Character{
    // characteristics
    Species m_species;
    std::string m_name;
protected:
    bool bDead = false;
    unsigned m_HP;
    unsigned m_Atk;
    unsigned m_Def;

    // control the move
    Room *m_room = nullptr;
    Pos m_pos;
    Pos update_pos;

    // control the attack
    std::chrono::high_resolution_clock::time_point m_time;
public:
    Character(Species species, Pos pos, Room *room, 
            unsigned HP, unsigned Atk, unsigned Def);
    virtual ~Character() = 0;
    virtual Pos generateNewPos();
    void update();

    // get character info
    virtual Species getSpecies() = 0;
    unsigned getHP();
    unsigned getAtk();
    unsigned getDef();

    Pos getPos();
    Room *getRoom();
};