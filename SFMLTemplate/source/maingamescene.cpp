#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>

#include "sfml-engine/game.h"
#include "sfml-engine/shapenode.h"
#include "sfml-engine/spritenode.h"
#include "sfml-engine/textnode.h"
#include "sfml-engine/mathutils.h"
#include "utils.h"
#include "constants.h"
#include <nlohmann/json.hpp>

#include <iostream>
#include <fstream>
std::string levelFilename = "../assets/levels/level_01.json";


#include "maingamescene.h"

sf::Vector2f prevPos(0.f, 0.f);
bool loadCheckpointsFromFile = false;
const float kSceneSizeX = 2.f*kSizeX;
const float kSceneSizeY = 2.f*kSizeY;

void MainGameScene::onInitializeScene()
{
    //setDrawPhysicsDebug(true);
    
    createPhysicsWorld(sf::Vector2f(0.0f, 0.0f));
    
    
    // BOUNDARY
    
    std::shared_ptr<gbh::Node> boundary = std::make_shared<gbh::Node>();
    boundary->setPhysicsBody(getPhysicsWorld()->createEdgeBox(sf::Vector2f(kSceneSizeX*2-400, kSceneSizeY*2-400)));
    boundary->getPhysicsBody()->setType(gbh::PhysicsBodyType::Static);
    boundary->setPosition(kSceneSizeX, kSceneSizeY);
    addChild(boundary);
    
    // BACKGROUND
    
    std::shared_ptr<gbh::SpriteNode> backgroundNode = std::make_shared<gbh::SpriteNode>(kMainScreenBackground);
    backgroundNode->setPosition(kSceneSizeX, kSceneSizeY);
    backgroundNode->setName("Background");
    addChild(backgroundNode);
    m_robotoFont.loadFromFile(kMainScreenFont);
    
    // DEPTHBLOBS
    
    for (int i = 0 ; i < 40 ; i++) {
        std::shared_ptr<gbh::SpriteNode> depthBlob = std::make_shared<gbh::SpriteNode>(kDepthBlob);
        depthBlob->setPosition(getRandomPosInt(-kSceneSizeX, kSceneSizeX*2.5f), getRandomPosInt(-kSceneSizeY, kSceneSizeY*2.5f));
        m_depthBlobs.push_back(depthBlob);
        addChild(depthBlob);
    }
    
    // BUBBLES
    
    for (int i = 0 ; i < 40 ; i++) {
        std::string bubbleSprite;
        if (getRandomNormalized() > 0)
            bubbleSprite = kBubbleSmall;
        else
            bubbleSprite = kBubbleXSmall;
        
        std::shared_ptr<gbh::SpriteNode> bubble = std::make_shared<gbh::SpriteNode>(bubbleSprite);
        bubble->setPosition(getRandomPosInt(0, kSceneSizeX*2), getRandomPosInt(0, kSceneSizeY*2));
        m_bubbles.push_back(bubble);
        addChild(bubble);
    }
    
    // PLAYER
    
    const sf::Vector2f shipSize = sf::Vector2f(80.0f, 120.0f);
    
    m_playerWhale = std::make_shared<gbh::SpriteNode>(kPlayerWhale);
    m_playerWhale->setPosition(300.f, kSceneSizeX/2.f);
    m_playerWhale->setScale(0.5f, 0.5f);
    m_playerWhale->setPhysicsBody(getPhysicsWorld()->createBox(shipSize * 0.5f));
    m_playerWhale->getPhysicsBody()->setLinearDamping(2.0f);
    m_playerWhale->getPhysicsBody()->setFixedRotation(true);
    addChild(m_playerWhale);
    
    prevPos = m_playerWhale->getPosition();
    
    // CHECKPOINTS
    
    loadCheckPoints();
    advanceCheckpoint();
    
    // LIGHT BLOBS
    
    for (int i = 0 ; i < 40 ; i++) {
        std::shared_ptr<gbh::SpriteNode> lightBlob = std::make_shared<gbh::SpriteNode>(kLightBlob);
        lightBlob->setPosition(getRandomPosInt(-kSceneSizeX, kSceneSizeX*3), getRandomPosInt(-kSceneSizeY, kSceneSizeY*3));
        m_lightBlobs.push_back(lightBlob);
        addChild(lightBlob);
    }
    
    m_dirPointer = std::make_shared<gbh::SpriteNode>(kDirBubble);
    m_dirPointer->setColor(sf::Color::Cyan);
    m_dirPointer->setPosition(m_playerWhale->getPosition());
    addChild(m_dirPointer);
    
    // TIMER
    
    m_timerText = std::make_shared<gbh::TextNode>("0", m_robotoFont, 24);
    m_timerText->setPosition(kSizeX-30, kSizeY-25);
    getOverlay().addChild(m_timerText);
    
    // CAMERA
    
    m_followCamera = std::make_shared<FollowCameraNode>();
    m_followCamera->setTarget(m_playerWhale);
    m_followCamera->setPosition(kSceneSizeX, kSceneSizeY);
    
    addChild(m_followCamera);
    setCamera(m_followCamera);
    
    
    for (int i = 0 ; i < 10 ; i++) {
        std::shared_ptr<Krill> krill = std::make_shared<Krill>(kKrill, getPhysicsWorld());
        krill->setPosition(kSizeX, kSizeY);
        krills.push_back(krill);
        addChild(krill);
    }
}

