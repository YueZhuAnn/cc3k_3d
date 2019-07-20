#include "wa.hpp"
#include "player.hpp"

using namespace std;

WA::WA(int index, Pos pos):Item(ItemTypes::potionWA, pos, index){}

WA::~WA(){}

bool WA::effect(Player *player){
    player->subAtk(5); 
    return true; 
}