//
//  gamedata.cpp
//  text-adventure
//
//  Created by Veronica Hage on 2020-11-21.
//  Copyright © 2020 Veronica Hage. All rights reserved.
//

#include "gamedata.hpp"
#include <iostream>
#include <fstream>
#include <random>

Location::Location(std::string inId, std::string inDescription) {
    id = inId;
    description = inDescription;
}

GameData::GameData() {
    std::shared_ptr<BaseItem> item{ initializeItemFromName("Teleport Scroll") };
    items.push_back(item);

    loadNpcs("text-adventure/content/npcs.json");
    loadEnemies("text-adventure/content/enemies.json");
    loadCharacters("text-adventure/content/characters.txt");
    loadBiomes("text-adventure/content/biomes.json");
    createLocations();
}

void GameData::createLocations() {
    loadLocations("text-adventure/content/locations.json");
    std::cout << biomeRootNode->data["locations"][0];
    startLocation = getLocationById(biomeRootNode->data["locations"][0]);
    generateLocationGraph(biomeRootNode, startLocation);
}
void GameData::generateLocationGraph(node* biomeNode, std::shared_ptr<Location> currentLoc) {
    std::vector<std::string> locs = biomeNode->data["locations"];
    std::vector<std::string> neighboursWithinBiome=getRandomSelection(locs, getRandomPosInt(1, locs.size()));
    
    for (std::string neighbour : neighboursWithinBiome) {
        currentLoc->choices.push_back(getLocationById(neighbour));
    }
    
    if (biomeNode->next == nullptr)
        return;
    
    std::vector<std::string> nextLocs = biomeNode->next->data["locations"];
    std::vector<std::string> neighboursInNextBiome=getRandomSelection(nextLocs, getRandomPosInt(1, 2));
    
    for (std::string neighbour : neighboursInNextBiome) {
        currentLoc->choices.push_back(getLocationById(neighbour));
        generateLocationGraph(biomeNode->next, getLocationById(neighbour));
    }
}

std::shared_ptr<Location> GameData::getStartLocation() {
    return startLocation;
}

std::shared_ptr<Location> GameData::getLocationById(const std::string id) {
    
    return availableLocations[id];
}

int GameData::loadLocations(const std::string& path) {
    std::ifstream file(path);
    std::vector<std::string> lines;
    int locationsAdded = 0;
    Location workingLocation("","");
    
    if (!file.is_open()) {
        return locationsAdded;
    }

    nlohmann::json locationsJson = nlohmann::json::parse(file);
    
    for (int i=0 ; i<locationsJson["locations"].size()-1 ; i++) {
        workingLocation.id = locationsJson["locations"][i]["name"];
        workingLocation.description = locationsJson["locations"][i]["description"];
        
        availableLocations.insert_or_assign(workingLocation.id, std::make_shared<Location>(workingLocation));
        
        workingLocation.id = "";
        workingLocation.description = "";
        locationsAdded++;
    }

    return locationsAdded;
}

BaseItem* GameData::initializeItemFromName(std::string name) {

    if (name == "Teleport Scroll"){
        return new TeleportScroll((int)items.size(), "location01");
    } else
        return nullptr;
}

void GameData::loadNpcs(std::string path) {
    std::ifstream file(path);
    availableNpcs = nlohmann::json::parse(file);
    //std::cout << enemies["🦇"]["attacks"][0]["name"];
}

void GameData::loadEnemies(std::string path) {
    std::ifstream file(path);
    availableEnemies = nlohmann::json::parse(file);
    //std::cout << enemies["🦇"]["attacks"][0]["name"];
}

void GameData::loadBiomes(std::string path) {
    std::ifstream file(path);
    nlohmann::json biomeJson = nlohmann::json::parse(file);

    node* temp = nullptr;
    node* root = nullptr;
    for (int i=0 ; i<biomeJson["biomes"].size()-1 ; i++) {
    
        root = new node;
        root->data = biomeJson["biomes"][i];
        root->next = temp;
        temp = root;
    }
    biomeRootNode = root;
}

int GameData::loadCharacters(std::string path) {
    std::ifstream file(path);
    std::vector<std::string> lines;
    int charactersAdded = 0;
    
    if (!file.is_open()) {
        return charactersAdded;
    }
    std::string line;
    std::string character;
    
    while(std::getline(file, line)) {
        character = line.substr(0,line.size());
        availableCharacters.push_back(character);
    }
    return charactersAdded;
}

std::shared_ptr<BaseItem> GameData::getItemById(const int itemId) {
    for(int i = 0; i < items.size(); ++i) {
        if (items[i]->id == itemId) {
            return items[i];
        }
    }
    return nullptr;
}

std::shared_ptr<Entity> GameData::getEntityById(const std::string entityId) {
    for(int i = 0; i < entities.size(); ++i) {
        if (entities[i]->id == entityId) {
            return entities[i];
        }
    }
    return nullptr;
}

void GameData::printAvailableCharacters() {
    for (int i = 0; i < availableCharacters.size(); ++i) {
        std::cout << "\n [" << i+1 << "]   " << availableCharacters[i] <<"\n";
    }
}
