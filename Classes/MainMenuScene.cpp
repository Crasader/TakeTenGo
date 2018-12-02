#include "MainMenuScene.h"
#include "GameScene.h"
#include "Definitions.h"
#include "GameManager.h"
//#include "ui/CocosGUI.h"
//#include "extensions/cocos-ext.h"
#include <string.h>


USING_NS_CC;

Scene* MainMenuScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
	auto layer = MainMenuScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

void MainMenuScene::onHttpRequestCompleted(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response) {
	if (!response) {
		return;
	}

	// You can get original request type from: response->request->reqType
	if (0 != strlen(response->getHttpRequest()->getTag()))
	{
		CCLOG("%s completed", response->getHttpRequest()->getTag());
	}

	int statusCode = response->getResponseCode();

		if (!response->isSucceed())
		{
			CCLOG("response failed");
			CCLOG("error buffer: %s", response->getErrorBuffer());
		return;
		}

	// dump data
	std::vector<char> *buffer = response->getResponseData();

	std::string bufferData = "";

	CCLOG("Http Test, dump data: ");
	for (unsigned int i = 0; i < buffer->size(); i++)
	{
		bufferData += (*buffer)[i];
		CCLOG("%c", (*buffer)[i]);
	}
	CCLOG("DATA: %s", bufferData);

}

// on "init" you need to initialize your instance
bool MainMenuScene::init() {
	if (!LayerColor::initWithColor(Color4B(51, 51, 51, 255))) {
		return false;
	}

	
	cocos2d::network::HttpRequest* request = new cocos2d::network::HttpRequest();
	request->setUrl("http://www.shadowbizgame.com/bluesbowling/display.php");
	request->setRequestType(cocos2d::network::HttpRequest::Type::GET);
	request->setResponseCallback(CC_CALLBACK_2(MainMenuScene::onHttpRequestCompleted, this));
	request->setTag("GET SCORE");
	cocos2d::network::HttpClient::getInstance()->send(request);
	

	
	request->release();


	auto gameManager = TakeTen::GameManager::getInstance();

	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto origin = Director::getInstance()->getVisibleOrigin();
	
	cocos2d::Vector<cocos2d::MenuItem*> pMenuItems;

	TTFConfig label_config;
	label_config.fontFilePath = "fonts/gameFont.ttf";
	label_config.fontSize = 95;
	label_config.glyphs = GlyphCollection::DYNAMIC;
	label_config.customGlyphs = nullptr;
	label_config.distanceFieldEnabled = true;
	label_config.outlineSize = 0;
	
	auto centerPosition = Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y - 600);

	auto menuPos = Vec2(0, 600);

	std::string menu[6] = {
		"Practice ", "Easy ", "Medium ", "Hard ", "Ultimate ", "Hardcore "
	};

	auto menuHeight = 1.25f * 200 * 6;

	ColorHSB colors[2] = { ColorHSB(5, 53, 57), ColorHSB(144, 22, 46) };

	//colors[index0] = ColorHSB(199, 25, 27);
	//colors[index1] = ColorHSB(33, 40, 77);
	//colors[index2] = ColorHSB(200, 23, 57);
	//colors[index3] = ColorHSB(5, 53, 57);

	auto index = 0;

	for (auto r = 0; r != 3; ++r) {
		for (auto c = 0; c != 2; ++c) {

			auto y = 400 * (r- 1);

			auto sprite = Sprite::createWithSpriteFrameName("gamemenu_Cell.png");
			auto spritePressed = Sprite::createWithSpriteFrameName("gamemenu_Cell.png");
			auto color = ColorHSB(114, 100, 74);
			auto pressedColor = ColorHSB(9, 25, 127);

			//sprite->setColor(color.toRGB());

			auto boardIndex = gameManager->getIndex(index);
			sprite->setColor(ColorHSB::lerp(colors[0], colors[1], boardIndex / 100.0f).toRGB());

			spritePressed->setColor(pressedColor.toRGB());
			auto menuItem = cocos2d::MenuItemSprite::create(sprite, spritePressed, CC_CALLBACK_1(MainMenuScene::GotoGameSceneMenuItem, this));
			menuItem->setTag(index);
			menuItem->setPosition((static_cast<float>(c) - 0.5f) * 400, -y);

			auto difficultyLabel = Label::createWithBMFont("fonts/bitmapFont.fnt", menu[index]);
			difficultyLabel->setColor(Color3B(198, 163, 117));
			difficultyLabel->setAnchorPoint(Vec2(0, 1));
			difficultyLabel->setPosition(40, 375);
			difficultyLabel->setScale(0.75f);

			auto labelString = std::to_string(gameManager->getIndex(index)) + "%";
			auto percentLabel = Label::createWithBMFont("fonts/bitmapFontNumeric.fnt", labelString);
			percentLabel->setColor(Color3B(198, 163, 117));
			percentLabel->setAnchorPoint(Vec2(1, 0));
			percentLabel->setPosition(370, 20);
			percentLabel->setScale(0.75f);

			menuItem->addChild(percentLabel, 1);
			menuItem->addChild(difficultyLabel, 1);

			auto time = gameManager->getTime(index);
			if (time > 1) {
				auto timeString = TakeTen::Tools::getTimeString(gameManager->getTime(index));
				auto timeLabel = Label::createWithBMFont("fonts/bitmapFontNumeric.fnt", timeString);
				timeLabel->setColor(Color3B(198, 163, 117));
				timeLabel->setAnchorPoint(Vec2(0, 1));
				timeLabel->setPosition(40, 300);
				timeLabel->setScale(0.55f);
				menuItem->addChild(timeLabel, 1);
			}
			

			
			

			pMenuItems.pushBack(menuItem);

			index++;
		}
	}

	auto pMenu = cocos2d::Menu::createWithArray(pMenuItems);
	this->addChild(pMenu, 1);


    return true;
}

void MainMenuScene::GotoGameSceneMenuItem(Ref* pSender) {
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("tick.wav");

	auto menuItem = static_cast<cocos2d::MenuItemSprite*>(pSender);
	auto gamemanager = TakeTen::GameManager::getInstance();

	gamemanager->newGame(static_cast<TakeTen::GameDifficuty>(menuItem->getTag()));

	return;

	switch (menuItem->getTag())
	{
	case MENU_PRACTICE_TAG:
		gamemanager->newGame(TakeTen::GameDifficuty::Practice);
		break;
	case MENU_EASY_TAG:
		gamemanager->newGame(TakeTen::GameDifficuty::Easy);
		break;
	case MENU_MEDIUM_TAG:
		gamemanager->newGame(TakeTen::GameDifficuty::Medium);
		break;
	case MENU_HARD_TAG:
		gamemanager->newGame(TakeTen::GameDifficuty::Hard);
		break;
	case MENU_ULTIMATE_TAG:
		gamemanager->newGame(TakeTen::GameDifficuty::Ultimate);
		break;
	case MENU_HARDCORE_TAG:
		gamemanager->newGame(TakeTen::GameDifficuty::Hardcore);
		break;
	default:
		gamemanager->newGame(TakeTen::GameDifficuty::Practice);
		break;
	}
}

void MainMenuScene::menuCloseCallback(Ref* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
    return;
#endif

    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
