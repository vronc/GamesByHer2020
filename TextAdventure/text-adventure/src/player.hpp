//
//  player.hpp
//  text-adventure
//
//  Created by Veronica Hage on 2020-12-13.
//  Copyright © 2020 Veronica Hage. All rights reserved.
//
#pragma once

#ifndef player_hpp
#define player_hpp

#include <stdio.h>
#include "gamedata.hpp"

#endif /* player_hpp */

struct InventoryItem {
    std::string itemName;
    int itemId;
    int amount = 1;
};

class Player {
public:
    std::shared_ptr<Location> currentLocation = nullptr;
    int moves = 0;
    int health = 1;
    std::string character;
    
    void addItem(const int id, int amount);
    void removeItem(const int id, int amount);
    void printInventory();
    int attack();
    void takeDamage(int dmg);
    bool isDead();
    
    std::vector<InventoryItem> inventory;
};

