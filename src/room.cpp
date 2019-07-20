#include<iostream>
#include "room.hpp"
#include "enemylist.hpp"
#include "playerlist.hpp"
#include "itemlist.hpp"
#include "randomgan.hpp"

using namespace std;

Room::Room(Pos pos, unsigned w, unsigned h, Floor* info):
    pos(pos), width(w), height(h){
        m_roominfo = new Floor*[h];
        m_buffer.resize(h);
        for(unsigned i = 0; i < h; i++){
            m_roominfo[i] = new Floor[w];
            m_buffer[i].resize(w, -1);
            for(unsigned q = 0; q < w; q++){
                m_roominfo[i][q] = info[i*w+q];
            }
        }
    }

Room::~Room(){}

void Room::clear(){
    clearCurrentFloorInfo();
    clearRoomData();
}

void Room::clearCurrentFloorInfo(){
    clearEnemies();
    clearItems();
    clearBuffer();
}

void Room::clearRoomData(){
    for(unsigned i = 0; i < height; i++){
        delete [] m_roominfo[i];
    }
    delete [] m_roominfo;
}

void Room::clearEnemies(){
    for(auto it = enemies.begin(); it != enemies.end(); ++it){
        Enemy * enemy = *it;
        delete enemy;
    }
    enemies.clear();
    enemyIndex = 1;
}

void Room::clearItems(){
    // clear potions
    for(auto it = items.begin(); it != items.end(); ++it){
        Item * item = *it;
        delete item;
    }
    items.clear();
    potionIndex = 0;
    goldIndex = 0;
}

void Room::clearBuffer(){
    m_buffer.clear();
    m_buffer.resize(height);
    for(unsigned i = 0; i < height; i++){
        m_buffer[i].resize(width, -1);
    }
}

void Room::fillDrawInfo(vector< vector<Floor> > &info){
    unsigned x = pos.x;
    unsigned y = pos.y;
    // fill in room info
    for(unsigned p = 0; p < height; p++){ 
        for(unsigned q = 0; q < width; q++){
            info[x+p][y+q] = m_roominfo[p][q];
        }
    }

    // fill in enemies
    for(auto it = enemies.begin(); it != enemies.end(); ++it){
        if((*it) == nullptr) continue;
        Pos pos = (*it)->getPos();
        info[x+pos.x][y+pos.y] = Floor::character;
    }

    // fill in items
    for(unsigned i = 0; i < potionIndex; i++){
        Item *ptr = items[i];
        if(ptr == nullptr) continue;
        Pos pos = ptr->getPos();
        info[x+pos.x][y+pos.y] = Floor::potion;
    }

    // fill in golds
    for(unsigned i = 0; i < goldIndex; i++){
        Item *ptr = items[i+potionIndex];
        if(ptr == nullptr) continue;
        Pos pos = ptr->getPos();
        info[x+pos.x][y+pos.y] = Floor::gold;
    }
}

bool Room::validPos(const Pos &pos){
    int x = pos.x;
    int y = pos.y;
    if(x < 0 || y < 0) return false;
    if(x >= height || y >= width) return false;
    return (m_roominfo[x][y] == Floor::tile &&
    m_buffer[x][y] == -1);
}

unsigned Room::isEnemy(const int x, const int y){
    if(x < 0 || y < 0) return 0;
    if(x >= height || y >= width) return 0;
    int index = max(0,m_buffer[x][y]);
    if(m_roominfo[x][y] == Floor::tile && index < enemyIndex) return index;
    return 0;
}

bool Room::validGlobalPos(const Pos &gpos){
    Pos lpos = getLocalPos(gpos);
    return validPos(lpos);
}

int Room::getItemIndex(const int index){
    if(index >= 100 && index < 200){
        // potion
        return index-100;
    }else if(index >= 200){
        // gold
        return index-200+potionIndex;
    }
    return -1;
}

bool Room::isDoor(const int x, const int y){
    if(m_player == nullptr) return false;
    Pos pos = m_player->getPos();
    int playerx = (int)pos.x + x;
    int playery = (int)pos.y + y;
    if(playerx < 0 || playerx >= height) return false;
    if(playery < 0 || playery >= width)  return false;
    return m_roominfo[playerx][playery] == Floor::door;
}

