#include "bd.hpp"
#include "player.hpp"

using namespace std;

BD::BD(int index, Pos pos):Item(ItemTypes::potionBD, pos, index){}

BD::~BD(){}

bool BD::effect(Player *player){
    player->addDef(5);  
    return true;
}
