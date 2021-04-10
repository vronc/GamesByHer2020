#pragma once

#include "sfml-engine/scene.h"
#include <SFML/Audio.hpp>
#include "followcameranode.h"
#include "krill.h"


class MainGameScene : public gbh::Scene
{
protected:
    void onInitializeScene() override;
    void onUpdate(double deltaTime) override;
    void onKeyboardEvent(sf::Event& event) override;
private:
    void onBeginPhysicsContact(const gbh::PhysicsContact& contact) override;
    void advanceCheckpoint();
    void loadCheckPoints();
    std::shared_ptr<gbh::SpriteNode> m_playerWhale;
    std::shared_ptr<FollowCameraNode> m_followCamera;
    std::vector<std::shared_ptr<gbh::SpriteNode>> m_checkPoints;
    int m_currentCheckPoint = -1;
    std::vector<std::shared_ptr<Krill>> krills;
    std::vector<std::shared_ptr<gbh::SpriteNode>> m_depthBlobs;
    std::vector<std::shared_ptr<gbh::SpriteNode>> m_lightBlobs;
    std::vector<std::shared_ptr<gbh::SpriteNode>> m_bubbles;
    double m_playerTimer;
    std::shared_ptr<gbh::TextNode> m_timerText;
    sf::Font m_robotoFont;
    bool m_courseFinished = false;
    std::shared_ptr<gbh::SpriteNode> m_dirPointer;
};

