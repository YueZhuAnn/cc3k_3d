#include <sstream>
#include <iostream>
#include <vector>
#include <chrono> 
#include "view.hpp"
#include "model.hpp"
#include "randomgan.hpp"
#include "playerlist.hpp"

using namespace std;

// test function

CC3KModel::CC3KModel(string mapPath):m_mapPath(mapPath){
    // read map infomation from configure
    string sInfo;
    ifstream ifsMap(mapPath);
    
    if (ifsMap.is_open()){
        getline(ifsMap, sInfo);
        // get grid size
        istringstream issSize(sInfo);
        issSize >> m_size;
        readRoomInfo(ifsMap);
        readPasswayInfo(ifsMap);
        ifsMap.close();   
    }
}

CC3KModel::~CC3KModel(){}

void CC3KModel::linkView(CC3KView *view){ m_view = view; }

void CC3KModel::clearData(){
    bRunning = false;
    // stop moving
    m_thread->join();
    delete m_thread;

    for(auto it = rooms.begin(); it!= rooms.end(); ++it){
        Room *ptr = (*it);
        ptr->clear();
        delete ptr;
    }

    for(auto it = passways.begin(); it != passways.end(); ++it){
        Passway *ptr = (*it);
        delete ptr;
    }

    delete m_player;
}

void CC3KModel::fillDrawInfo(std::vector< std::vector<Floor> > &info){
    // resize info
    info.resize(m_size);
    for(unsigned i = 0; i < m_size; i++){
        info[i].resize(m_size, Floor::none);
    }

    // room info
    for(auto it = rooms.begin(); it != rooms.end(); ++it){
        (*it)->fillDrawInfo(info);   
    }

    // passway info
    for(auto it = passways.begin(); it != passways.end(); ++it){
        (*it)->fillDrawInfo(info);  
    }

    // player info
    Pos pos = m_player->getGlobalPos();
    info[pos.x][pos.y] = Floor::player;
}

void CC3KModel::readRoomInfo(std::ifstream &ifsFile){
    string sInfo;
    while(getline(ifsFile, sInfo)){
            // ignore empty string
            if (sInfo == "") continue;
            if (sInfo == "passway") break;
            istringstream issInfo(sInfo);
            int x, y, h, w;
            issInfo >> x >> y >> h >> w;
            Pos pos{x, y};
            Floor* roomInfo = new Floor[w*h];
            for (unsigned i = 0; i < h; i++){
                getline(ifsFile, sInfo);
                unsigned length = sInfo.length();
                for(unsigned q = 0; q < w; q++){
                    if (q >= length){
                        roomInfo[i*w+q] = Floor::none;
                        continue;}
                    switch(sInfo[q]){
                        case '|': roomInfo[i*w+q] = Floor::wall; break;
                        case '-': roomInfo[i*w+q] = Floor::wall; break;
                        case '.': roomInfo[i*w+q] = Floor::tile; break;
                        case '#': roomInfo[i*w+q] = Floor::tile; break;
                        case '\\': roomInfo[i*w+q] = Floor::door; break;
                        default: roomInfo[i*w+q] = Floor::none; 
                    }
                }
            }
            // store the info
            Room *room = new Room(pos, w, h, roomInfo);
            rooms.push_back(room);
            delete [] roomInfo;
    }      
}

void CC3KModel::readPasswayInfo(ifstream &ifsFile){
    string sInfo;
    while(getline(ifsFile, sInfo)){
        // ignore empty string
        if (sInfo == "") continue;
        istringstream issInfo(sInfo);
        int x, y;
        unsigned d, l;
        issInfo >> x >> y >> d >> l;
        Direction dir = d == 0? Direction::ver : Direction::hor;
        Pos pos{x, y};
        // store the info
        Passway *passway = new Passway{pos, dir, l};
        passways.push_back(passway);
    }
}

void CC3KModel::gameInit(){
    m_roomsize = rooms.size();
    // enemy generation probability
    auto it = enemy_prob.begin();
    it = enemy_prob.insert(it, 4, Species::human);
    it = enemy_prob.insert(it, 3, Species::dwarf);
    it = enemy_prob.insert(it, 5, Species::halfling);
    it = enemy_prob.insert(it, 2, Species::elf);
    it = enemy_prob.insert(it, 2, Species::orc);
    it = enemy_prob.insert(it, 2, Species::merchant);

    // potion generation probability
    auto potionProb = potion_prob.begin();
    potionProb = potion_prob.insert(potionProb, 1, ItemTypes::potionRH);
    potionProb = potion_prob.insert(potionProb, 1, ItemTypes::potionBA);
    potionProb = potion_prob.insert(potionProb, 1, ItemTypes::potionBD);
    potionProb = potion_prob.insert(potionProb, 1, ItemTypes::potionPH);
    potionProb = potion_prob.insert(potionProb, 1, ItemTypes::potionWA);
    potionProb = potion_prob.insert(potionProb, 1, ItemTypes::potionWD);

    // gold generation probability
    auto goldProb = gold_prob.begin();
    goldProb = gold_prob.insert(goldProb, 5, ItemTypes::goldNormal);
    goldProb = gold_prob.insert(goldProb, 2, ItemTypes::goldSmall);
    goldProb = gold_prob.insert(goldProb, 1, ItemTypes::goldDragon);

    // create player
    m_player = new Shade(Pos{0,0}, this, nullptr);
    generateFloor();
    generatePlayer();
    moveEnemies();
}

