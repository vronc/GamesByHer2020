//
//  entities.cpp
//  text-adventure
//
//  Created by Veronica Hage on 2021-01-10.
//  Copyright © 2021 Veronica Hage. All rights reserved.
//
#include <iostream>
#include <random>
#include "../utils/utils.hpp"
#include "game.hpp"

extern Game game;

Entity::Entity(const std::string entityId) {
    id=entityId;
    health=20;
};

bool Entity::isDead() {
    return health <= 0;
};

void Entity::takeDamage(int dmg) {
    health -= dmg;
}

Enemy::Enemy(const std::string enemyId, std::vector<Attack> enemyAttacks)
: Entity (enemyId) {
    attacks = enemyAttacks;
};

void Enemy::interact() {
    game.enterCombatMode(this);
}

int Enemy::getNextAttackDmg() {
    Attack attack = attacks[getRandomPosInt(0, attacks.size()-1)];
    int dmg = std::max(1, attack.avgDmg-3+getRandomPosInt(0,6));
    std::cout << id << " used " << attack.name << ", inflicting " << dmg << " damage points\n\n";
    return dmg;
}

Npc::Npc(const std::string npcId, std::vector<std::string> npcDialogues)
: Entity (npcId) {
    dialogues = npcDialogues;
};

void Npc::interact() {
    std::cout << this->id << ": " << this->dialogues[0];
}
