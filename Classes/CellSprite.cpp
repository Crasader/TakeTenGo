#include "CellSprite.h" 
#include "Definitions.h"
#include "Tools.h"

namespace TakeTen {
	CellSprite::CellSprite() : _digitSprite(nullptr), _value(0), _position(-1, -1), _isSelected(false) {

	}

	CellSprite::~CellSprite() {
		removeChild(_digitSprite, true);
	}

	cocos2d::Vec2 CellSprite::cellPositionToVec2(const Position& position, const Size& boardSize) {
		float column = position.column - (boardSize.width - 1) / 2.0f;
		float row = position.row - (boardSize.height - 1) / 2.0f;

		auto vecPostion = cocos2d::Vec2(column * SPRITE_SIZE, -row * SPRITE_SIZE);

		return vecPostion;
	}

	CellSprite* CellSprite::create(const unsigned char value, const Position& position, const Size& boardSize, const ColorHSB& color) {

		auto cellSprite = new (std::nothrow) CellSprite();

		if (cellSprite) {
			if (cellSprite->initWithSpriteFrameName(CELL_SPRITE)) {
				cellSprite->autorelease();

				cellSprite->_position = position;

				float column = position.column - (boardSize.width - 1) / 2.0f;
				float row = position.row - (boardSize.height - 1) / 2.0f;

				cellSprite->_zOrder = -10 * (boardSize.height - position.row + 1);

				cellSprite->_cellPosition = position;
				cellSprite->_basePosition = cellPositionToVec2(position, boardSize);

				cellSprite->setPosition(cellSprite->_basePosition);
				cellSprite->setColor(color.toRGB());

				char buff[100];
				sprintf(buff, CELL_DIGITS, value);
				std::string buffAsStdStr(buff);

				auto digitSprite = cocos2d::Sprite::createWithSpriteFrameName(buffAsStdStr);

				digitSprite->setPosition(SPRITE_SIZE / 2, SPRITE_SIZE / 2);
				digitSprite->getTexture()->setAntiAliasTexParameters();
				cellSprite->addChild(digitSprite, 1);

				cellSprite->_value = value;
				cellSprite->_digitSprite = digitSprite;

				return cellSprite;
			}

		}
		CCLOG("ERROR CREATING SPRITE");
		CC_SAFE_DELETE(cellSprite);
		return nullptr;
	}

	void CellSprite::pop() {

		stopAllActions();

		setLocalZOrder(10);
		setScaleX(1.0f);
		setRotation(0.0f);

		auto actionScaleUp = cocos2d::ScaleTo::create(CARD_FLIP_TIME, 1.3f, 1.3f);
		auto actionScaleDown = cocos2d::ScaleTo::create(CARD_FLIP_TIME / 2, 1.0f, 1.0f);

		auto easyUp = cocos2d::EaseSineIn::create(actionScaleUp);
		auto easyDown = cocos2d::EaseBackOut::create(actionScaleDown);

		auto sequenceScale = cocos2d::Sequence::create(easyUp, easyDown, nullptr);

		runAction(sequenceScale);
	}

	void CellSprite::hide(float delayTimer) {

		stopAllActions();

		setPosition(_basePosition);
		setLocalZOrder(0);
		setScaleX(1.0f);
		setSkewY(0);

		auto delay = cocos2d::DelayTime::create(delayTimer);

		auto actionScale = cocos2d::ScaleTo::create(CARD_FLIP_TIME / 2, 0.0f, 1.0f);
		auto actionSkew = cocos2d::SkewTo::create(CARD_FLIP_TIME / 2, 0, SKEW_AMOUNT);

		auto easyScale = cocos2d::EaseBackIn::create(actionScale);
		auto easySkew = cocos2d::EaseBackIn::create(actionSkew);

		auto hideFn = cocos2d::CallFunc::create(CC_CALLBACK_0(cocos2d::Sprite::removeFromParent, this));

		runAction(cocos2d::Sequence::create(delay, easyScale, nullptr));
		runAction(cocos2d::Sequence::create(delay, easySkew, hideFn, nullptr));
	}

	void CellSprite::show(float delayTimer) {

		setLocalZOrder(0);
		setScaleX(0);
		setSkewY(-SKEW_AMOUNT);

		auto delay = cocos2d::DelayTime::create(delayTimer);
		auto actionScale = cocos2d::ScaleTo::create(CARD_FLIP_TIME, 1.0f, 1.0f);
		auto actionSkew = cocos2d::SkewTo::create(CARD_FLIP_TIME, 0, 0);

		auto easyScale = cocos2d::EaseBackOut::create(actionScale);
		auto easySkew = cocos2d::EaseBackOut::create(actionSkew);

		runAction(cocos2d::Sequence::create(delay, easyScale, nullptr));
		runAction(cocos2d::Sequence::create(delay, easySkew, nullptr));
	}

	void CellSprite::select() {

		if (_isSelected) {
			return;
		}

		Sprite::setLocalZOrder(_zOrder);
		stopAllActions();

		auto actionMove = cocos2d::MoveTo::create(CARD_FLIP_TIME, _basePosition - cocos2d::Vec2(0, 30.0f));
		auto easyMove = cocos2d::EaseBackOut::create(actionMove);

		auto actionScale = cocos2d::ScaleTo::create(CARD_FLIP_TIME, PRESSED_SCALE, PRESSED_SCALE);
		auto easyScale = cocos2d::EaseBackOut::create(actionScale);

		runAction(cocos2d::Sequence::create(easyMove, nullptr));
		runAction(cocos2d::Sequence::create(easyScale, nullptr));

		_isSelected = true;
	}

	void CellSprite::unselect() {

		if (!_isSelected) {
			return;
		}
		stopAllActions();

		//auto hideFn = cocos2d::CallFuncN::create(CC_CALLBACK_0(cocos2d::Sprite::removeFromParent, this));
		auto zOrderAction = cocos2d::CallFunc::create(CC_CALLBACK_0(CellSprite::resetZOrder, this));

		auto actionMove = cocos2d::MoveTo::create(CARD_FLIP_TIME, _basePosition);
		auto easyMove = cocos2d::EaseBackOut::create(actionMove);

		auto actionScale = cocos2d::ScaleTo::create(CARD_FLIP_TIME, 1, 1);
		auto easyScale = cocos2d::EaseBackOut::create(actionScale);

		runAction(cocos2d::Sequence::create(easyMove, nullptr));
		runAction(cocos2d::Sequence::create(easyScale, zOrderAction, nullptr));
		_isSelected = false;
	}

	void CellSprite::resetZOrder() {
		cocos2d::Sprite::setLocalZOrder(0);
	}

	Position& CellSprite::getCellPosition() {
		return _position;
	}
}

