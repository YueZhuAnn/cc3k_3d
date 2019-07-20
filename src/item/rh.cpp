#include "rh.hpp"
#include "player.hpp"

using namespace std;

RH::RH(int index, Pos pos):Item(ItemTypes::potionRH, pos, index){}

RH::~RH(){}

bool RH::effect(Player *player){
    player->addHP(10);  
    return true;
}