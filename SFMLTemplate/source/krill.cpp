//
//  krill.h
//  SpaceRace
//
//  Created by Veronica Hage on 2021-02-07.
//

#include <stdio.h>
#include <string>
#include <SFML/Graphics.hpp>
#include "krill.h"
#include "utils.h"
#include "sfml-engine/mathutils.h"

Krill::Krill(std::string sprite, gbh::PhysicsWorld* physicsWorld)
: gbh::SpriteNode (sprite) {
    this->setPhysicsBody(physicsWorld->createCircle(10));
    this->getPhysicsBody()->setLinearDamping(2.0f);
    this->m_prevSteering = sf::Vector2f(1.f, 1.f);
};

sf::Vector2f Krill::getSteeringForce() {
    sf::Vector2f newSteering = gbh::math::normalize(m_prevSteering + 0.5f*sf::Vector2f(getRandomNormalized(), getRandomNormalized()));
    m_prevSteering = newSteering;
    return 0.2f*newSteering;
};


