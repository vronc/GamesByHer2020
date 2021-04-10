#include "title_scene.h"

#include <iostream>

#include "sfml-engine/game.h"
#include "sfml-engine/shapenode.h"
#include "sfml-engine/spritenode.h"
#include "sfml-engine/textnode.h"
#include "constants.h"
#include "utils.h"

const std::string kTitleMusic = "../assets/music/titlescreen.ogg";
const std::string kTitleScreenFont = "../assets/fonts/roboto-regular.ttf";
const std::string kTitleScreenBackground = "../assets/gfx/space_backdrop.png";


void TitleScene::onInitializeScene()
{
	m_robotoFont.loadFromFile(kTitleScreenFont);
	m_titleMusic.openFromFile(kTitleMusic);
	m_titleMusic.setVolume(25);

	std::shared_ptr<gbh::SpriteNode> spriteNode = std::make_shared<gbh::SpriteNode>(kTitleScreenBackground);
    spriteNode->setName("Background");
    spriteNode->setPosition(kSizeX/2.f, kSizeY/2.f);
	addChild(spriteNode);

	std::shared_ptr<gbh::TextNode> textNode = std::make_shared<gbh::TextNode>("Sea Race", m_robotoFont, 60);
	textNode->setPosition(kSizeX/2.f, kSizeY/2.f-250.f);
	textNode->setName("Title");
	addChild(textNode);

	std::shared_ptr<gbh::SpriteNode> whaleNode = std::make_shared<gbh::SpriteNode>(kPlayerWhale);
    whaleNode->setName("Whale");
	whaleNode->setPosition(kSizeX/2.f, kSizeY/2.f+100.f);
	addChild(whaleNode);

    const int k_buttonSizeX = 200;
    const int k_buttonSizeY = 60;
	std::shared_ptr<gbh::ShapeNode> startButton = std::make_shared<gbh::ShapeNode>(sf::RectangleShape(sf::Vector2f(k_buttonSizeX, k_buttonSizeY)));
	startButton->setPosition(kSizeX/2.f, kSizeY/2.f+250.f);
	startButton->getShape()->setFillColor(sf::Color(0, 255, 0, 64));
	startButton->setName("StartButton");
	addChild(startButton);

	std::shared_ptr<gbh::TextNode> startNode = std::make_shared<gbh::TextNode>("Start Game", m_robotoFont);
    startNode->setPosition(k_buttonSizeX/2.f, k_buttonSizeY/2.f-10.f);
    startNode->setName("StartButton");
	startButton->addChild(startNode);
    
    createPhysicsWorld(sf::Vector2f(0.0f, 0.0f));
    timer = 0.f;
    
    for (int i = 0 ; i < 40 ; i++) {
        std::string bubbleSprite;
        if (getRandomNormalized() > 0)
            bubbleSprite = kBubbleSmall;
        else
            bubbleSprite = kBubbleXSmall;
        
        std::shared_ptr<gbh::SpriteNode> bubble = std::make_shared<gbh::SpriteNode>(bubbleSprite);
        bubble->setPosition(getRandomPosInt(0, kSizeX*2), getRandomPosInt(0, kSizeY/2.f-250.f));
        m_bubbles.push_back(bubble);
        addChild(bubble);
    }
}


void TitleScene::onUpdate(double deltaTime)
{
    timer += deltaTime;
    std::shared_ptr<gbh::Node> titleNode = getFirstChildWithName("Title", false);
    std::shared_ptr<gbh::Node> whaleNode = getFirstChildWithName("Whale", false);

    if (titleNode)
    {
        titleNode->move(0, 2*sin((float)timer));
    }
    
    if (whaleNode)
    {
        whaleNode->rotate(1.f+sin((float)timer));
    }
    
    for (std::shared_ptr<gbh::SpriteNode> bubble : m_bubbles) {
        if (bubble->getPosition().y < 0)
            bubble->setPosition(bubble->getPosition().x, titleNode->getPosition().y);
        bubble->move(sin(timer), -1.f);
    }
}


void TitleScene::onMouseEvent(sf::Event& event)
{
	if (event.type == sf::Event::MouseButtonPressed)
	{
		std::shared_ptr<gbh::Node> node = this->getNodeAtViewPoint((float)event.mouseButton.x, (float)event.mouseButton.y);
        
        if (node->getName() == "StartButton")
        {
            gbh::Game::getInstance().changeScene("maingame");
        }
	}
}


void TitleScene::onShowScene()
{
	//m_titleMusic.play();
}


void TitleScene::onHideScene()
{
	m_titleMusic.stop();
}

