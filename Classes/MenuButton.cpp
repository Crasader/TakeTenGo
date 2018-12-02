#include "MenuButton.h"

namespace TakeTen {
	MenuButton::~MenuButton() {
		removeFromParentAndCleanup(true);
	}

	MenuButton* MenuButton::create(const std::string& sprite, const std::string& pressedSprite, const ColorHSB& pressedColor, const cocos2d::ccMenuCallback& callBack) {
		auto icon = cocos2d::Sprite::createWithSpriteFrameName(sprite);
		auto invertedIcon = cocos2d::Sprite::createWithSpriteFrameName(pressedSprite);
		invertedIcon->setColor(pressedColor.toRGB());

		auto ret = new (std::nothrow) MenuButton();
		if (ret->initWithNormalSprite(icon, invertedIcon, nullptr, callBack)) {
			ret->autorelease();
			return ret;
		}
		CCLOG("ERROR CREATING BOARD NODE");
		CC_SAFE_DELETE(ret);
		return nullptr;
	}

	MenuButton* MenuButton::create(const std::string& sprite, const ColorHSB& pressedColor, const float delayTimer, const cocos2d::ccMenuCallback& callBack) {
		auto icon = cocos2d::Sprite::createWithSpriteFrameName(sprite);
		auto invertedIcon = cocos2d::Sprite::createWithSpriteFrameName(sprite);
		invertedIcon->setColor(pressedColor.toRGB());

		auto ret = new (std::nothrow) MenuButton();
		ret->initWithNormalSprite(icon, invertedIcon, nullptr, callBack);
		ret->autorelease();

		ret->setScaleX(0);
		ret->setSkewY(-SKEW_AMOUNT);

		auto delay = cocos2d::DelayTime::create(delayTimer);
		auto actionScale = cocos2d::ScaleTo::create(CARD_FLIP_TIME, 1.0f, 1.0f);
		auto actionSkew = cocos2d::SkewTo::create(CARD_FLIP_TIME, 0, 0);

		auto easyScale = cocos2d::EaseBackOut::create(actionScale);
		auto easySkew = cocos2d::EaseBackOut::create(actionSkew);

		ret->runAction(cocos2d::Sequence::create(delay, easyScale, nullptr));
		ret->runAction(cocos2d::Sequence::create(delay, easySkew, nullptr));

		return ret;
	}

	void MenuButton::enable() {
		setEnabled(true);
		setOpacity(255);
	}

	void MenuButton::disable() {
		setEnabled(false);
		setOpacity(30);
	}


}
