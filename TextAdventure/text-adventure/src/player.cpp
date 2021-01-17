//
//  player.cpp
//  text-adventure
//
//  Created by Veronica Hage on 2020-12-13.
//  Copyright © 2020 Veronica Hage. All rights reserved.
//
#include "player.hpp"
#include <iostream>
#include "game.hpp"
#include "../utils/utils.hpp"

extern Game game;

void Player::addItem(const int id, int amount) {
    for(int i = 0; i < inventory.size(); ++i) {
        if (inventory[i].itemId == id) {
            inventory[i].amount += amount;
            return;
        }
    }
    
    inventory.push_back(InventoryItem{ .itemId = id, .itemName = game.gameData.getItemById(id)->name, .amount = amount });
}

void Player::removeItem(const int id, int amount) {
    for(int i = 0; i < inventory.size(); ++i) {
        if (inventory[i].itemId == id) {
            inventory[i].amount -= amount;
            
            if (inventory[i].amount <= 0) {
                inventory[i].amount = 0;
            }
            
            return;
        }
    }
}

void Player::printInventory() {
    for (int i = 0; i < inventory.size(); ++i) {
        std::cout << "\nYour inventory🎒:\n\n [" << i+1 << "]   " << inventory[i].amount << "   " << inventory[i].itemName << "\n\n [e]    Exit\n";
    }
}

int Player::attack() {
    int dmg = getRandomPosInt(3, 10);
    std::cout << "\nYou attacked, inflicting " << dmg << " damage points\n";
    return dmg;
}

void Player::takeDamage(int dmg) {
    health -= dmg;
}
