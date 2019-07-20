#include "ba.hpp"
#include "player.hpp"

using namespace std;

BA::BA(int index, Pos pos):Item(ItemTypes::potionBA, pos, index){}

BA::~BA(){}

bool BA::effect(Player *player){
    player->addAtk(5); 
    return true; 
}