void CC3KModel::gameOver(){
    bRunning = false;
    m_view->gameOver();
}

void CC3KModel::gameRestart(){
    m_floor = 0;
    delete m_player;
    m_player = new Shade(Pos{0,0}, this, nullptr);
    resetFloor();
    m_thread->join();
    delete m_thread;
    bRunning = true;
    moveEnemies();
}

void CC3KModel::resetFloor(){
    for(auto it = rooms.begin(); it != rooms.end(); ++it){
        (*it)->clearCurrentFloorInfo();
    }
    m_player->AtkDefReset();
    generateFloor();
    generatePlayer();
}

void CC3KModel::generateFloor(){
    // generate enemies
    uniform_int_distribution<int> room_dist(0, m_roomsize-1);
    uniform_int_distribution<int> enemy_dist(0, enemy_prob.size()-1);
    uniform_int_distribution<int> potion_dist(0, potion_prob.size()-1);
    uniform_int_distribution<int> gold_dist(0, gold_prob.size()-1);
    for(unsigned i = 0; i < m_enemies; i++){
        int room_id = gen.getNumber(room_dist);
        int enemy_spe = gen.getNumber(enemy_dist);
        Species new_enemy = enemy_prob[enemy_spe];
        rooms[room_id]->generateEnemy(new_enemy);
    }

    // generate potions
    for(unsigned i = 0; i < m_potions; i++){
        int room_id = gen.getNumber(room_dist);
        int potion_type = gen.getNumber(potion_dist);
        ItemTypes new_potion = potion_prob[potion_type];
        rooms[room_id]->generatePotion(new_potion);
    }
    
    // generate gold
    for(unsigned i = 0; i < m_potions; i++){
        int room_id = gen.getNumber(room_dist);
        int gold_type = gen.getNumber(gold_dist);
        ItemTypes new_gold = gold_prob[gold_type];
        rooms[room_id]->generateGold(new_gold);
    }
    m_floor++;
}

void CC3KModel::generatePlayer(){
    // random get a room
    uniform_int_distribution<int> room_dist(0, m_roomsize-1);
    int room_id = gen.getNumber(room_dist);
    rooms[room_id]->generatePlayer(m_player);
}

void CC3KModel::moveLoop(){
    auto m_time = chrono::high_resolution_clock::now();
    auto curtime = chrono::high_resolution_clock::now();
    const unsigned frametime = 1000;
    while(bRunning){
        for(auto it = rooms.begin(); it != rooms.end(); ++it){
            (*it)->moveEnemies();
        }
        curtime = chrono::high_resolution_clock::now();
        auto t_interval = chrono::duration_cast<chrono::milliseconds>(curtime  - m_time);
        int interval = frametime - t_interval.count();
        if(interval > 0){
            this_thread::sleep_for(chrono::milliseconds(interval));
        }
        m_time = chrono::high_resolution_clock::now();
    }
}

void CC3KModel::moveEnemies(){
    m_thread = new thread(&CC3KModel::moveLoop, this);
}

void CC3KModel::playerMove(int x, int y){ 
    Room *room = m_player->getRoom();
    if(room != nullptr && room->isDoor(x, y)){
        resetFloor();
        return;
    }
    if (room != nullptr) room->pickItem(x, y);
    updatePlayerPos(x, y);
}

void CC3KModel::updatePlayerPos(int x, int y){
    Pos gpos = m_player->getGlobalPos();
    gpos.x += x;
    gpos.y += y;
    
    for(auto it = rooms.begin(); it != rooms.end(); ++it){
        if( (*it)->validGlobalPos(gpos) ){
            m_player->resetRoom(*it, (*it)->getLocalPos(gpos));
            (*it)->setPlayer(m_player);
            return;}
    }
    for(auto it = passways.begin(); it != passways.end(); ++it){
        if( (*it)->validGlobalPos(gpos) ){
            m_player->resetPassway(*it, (*it)->getLocalPos(gpos));
            (*it)->setPlayer(m_player);
            return;}
    }
}

void CC3KModel::playerAttack(){
    Room * room= m_player->getRoom();
    if (room == nullptr) return;
    room->playerAttack();
}

unsigned CC3KModel::getGridSize(){ return m_size; }

unsigned CC3KModel::getFloor(){ return m_floor; }

Species CC3KModel::getPlayerSpecies(){ return m_player->getSpecies(); }

unsigned CC3KModel::getPlayerHP(){ return m_player->getHP(); }

unsigned CC3KModel::getPlayerAtk(){ return m_player->getAtk(); }

unsigned CC3KModel::getPlayerDef(){ return m_player->getDef(); }

unsigned CC3KModel::getScore(){ return m_player->getScore(); }

Species CC3KModel::getEnemySpecies(){ return m_enemy->getSpecies(); }

unsigned CC3KModel::getEnemyHP(){ return m_enemy->getHP(); }

unsigned CC3KModel::getEnemyAtk(){ return m_enemy->getAtk(); }

unsigned CC3KModel::getEnemyDef(){ return m_enemy->getDef(); }