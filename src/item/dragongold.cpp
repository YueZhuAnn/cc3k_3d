#include "dragongold.hpp"
#include "player.hpp"
#include "dragon.hpp"
#include <iostream>

using namespace std;

DragonGold::DragonGold(int index, Pos pos):Item(ItemTypes::goldDragon, pos, index){}

DragonGold::~DragonGold(){}

bool DragonGold::effect(Player *player){
    if(m_dragon != nullptr) {
        return false;
    }
    player->addScore(4);  
    return true;
}

void DragonGold::setDragon(Dragon *dragon){ m_dragon = dragon; }

void DragonGold::removeDragon(){ m_dragon = nullptr; }
void DragonGold::guard(Player *player){
    if (m_dragon == nullptr) return;
    m_dragon->attack(player);
}