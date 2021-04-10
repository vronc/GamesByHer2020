//
//  krill.cpp
//  SpaceRace
//
//  Created by Veronica Hage on 2021-02-07.
//
#pragma once

#include <stdio.h>
#include <SFML/Graphics.hpp>
#include "sfml-engine/spritenode.h"


class Krill : public gbh::SpriteNode {
public:
    Krill(std::string sprite, gbh::PhysicsWorld* physicsWorld);
    sf::Vector2f getSteeringForce();
    sf::Vector2f m_prevSteering;
};

