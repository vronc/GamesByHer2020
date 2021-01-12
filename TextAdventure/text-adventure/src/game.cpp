//
//  game.cpp
//  text-adventure
//
//  Created by Veronica Hage on 2020-11-28.
//  Copyright © 2020 Veronica Hage. All rights reserved.
//

#include "game.hpp"
#include "gamedata.hpp"
#include <iostream>
#include "../utils/utils.hpp"

void Game::handleInventoryInput() {
    std::string choice = "";
    while (true) {
        std::cin >> choice;
        if (choice == "e" || choice == "E") { return; }
        
        else if (isPositiveIntInput(choice) || std::stoi(choice) > player.inventory.size()) {
            std::cout << "\nYour choice isn't valid, try again!\n";
        }
        else if (player.inventory[std::stoi(choice)-1].amount < 1) {
            std::cout << "\n You're out of "+player.inventory[std::stoi(choice)-1].itemName << "\n";
        } else {
            break;
        }
    }
    InventoryItem &item = player.inventory[std::stoi(choice)-1];
    gameData.getItemById(item.itemId)->useItem();
    std::cout << "\nYou used your " << item.itemName << "\n\n";
    item.amount--;
}

void Game::run() {
    player.addItem(0, 1);
    player.currentLocation = gameData.getStartLocation();
    bool isRunning = true;
    std::string choice;
    std::cout << "Welcome to this text adventure! 🏕";
    std::cout << "\n\nChoose your character:\n\n";
    gameData.printAvailableCharacters();
    while (true) {
        std::cin >> choice;
        if (isPositiveIntInput(choice) || std::stoi(choice) > gameData.availableCharacters.size()) {
            std::cout << "\nYour choice isn't valid, try again!\n";
        } else {
            player.character = gameData.availableCharacters[std::stoi(choice)-1];
            break;
        }
    }

    while (isRunning) {
        if (player.currentLocation == nullptr) {
            std::cout << "You have fallen into an unexpected void...🕳";
            break;
        }
        std::cout << player.currentLocation->description;
        
        if (player.currentLocation->choices.size()==0){
            std::cout << "\ngame over\n";
            break;
        }
    
        std::cout << "\nChoose where you" << player.character << " want to go: ";
        for(int i=0 ; i<player.currentLocation->choices.size() ; i++) {
            std::cout << "\n" << i+1 << ": " << player.currentLocation->choices[i]->description;
        }
        std::cout << "\n" << "[i] Show Inventory" << "\n";
        
        choice = (int)player.currentLocation->choices.size()+1;
        
        while (true) {
            std::cin >> choice;
            if (choice == "i" || choice == "I") {
                player.printInventory();
                handleInventoryInput();
                break;
            }
            else if (isPositiveIntInput(choice) || std::stoi(choice) > player.currentLocation->choices.size()) {
                std::cout << "\nYour choice isn't valid, try again!\n";
            } else {
                player.currentLocation=player.currentLocation->choices[std::stoi(choice)-1];
                break;
            }
            
        }
    }
}

void Game::enterCombatMode(Enemy* enemy) {
    while (!enemy->isDead()) {
        enemy->takeDamage(player.attack());
        player.takeDamage(enemy->getNextAttackDmg());
    }
}
