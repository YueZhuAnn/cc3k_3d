#include "human.hpp"
#include <iostream>
#include "randomgan.hpp"
#include "room.hpp"
#include "player.hpp"
#include "playerlist.hpp"

using namespace std;

Human::Human(Pos pos, Room* room, int index):
        Enemy{Species::human, pos, room, index, 140, 20, 20}{}

Human::~Human(){}

void Human::attack(Player *player){
        auto curtime = chrono::high_resolution_clock::now();
        auto t_interval = chrono::duration_cast<chrono::milliseconds>(curtime  - m_time);
        if(t_interval.count() < 1000) return;
        // update time
        m_time = chrono::high_resolution_clock::now();
        // 50% miss
        uniform_int_distribution<int> attack_dist(0, 1);
        bool bAttack = gen.getNumber(attack_dist);
        if ( !bAttack ) return;
        // calculate attack
        player->attacked(this);
}

void Human::attacked(Shade *player){
        float def = (float)100+m_Def;
        float atk = (float)player->getDef();
        int damage = 100/def * atk;
        m_HP -= min((int)m_HP, damage);
        if(m_HP == 0 && !bDead) {
                bDead = true;
                m_room->removeEnemy(index);
        }
}

Species Human::getSpecies(){ return Species::human; }