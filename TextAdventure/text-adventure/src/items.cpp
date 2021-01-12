//
//  items.cpp
//  text-adventure
//
//  Created by Veronica Hage on 2020-12-13.
//  Copyright © 2020 Veronica Hage. All rights reserved.
//


#include <string>
#include <iostream>
#include "items.hpp"
#include "game.hpp"

extern Game game;

BaseItem::BaseItem(const int itemId) {
    id=itemId;
};

TeleportScroll::TeleportScroll(const int itemId, const std::string& locationId)
: BaseItem (itemId) {
    keySymbol="📜";
    name="Teleport Scroll";
    teleportLocationId=locationId;
};

void TeleportScroll::useItem() {
    std::shared_ptr<Location> newLocation = game.gameData.getLocationById(teleportLocationId);
    
    if (newLocation != nullptr) {
        game.player.currentLocation = newLocation;
        
        std::cout << "You read the words on the scroll and you are no longer in the same place you were before! \n";
    }
}
    
