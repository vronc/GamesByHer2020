
#ifndef gamedata_hpp
#define gamedata_hpp

#include <stdio.h>

#endif /* gamedata_hpp */
# pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include "items.hpp"
#include "entities.hpp"
#include "../json/single_include/nlohmann/json.hpp"
#include "../utils/utils.hpp"

struct Location {
    Location(std::string id, std::string description);
    
    std::string id;
    std::string description;
    std::string text;
    std::vector<std::shared_ptr<Location>> choices;
    std::vector<std::shared_ptr<Entity>> entities;
};

class GameData {
    public:
        GameData();
        void debugLocations();
        std::shared_ptr<Location> getStartLocation();
        std::shared_ptr<Location> getLocationById(const std::string id);
        BaseItem* initializeItemFromName(std::string name);
        std::shared_ptr<BaseItem> getItemById(const int itemId);
        std::shared_ptr<Entity> getEntityById(const std::string entityId);
        std::unordered_map<std::string, std::shared_ptr<Location>> locationsMap;
        nlohmann::json availableNpcs;
        nlohmann::json availableEnemies;
        std::vector<std::string> availableCharacters;
        std::unordered_map<std::string, std::shared_ptr<Location>> availableLocations;
        void printAvailableCharacters();
        std::shared_ptr<Location> startLocation;
        node* biomeRootNode;
        void generateLocationGraph(node* biomeRoot, std::shared_ptr<Location> startLocation);
        void assignRandomEntities(std::vector<std::shared_ptr<Entity>> &entities, std::vector<std::string> biomeEnemies, std::vector<std::string> biomeNpcs);
        
    private:
        void createLocations();
        int loadLocations(const std::string& path);
        std::vector<std::shared_ptr<BaseItem>> items;
        std::vector<std::shared_ptr<Entity>> entities;
        void loadNpcs(std::string path);
        void loadEnemies(std::string path);
        int loadCharacters(std::string path);
        void loadBiomes(std::string path);
        std::shared_ptr<BaseItem> initNewItem(BaseItem* newItem);
};