void MainGameScene::onBeginPhysicsContact(const gbh::PhysicsContact& contact)
{
    if (contact.containsNode(m_playerWhale.get())) {
        if (contact.otherNode(m_playerWhale.get())->getName() == "checkpoint") {
            advanceCheckpoint();
        }
    }
}

void MainGameScene::advanceCheckpoint() {
    if (m_currentCheckPoint >= 0 && m_currentCheckPoint < m_checkPoints.size()) {
        m_checkPoints[m_currentCheckPoint]->getPhysicsBody()->setEnabled(false);
        m_checkPoints[m_currentCheckPoint]->setColor(k_activeCheckpointColor);
        m_currentCheckPoint++;
    }
    
    else if (m_currentCheckPoint == -1) {
        m_currentCheckPoint = 0;
    }
    
    if (m_currentCheckPoint < m_checkPoints.size()) {
        m_checkPoints[m_currentCheckPoint]->setColor(k_nextCheckpointColor);
        m_checkPoints[m_currentCheckPoint]->getPhysicsBody()->setEnabled(true);
    }
    else if (m_currentCheckPoint == m_checkPoints.size()) {
        m_courseFinished = true;
    }
}

void MainGameScene::loadCheckPoints() {
    std::vector<sf::Vector2f> checkPointPositions;
    
    if (loadCheckpointsFromFile) {
        std::ifstream levelFile(levelFilename);
        nlohmann::json levelJsonFile;
        
        try
        {
            levelJsonFile = nlohmann::json::parse(levelFile);
        }
        catch(const std::exception& ex)
        {
            std::cout << "Failed to load level from file " << levelFilename << ": " << ex.what() << "\n";
            return;
        }
        nlohmann::json checkpoints = levelJsonFile["checkpoints"];
        
        if (!checkpoints.is_array())
        {
            std::cout << "Levels file either does not include a 'checkpoints' entry, or it is not an array.";
            return;
            
        } else {
            for(int i = 0; i < checkpoints.size(); ++i)
            {
                float x = checkpoints[i]["x"].get<float>();
                float y = checkpoints[i]["y"].get<float>();
                
                checkPointPositions.push_back(sf::Vector2(x, y));
            }
        }
    } else {
        
        // GENERATED COURSE
        int i = 1;
        float p = 200.f;
        float newX = 0.f;
        float newY = 0.f;
        sf::Vector2f prev(newX, newY);
        sf::Vector2f temp(newX, newY);
        while (newX < kSceneSizeX-300.f) {

            newX = 200.f+p*i + (getRandomNormalized()+1)*75.f;
            newY = (kSceneSizeY/3.f)*std::cos(p*i)+kSceneSizeY + (getRandomNormalized()+1)*75.f;
            
            temp = sf::Vector2f(newX, newY);
            
            if (gbh::math::distance(prev, temp) < 100.f) {
                temp = sf::Vector2f(newX + 75.f, newY + 75.f);
            }
                
            
            checkPointPositions.push_back(temp);
            prev = sf::Vector2f(temp.x, temp.y);
            i = i + std::abs(getRandomNormalized())*1.2 + 0.75f;
        }
    }
    
    for (int i = 0 ; i < checkPointPositions.size() ; i++) {
        sf::Vector2f nextPosition = checkPointPositions[i];
        
        std::shared_ptr<gbh::SpriteNode> node = std::make_shared<gbh::SpriteNode>(kCheckPointSprite);
        node->setColor(k_inactiveCheckpointColor);
        node->setPosition(nextPosition);
        node->setName("checkpoint");
        node->setPhysicsBody(getPhysicsWorld()->createCircle(50));
        node->getPhysicsBody()->makeSensor();
        node->getPhysicsBody()->setEnabled(false);
        
        m_checkPoints.push_back(node);
        addChild(node);
    }
}

