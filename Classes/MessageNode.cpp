#include "Definitions.h"

#include "Tools.h"
#include "BackgroundNode.h"
#include "GameManager.h"

#include "LanguageManager.h"

#include "MessageNode.h"

namespace TakeTen {
	MessageNode::~MessageNode() {
		removeAllChildrenWithCleanup(true);
	}

	cocos2d::Scene* MessageNode::createScene(const cocos2d::ccMenuCallback& acceptCallBack, int tag) {
		auto scene = cocos2d::Scene::create();
		auto layer = MessageNode::create(acceptCallBack, tag);
		scene->addChild(layer);
		return scene;
		
	}

	MessageNode* MessageNode::create(const cocos2d::ccMenuCallback& acceptCallBack, int tag) {
		auto node = new (std::nothrow) MessageNode;
		node->setTag(tag);
		if (node->init(acceptCallBack)) {
			node->autorelease();
			return node;
		}
		CCLOG("ERROR CREATING BOARD NODE");
		CC_SAFE_DELETE(node);
		return nullptr;
	}

	bool MessageNode::init(const cocos2d::ccMenuCallback& acceptCallBack) {
		
		if (!LayerColor::initWithColor(cocos2d::Color4B(51, 51, 51, 255))) {
			return false;
		}

		auto alignment = cocos2d::TextHAlignment::CENTER;
		auto color = cocos2d::Color3B(198, 163, 117);
		auto gameManager = GameManager::getInstance();

		auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
		auto center = cocos2d::Vec2(visibleSize.width / 2, visibleSize.height / 2);

		auto backFn = [](Ref* pSender) {
			GameManager::getInstance()->soundSelect();
			cocos2d::Director::getInstance()->popScene();
		};

		auto listener = cocos2d::EventListenerKeyboard::create();
		listener->onKeyReleased = CC_CALLBACK_2(MessageNode::onKeyReleased, this);
		_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

		auto bg = BackgroundNode::create();
		this->addChild(bg);

		auto label = cocos2d::Label::createWithBMFont(FONT_FILE_PATH, REPLAY_STRING);
		label->setAlignment(alignment);
		label->setColor(color);
		label->setPosition(visibleSize.width / 2, visibleSize.height / 2 + 200);
		label->setScale(0.65f * FONT_SCALE_FACTOR);
		addChild(label, 1);
		
		cocos2d::Vector<cocos2d::MenuItem*> pMenuItems;
		auto pressedColor = ColorHSB(9, 25, 127);
		auto delayTimer = ANIMATION_DELAY_TIME;

		_backMenu = MenuButton::create(ICON_MENU_QUIT, pressedColor, delayTimer, backFn);
		delayTimer += ANIMATION_DELAY_TIME;

		_acceptMenu = MenuButton::create(ICON_MENU_PLAY, pressedColor, delayTimer, acceptCallBack);
		delayTimer += ANIMATION_DELAY_TIME;

		_backMenu->setPosition(-100, 0);
		_acceptMenu->setPosition(100, 0);

		_acceptMenu->setTag(this->getTag());

		pMenuItems.pushBack(_backMenu);
		pMenuItems.pushBack(_acceptMenu);

		auto mainMenu = cocos2d::Menu::createWithArray(pMenuItems);
		this->addChild(mainMenu, 100);

		CCLOG("MESSAGE NO TAG %d", this->getTag());

		return true;
	}

	void MessageNode::onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event) {
		if (keyCode == cocos2d::EventKeyboard::KeyCode::KEY_ESCAPE) {
			GameManager::getInstance()->soundSelect();
			cocos2d::Director::getInstance()->popScene();
		}
	}

}