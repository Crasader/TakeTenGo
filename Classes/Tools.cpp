#include "Tools.h"

namespace TakeTen {
	void Tools::flipInItem(cocos2d::Node* item, float delayTimer, float scaleTo) {
		item->setScaleX(0);
		item->setSkewY(-SKEW_AMOUNT);

		auto delay = cocos2d::DelayTime::create(delayTimer);
		auto actionScale = cocos2d::ScaleTo::create(CARD_FLIP_TIME, scaleTo, scaleTo);
		auto actionSkew = cocos2d::SkewTo::create(CARD_FLIP_TIME, 0, 0);

		auto easyScale = cocos2d::EaseBackOut::create(actionScale);
		auto easySkew = cocos2d::EaseBackOut::create(actionSkew);

		item->runAction(cocos2d::Sequence::create(delay, easyScale, nullptr));
		item->runAction(cocos2d::Sequence::create(delay, easySkew, nullptr));
	}

	void Tools::cropSprite(cocos2d::Sprite* sprite, float amount, /*out*/ cocos2d::Rect& rect) {
		auto isRotated = sprite->isTextureRectRotated();
		rect = sprite->getTextureRect();
		auto width = rect.size.width;
		auto height = rect.size.height;

		height *= amount;

		auto translation = (rect.size.height - height);

		auto startX = rect.origin.x;
		auto startY = rect.origin.y + translation;

		auto newRect = cocos2d::Rect(startX, startY, width, height);
		sprite->setTextureRect(newRect, isRotated, newRect.size);
	}
}