void MainGameScene::onKeyboardEvent(sf::Event& event)
{
    if (event.type == sf::Event::KeyPressed)
    {
        if (event.key.code == sf::Keyboard::Space)
        {
            if (m_courseFinished)
            {
                gbh::Game::getInstance().changeScene("title");
            }
        }
    }
}

void MainGameScene::onUpdate(double deltaTime)
{
    static const float acceleration = 2000.0f;
    sf::Vector2f playerPos = m_playerWhale->getPosition();
    
    sf::Vector2f moveDirection;
    
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
        moveDirection.y -= 1.0f;
        m_playerWhale->setRotation(0);
    }
    
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
        moveDirection.y += 1.0f;
        m_playerWhale->setRotation(180);
    }
    
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
        moveDirection.x -= 1.0f;
        m_playerWhale->setRotation(270);
    }
    
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        moveDirection.x += 1.0f;
        m_playerWhale->setRotation(90);
    }
    
    moveDirection = gbh::math::normalize(moveDirection);
    m_playerWhale->getPhysicsBody()->applyForceToCenter(moveDirection * acceleration);
    
    // CHECKPOINT POINTER
    
    if (m_currentCheckPoint < m_checkPoints.size()) {
        sf::Vector2f checkpointPos = m_checkPoints[m_currentCheckPoint]->getPosition();
        sf::Vector2f dir = gbh::math::direction(checkpointPos, playerPos);
        m_dirPointer->setPosition(playerPos+gbh::math::normalize(dir)*75.f);
    }
    
    // BLOBS AND BUBBLES
    
    sf::Vector2f blobMovement(-(m_playerWhale->getPosition().x - prevPos.x),
                              -(m_playerWhale->getPosition().y - prevPos.y));
    
    for (std::shared_ptr<gbh::SpriteNode> depthBlob : m_depthBlobs) {
        float dist = gbh::math::distance(m_playerWhale->getPosition(), depthBlob->getPosition());
        float clamped =gbh::math::clampValue(dist, 299.f);
        depthBlob->move(blobMovement*gbh::math::clampValue((300.f-clamped), 2.f));
    }
    
    for (std::shared_ptr<gbh::SpriteNode> lightBlob : m_lightBlobs) {
        float dist = gbh::math::distance(m_playerWhale->getPosition(), lightBlob->getPosition());
        float clamped =gbh::math::clampValue(dist, 299.f);
        lightBlob->move(blobMovement*gbh::math::clampValue((300.f-clamped), 4.f));
    }
    
    for (std::shared_ptr<gbh::SpriteNode> bubble : m_bubbles) {
        if (bubble->getPosition().y < 0)
            bubble->setPosition(bubble->getPosition().x, getRandomPosInt(kSceneSizeY, kSceneSizeY*3));
        bubble->move(sin(m_playerTimer), -1.f);
    }
    
    if (gbh::math::distance(m_playerWhale->getPosition(), prevPos) > 10.f && getRandomNormalized() > 0.5f) {
        m_bubbles[getRandomPosInt(0, m_bubbles.size()-1)]->setPosition(prevPos);
    }

    
    prevPos = m_playerWhale->getPosition();
    
    
    // KRILLS
    
    for (std::shared_ptr<Krill> krill : krills) {
        krill->getPhysicsBody()->applyForceToCenter(krill->getSteeringForce() * acceleration*0.5f);
    }
    
    
    // GAME LOOP CHECK
    
    m_playerTimer += deltaTime;
    char buffer[32];
    sprintf(buffer, "%.2f", m_playerTimer);
    
    if (m_courseFinished == true) {
        m_timerText->setFillColor(sf::Color(0.f, 255.f*sin(m_playerTimer*3.f), 60.f));
    } else {
        m_timerText->setString(buffer);
    }
}