void Room::pickItem(const int x, const int y){
    if(m_player == nullptr) return;
    Pos pos = m_player->getPos();
    int playerx = (int)pos.x + x;
    int playery = (int)pos.y + y;
    if(playerx < 0 || playerx >= height) return;
    if(playery < 0 || playery >= width)  return;
    int index =  getItemIndex(m_buffer[playerx][playery]);
    if (index != -1){
        Item *item = items[index];
        if (item->effect(m_player)){
            // delete item
            mtxlk.lock();
            {   // clear buffer
                m_buffer[playerx][playery] = -1;
            }
            mtxlk.unlock();
            items[index] = nullptr;
            delete item;
        }
    }
}

void Room::playerAttack(){
    std::vector<Enemy *> attackEnemies;
    Pos pos = m_player->getPos();
    int x = pos.x;
    int y = pos.y;
    for(int p = -1; p <= 1; p++){
        for(int q = -1; q <= 1; q++){
            unsigned index = isEnemy(x+p, y+q);
            if(index > 0) {
                attackEnemies.push_back(enemies[index-1]);
            }
        }
    }
    m_player->attackEnemy(attackEnemies);
}

Pos Room::getLocalPos(const Pos &gpos){ return gpos-pos; }

Pos Room::getGlobalPos(const Pos &lpos){ return lpos+pos;}

void Room::moveEnemies(){
    mtxlk.lock();
    for(auto it = enemies.begin(); it != enemies.end(); ++it){
        if((*it) == nullptr) continue;
        Pos old_pos = (*it)->getPos();
        Pos new_pos = (*it)->generateNewPos();
        if(validPos(new_pos)){
            (*it)->update();
            swap(m_buffer[old_pos.x][old_pos.y], 
                m_buffer[new_pos.x][new_pos.y]);
        }
        if(Species::dragon == (*it)->getSpecies()){
            continue;
        }
        new_pos = (*it)->getPos();
        if(nearPlayer(new_pos)){
            int index = m_buffer[new_pos.x][new_pos.y];
            Enemy *enemy = enemies[index-1];
            enemy->attack(m_player);
        }
    }
    // check player move
    goldGuard();
    mtxlk.unlock();
}

void Room::goldGuard(){
    if (m_player == nullptr) return;
    for(unsigned i = 0; i < goldIndex; i++){
        Item *ptr = items[i+potionIndex];
        if(ptr == nullptr || ptr->getType() != ItemTypes::goldDragon) continue;
        Pos pos_diff = ptr->getPos() - m_player->getPos();
        if((-1 <= pos_diff.x && pos_diff.x <= 1) && 
            -1 <= pos_diff.y && pos_diff.y <= 1){
                DragonGold *goldptr = dynamic_cast<DragonGold *>(ptr);
                goldptr->guard(m_player);
            }
    }
}

bool Room:: nearPlayer(const Pos &pos){
    if(m_player == nullptr) return false;
    Pos playerPos = m_player->getPos();
    int x_diff = playerPos.x-pos.x;
    int y_diff = playerPos.y-pos.y;
    return (-1 <= x_diff && x_diff <= 1) && 
    (-1 <= y_diff && y_diff <= 1);
}

void Room::generateEnemy(Species new_enemy){
    uniform_int_distribution<int> x_dist(0, height-1);
    uniform_int_distribution<int> y_dist(0, width-1);
    int x = gen.getNumber(x_dist);
    int y = gen.getNumber(y_dist);
    Pos pos{x,y};
    while(!validPos(pos)){
        pos.x = gen.getNumber(x_dist);
        pos.y = gen.getNumber(y_dist);
    }
    m_buffer[pos.x][pos.y] = enemyIndex;
    Enemy *enemy = new Human(pos, this, enemyIndex);
    enemyIndex++;
    /*
    switch (new_enemy){
        case Species::human: {
            enemy = new Human(pos, this);  
            break;
        }
        default: break;
    } 
    */
    enemies.push_back(enemy);
}

