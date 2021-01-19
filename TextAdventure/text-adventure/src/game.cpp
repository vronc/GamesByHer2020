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

void Game::handleEntityInteraction() {
    if (player.currentLocation->entities.size() > 0) {
        std::cout << "\nChoose someone/something to approach:\n\n";
        for (int i = 0; i < player.currentLocation->entities.size(); ++i) {
            std::cout << "[" << i+1 << "]   " << player.currentLocation->entities[i]->id << "\n";
        }
        std::cout << "\n\n [e]    Exit\n";
        std::string choice = "";
        while (true) {
            std::cin >> choice;
            if (choice == "e" || choice == "E") { return; }
            
            else if (isPositiveIntInput(choice) || std::stoi(choice) > player.currentLocation->entities.size()) {
                std::cout << "\nYour choice isn't valid, try again!\n";
            } else {
                break;
            }
        }
        player.currentLocation->entities[std::stoi(choice)-1]->interact();
    } else {
        std::cout << "\nthis place seems to be empty...\n";
    }
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
        //std::cout << "~" << player.currentLocation->id << "~\n\n";
        std::cout << "\n" << player.currentLocation->text << "\n";
        
        if (player.currentLocation->choices.size()==0) {
            std::cout << "You've reached a point of no return!";
            gameOver();
        }
    
        std::cout << "\nChoose where you" << player.character << " want to go: ";
        for(int i=0 ; i<player.currentLocation->choices.size() ; i++) {
            std::cout << "\n" << i+1 << ": " << player.currentLocation->choices[i]->description;
        }
        std::cout << "\n\n" << "[i] Show Inventory";
        std::cout << "\n" << "[t] Interact with your surroundings" << "\n";
        
        choice = (int)player.currentLocation->choices.size()+1;
        
        while (true) {
            std::cin >> choice;
            if (choice == "i" || choice == "I") {
                player.printInventory();
                handleInventoryInput();
                break;
            }
            else if (choice == "t" || choice == "T") {
                handleEntityInteraction();
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
    std::cout << enemy->id << " attacks!\n";
    while (!enemy->isDead() && !player.isDead()) {
        enemy->takeDamage(player.attack());
        player.takeDamage(enemy->getNextAttackDmg());
    }
    if (enemy->isDead())
        std::cout << "\n" << enemy->id << " was defeated!\n";
    else if (player.isDead()){
        std::cout << "\nYou died...\n";
        gameOver();
    }
}

void Game::gameOver() {
    std::cout << "\ngame over!\n";
    std::exit(EXIT_SUCCESS);
}
