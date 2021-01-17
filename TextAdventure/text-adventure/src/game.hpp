//
//  game.hpp
//  text-adventure
//
//  Created by Veronica Hage on 2020-11-28.
//  Copyright © 2020 Veronica Hage. All rights reserved.
//

#ifndef game_hpp
#define game_hpp

#include <stdio.h>
#include "player.hpp"
#include "entities.hpp"

#endif /* game_hpp */

class Game {
    public:
        GameData gameData = GameData();
        Player player;
        void run();
        void enterCombatMode(Enemy* enemy);
    
private:
    bool isRunning = true;
    void handleInventoryInput();
    void handleEntityInteraction();
};
