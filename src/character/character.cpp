#include <iostream> 
#include <chrono> 
#include "character.hpp"
#include "randomgan.hpp"
#include "room.hpp"

using namespace std;

Character::Character(Species species, Pos pos, Room *room,
                    unsigned HP, unsigned Atk, unsigned Def):
                    m_species(species), m_pos(pos), m_room(room),
                    m_HP(HP), m_Atk(Atk), m_Def(Def){;
    m_time = chrono::high_resolution_clock::now();
}

Character::~Character(){}

void Character::update(){
    m_pos = update_pos;
}

Pos Character::getPos(){ return m_pos; }

Room *Character::getRoom(){ return m_room; }

Pos Character::generateNewPos(){
    int x_move = gen.getNumber(move_dist);
    int y_move = gen.getNumber(move_dist);
    update_pos.x = m_pos.x+x_move;
    update_pos.y = m_pos.y+y_move;
    return update_pos;
}

unsigned Character::getHP(){ return m_HP; }

unsigned Character::getAtk(){ return m_Atk; }

unsigned Character::getDef(){ return m_Def; }