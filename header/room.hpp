#pragma once
#include <vector>
#include <mutex>
#include "definition.hpp"
#include "enemy.hpp"
#include "player.hpp"
#include "item.hpp"

class Room{
    Player *m_player = nullptr;
    Pos pos;
    unsigned width;
    unsigned height;
    Floor ** m_roominfo = nullptr;
    std::vector<std::vector<int>> m_buffer;
    std::vector<Enemy *> enemies;
    std::vector<Item *> items;

    int enemyIndex = 1;
    int potionIndex = 0;
    int goldIndex = 0;
    std::mutex mtxlk;
    void clearRoomData();
    void clearEnemies();
    void clearItems();
    void clearBuffer();    
    bool nearPlayer(const Pos &pos);      
    void goldGuard();
    // calculate index
    int getItemIndex(const int index);  
public:
    Room(Pos pos, unsigned w, unsigned h, Floor* info);
    ~Room();
    void operator=(const Room&) = delete;   // no copy
    void clear(); 
    void clearCurrentFloorInfo();            // must be called after game is over
    void fillDrawInfo(std::vector< std::vector<Floor> > &info);
    void generateEnemy(Species new_enemy);
    void generatePotion(ItemTypes new_potion);
    void generateGold(ItemTypes new_gold);
    void generatePlayer(Player *player);
    void moveEnemies();
    bool validPos(const Pos &pos);
    unsigned isEnemy(const int x, int const y);
    bool validGlobalPos(const Pos &gpos);
    bool isDoor(const int x, const int y);
    void pickItem(const int x, const int y);

    void playerAttack();
    Pos getLocalPos(const Pos &gpos);
    Pos getGlobalPos(const Pos &lpos);
    void removePlayer();
    void removeEnemy(const unsigned index);
    void setPlayer(Player *player);
    // test function
    void printRoom();
};
