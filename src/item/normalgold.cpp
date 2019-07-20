#include "normalgold.hpp"
#include "player.hpp"

using namespace std;

NormalGold::NormalGold(int index, Pos pos):Item(ItemTypes::goldNormal, pos, index){}

NormalGold::~NormalGold(){}

bool NormalGold::effect(Player *player){
    player->addScore(2);  
    return true;
}