void Room::generatePotion(ItemTypes new_potion){
    uniform_int_distribution<int> x_dist(0, height-1);
    uniform_int_distribution<int> y_dist(0, width-1);
    int x = gen.getNumber(x_dist);
    int y = gen.getNumber(y_dist);
    Pos pos{x,y};
    while(!validPos(pos)){
        pos.x = gen.getNumber(x_dist);
        pos.y = gen.getNumber(y_dist);
    }
    Item *item = nullptr;
    m_buffer[pos.x][pos.y] = 100+potionIndex;
    switch (new_potion){
        case ItemTypes::potionRH: {
            item = new RH(100+potionIndex, pos);
            break;
        }
        case ItemTypes::potionBA: {
            item = new BA(100+potionIndex, pos);
            break;
        }
        case ItemTypes::potionBD: {
            item = new BD(100+potionIndex, pos);
            break;
        }
        case ItemTypes::potionPH: {
            item  = new PH(100+potionIndex, pos);
            break;
        }
        case ItemTypes::potionWA: {
            item = new WA(100+potionIndex, pos);
            break;
        }
        case ItemTypes::potionWD: {
            item = new WD(100+potionIndex, pos);
            break;
        }
        default: { break; }
    } 
    potionIndex++;
    items.push_back(item);
}

void Room::generateGold(ItemTypes new_gold){
    uniform_int_distribution<int> x_dist(0, height-1);
    uniform_int_distribution<int> y_dist(0, width-1);
    int x = gen.getNumber(x_dist);
    int y = gen.getNumber(y_dist);
    Pos pos{x,y};
    while(!validPos(pos)){
        pos.x = gen.getNumber(x_dist);
        pos.y = gen.getNumber(y_dist);
    }
    Item *item = nullptr;
    m_buffer[pos.x][pos.y] = 200+goldIndex;
    switch (new_gold){
        case ItemTypes::goldNormal: {
            item = new NormalGold(200+goldIndex, pos);
            break;
        }
        case ItemTypes::goldSmall: {
            item = new SmallGold(200+goldIndex, pos);
            break;
        }
        case ItemTypes::goldDragon: {
            DragonGold *gold= new DragonGold(200+goldIndex, pos);
            Pos dragonPos = pos;
            for(int x = -1; x <= 1; x++){
                for(int y = -1; y <= 1; y++){
                    dragonPos.x = pos.x+x;
                    dragonPos.y = pos.y+y;
                    if(validPos(dragonPos)) break;
                }
            }
            Dragon *dragon = new Dragon(dragonPos, this, enemyIndex);
            m_buffer[dragonPos.x][dragonPos.y] = enemyIndex;
            dragon->setGold(gold);
            gold->setDragon(dragon);
            enemyIndex++;
            enemies.push_back((Enemy*)dragon);
            item = gold;
            break;
        }
        default: { break; }
    } 
    goldIndex++;
    items.push_back(item);
}

void Room::generatePlayer(Player *player){
    uniform_int_distribution<int> x_dist(0, height-1);
    uniform_int_distribution<int> y_dist(0, width-1);
    int x = gen.getNumber(x_dist);
    int y = gen.getNumber(y_dist);
    Pos pos{x, y};
    while(!validPos(pos)){
        pos.x = gen.getNumber(x_dist);
        pos.y = gen.getNumber(y_dist);
    }
    m_buffer[pos.x][pos.y] = 0;
    player->resetRoom(this, pos);
    m_player = player;
}

void Room::removePlayer(){
    Pos pos = m_player->getPos();
    m_buffer[pos.x][pos.y] = -1;
    m_player = nullptr;
}


void Room::setPlayer(Player *player){
    m_player = player;
    Pos pos = m_player->getPos();
    m_buffer[pos.x][pos.y] = 0;
}

void Room::removeEnemy(const unsigned index){
    mtxlk.lock();
    {
        Enemy *ptr = enemies[index-1];
        Pos pos = enemies[index-1]->getPos();
        // clear ptr
        enemies[index-1] = nullptr;
        delete ptr;
        // clear buffer
        m_buffer[pos.x][pos.y] = -1;
    }
    mtxlk.unlock();
}

void Room::printRoom(){
    for(unsigned h = 0; h < height; h++){
        for(unsigned w = 0; w < width; w++){
            Floor floor = m_roominfo[h][w];
            if(m_buffer[h][w] > 0 && m_buffer[h][w] < enemyIndex){
                floor = Floor::character;
            } else if(m_buffer[h][w] >= 100 && m_buffer[h][w] < 200){
                floor = Floor::potion;
            } else if(m_buffer[h][w] >= 200){
                floor = Floor::gold;
            }
            switch (floor){
                case Floor::wall:               cout << "X"; break;
                case Floor::tile:               cout << "."; break;
                case Floor::character:          cout << "c"; break;
                case Floor::player:             cout << "p"; break;
                case Floor::potion:             cout << "i"; break;
                case Floor::gold:               cout << "g"; break;
                default:                        cout <<  " "; break;
            }
        }
        cout << endl;
    }
}