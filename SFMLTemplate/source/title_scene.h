#pragma once 

#include "sfml-engine/scene.h"
#include <SFML/Audio.hpp>


class TitleScene : public gbh::Scene
{
protected:
	void onInitializeScene() override;

	virtual void onUpdate(double deltaTime) override;
	virtual void onMouseEvent(sf::Event& event) override;
	virtual void onShowScene() override;
	virtual void onHideScene() override;

private:
	sf::Font m_robotoFont;
	sf::Music m_titleMusic;
    float timer;
    std::vector<std::shared_ptr<gbh::SpriteNode>> m_bubbles;
    
};
