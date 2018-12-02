#include "SplashScene.h"
#include "MainMenuScene.h"
#include "Definitions.h"
#include "BoardManager.h"
#include "GameManager.h"
#include "Board.h"

USING_NS_CC;

Scene* SplashScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
	auto layer = SplashScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool SplashScene::init()
{
    //////////////////////////////
    // 1. super init first
	if (!LayerColor::initWithColor(Color4B(51, 51, 51, 255)))
    {
        return false;
    }
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();
	auto spriteBatch = SpriteBatchNode::create("spriteSheet.png");
	auto cache = SpriteFrameCache::getInstance();
	cache->addSpriteFramesWithFile("spriteSheet.plist");
	this->addChild(spriteBatch);

	auto cellSprite = Sprite::createWithSpriteFrameName("logo.png");
	cellSprite->setPosition(Point(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
	//cellSprite->setColor()
	spriteBatch->addChild(cellSprite, 0);

	CocosDenshion::SimpleAudioEngine::getInstance()->setEffectsVolume(1.0f);
	//CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("music.wav", true);

	this->scheduleOnce(schedule_selector(SplashScene::MainMenuLoader), SPLASH_DISPLAY_TIME);
    return true;
}

void SplashScene::MainMenuLoader(float dt) {
	TakeTen::GameManager::getInstance();
	auto mainMenuScene = MainMenuScene::createScene();
	Director::getInstance()->replaceScene(SCENE_TRANSITION(TRANSITION_TIME, mainMenuScene));
}
