#include "dragon.hpp"
#include <iostream>
#include "randomgan.hpp"
#include "room.hpp"
#include "player.hpp"
#include "playerlist.hpp"
#include "dragongold.hpp"

using namespace std;

Dragon::Dragon(Pos pos, Room* room, int index):
        Enemy{Species::dragon, pos, room, index, 140, 20, 20}{}

Dragon::~Dragon(){}

void Dragon::setGold(DragonGold *gold){ m_gold = gold; }

int convertToIndex(const int x_diff, const int y_diff){
        int index;
        if (y_diff == -1){
                index = x_diff+1;
        } else if (y_diff == 0){
                x_diff == -1? index = 7:index = 3;
        } else{
                index = 5-x_diff;
        }
        return index;
}

Pos convertToPos(const int index){
        Pos pos{0,0};
        if (index < 3){
                pos.y = -1;
                pos.x = index-1;
        }else if(index == 7){
                pos.x = -1;
        }else if(index == 3){
                pos.x = 1;
        }else{
                pos.y = 1;
                pos.x = 5-index;
        }
        return pos;
}

Pos Dragon::generateNewPos(){
        uniform_int_distribution<int> move(0, 1);
        int left = gen.getNumber(move);
        Pos goldPos = m_gold->getPos();
        int x_diff = m_pos.x - goldPos.x;
        int y_diff = m_pos.y - goldPos.y;
        int index = convertToIndex(x_diff, y_diff);
        left? index++:index--;
        if( index == 8 ) index = 0;
        update_pos = goldPos+convertToPos(index);
        return update_pos;
}

void Dragon::attack(Player *player){
        auto curtime = chrono::high_resolution_clock::now();
        auto t_interval = chrono::duration_cast<chrono::milliseconds>(curtime  - m_time);
        if(t_interval.count() < 1000) return;
        // update time
        m_time = chrono::high_resolution_clock::now();
        // IF NOT NEAR MY GOLD, DON'T ATTACK
        // 50% miss
        uniform_int_distribution<int> attack_dist(0, 1);
        bool bAttack = gen.getNumber(attack_dist);
        if ( !bAttack ) return;
        // calculate attack
        player->attacked(this);
}

void Dragon::attacked(Shade *player){
        float def = (float)100+m_Def;
        float atk = (float)player->getDef();
        int damage = 100/def * atk;
        m_HP -= min((int)m_HP, damage);
        if(m_HP == 0 && !bDead) {
                bDead = true;
                m_gold->removeDragon();
                m_room->removeEnemy(index);
        }
}

Species Dragon::getSpecies(){ return Species::dragon; }