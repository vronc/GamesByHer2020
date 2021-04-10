//
//
//  SpaceRace
//
//  Created by Veronica Hage on 2021-02-07.
//

#pragma once

#include <SFML/Graphics.hpp>

static const sf::Color k_inactiveCheckpointColor(255, 255, 255, 175);
static const sf::Color k_activeCheckpointColor(0, 255, 150, 175);
static const sf::Color k_nextCheckpointColor(255, 255, 0, 225);

const std::string kPlayerWhale = "../assets/gfx/player_whale.png";
const std::string kMainScreenBackground = "../assets/gfx/sea_bg.png";
const std::string kDepthBlob = "../assets/gfx/depth_blob.png";
const std::string kLightBlob = "../assets/gfx/light-blob.png";
const std::string kBubbleSmall = "../assets/gfx/bubble_small.png";
const std::string kBubbleXSmall = "../assets/gfx/bubble_xSmall.png";
const std::string kCheckPointSprite = "../assets/gfx/checkpoint.png";
const std::string kKrill = "../assets/gfx/krill.png";
const std::string kDirBubble = "../assets/gfx/dirBubble.png";
const std::string kMainScreenFont = "../assets/fonts/roboto-regular.ttf";

static const float kSizeX = 1280.f;
static const float kSizeY = 720.f;
