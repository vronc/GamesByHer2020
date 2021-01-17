//
//  entities.hpp
//  text-adventure
//
//  Created by Veronica Hage on 2021-01-10.
//  Copyright © 2021 Veronica Hage. All rights reserved.
//

#ifndef entities_hpp
#define entities_hpp

#pragma once

#include <stdio.h>
#include <string>
#include <vector>

#endif /* entities_hpp */

struct Attack {
    std::string name;
    int avgDmg;
};

class Entity {
public:
    Entity(const std::string id);
    virtual void interact() = 0;
    void takeDamage(int dmg);
    bool isDead();
    
    std::string id;
    
private:
    int health;
};

class Enemy : public Entity {
public:
    Enemy(const std::string id, std::vector<Attack> attacks);
    virtual void interact();
    
    std::vector<Attack> attacks;
    
    int getNextAttackDmg();
};

class Npc : public Entity {
public:
    Npc(const std::string id, std::vector<std::string> dialogues);
    virtual void interact();
    
    std::vector<std::string> dialogues;
};
