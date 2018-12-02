#include "SimpleAudioEngine.h"

#include "Definitions.h"
#include "HelpScene.h"
#include "MainMenuScene.h"
#include "GameManager.h"
#include "SplashScene.h"

namespace TakeTen {
	cocos2d::Scene* SplashScene::createScene() {
		auto scene = cocos2d::Scene::create();
		auto layer = SplashScene::create();
		scene->addChild(layer);
		return scene;
	}

	// on "init" you need to initialize your instance
	bool SplashScene::init() {
		if (!LayerColor::initWithColor(cocos2d::Color4B(51, 51, 51, 255))) {
			return false;
		}
		auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
		auto origin = cocos2d::Director::getInstance()->getVisibleOrigin();
		auto spriteBatch = cocos2d::SpriteBatchNode::create(SPRITE_SHIT_PNG);
		auto cache = cocos2d::SpriteFrameCache::getInstance();
		cache->addSpriteFramesWithFile(SPRITE_SHIT_PLIST);
		this->addChild(spriteBatch);

		auto cellSprite = cocos2d::Sprite::createWithSpriteFrameName(LOGO_IMAGE);
		cellSprite->setPosition(cocos2d::Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
		spriteBatch->addChild(cellSprite, 0);

		GameManager::getInstance();

		this->scheduleOnce(schedule_selector(SplashScene::MainMenuLoader), SPLASH_DISPLAY_TIME);
		return true;
	}

	void SplashScene::MainMenuLoader(float dt) {

		if (GameManager::getInstance()->isFirstRun()) {
			CCLOG("SPLASH SCENE >> FIRST RUN...");
			auto helpScene = HelpScene::createScene();
			cocos2d::Director::getInstance()->replaceScene(SCENE_TRANSITION(TRANSITION_TIME, helpScene));
		}
		else {
			auto mainMenuScene = MainMenuScene::createScene();
			cocos2d::Director::getInstance()->replaceScene(SCENE_TRANSITION(TRANSITION_TIME, mainMenuScene));
		}
	}
}


