#include "CellSprite.h" 
#include "Definitions.h"

CellSprite::CellSprite() : _digitSprite(nullptr), _value(0), _position(-1,-1), _isSelected(false) {

}

CellSprite::~CellSprite() {
	removeChild(_digitSprite, true);
}

cocos2d::Vec2 CellSprite::cellPositionToVec2(const TakeTen::Position& position, const TakeTen::Size& boardSize) {
	float column = position.getColumn() - (boardSize.getWidht() - 1) / 2.0f;
	float row = position.getRow() - (boardSize.getHeight() - 1) / 2.0f;

	auto vecPostion = cocos2d::Vec2(column * SPRITE_SIZE, -row * SPRITE_SIZE);

	return vecPostion;
}

CellSprite* CellSprite::create(const UCHAR value, const TakeTen::Position& position, const TakeTen::Size& boardSize, const ColorHSB& color) {
	
	CellSprite* cellSprite = new CellSprite();

	if (cellSprite) {
		if (cellSprite->initWithSpriteFrameName("cell.png")) {

			cellSprite->_position = position;

			float column = position.getColumn() - (boardSize.getWidht() - 1) / 2.0f;
			float row = position.getRow() - (boardSize.getHeight() - 1) / 2.0f;

			auto vecPostion = cellPositionToVec2(position, boardSize);

			cellSprite->setPosition(vecPostion);
			cellSprite->setColor(color.toRGB());
			cellSprite->getTexture()->setAntiAliasTexParameters();

			char str[100] = { 0 };
			sprintf(str, "digits_%02d.png", value);
			auto digitSprite = Sprite::createWithSpriteFrameName(str);

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

	setZOrder(10);
	setScaleX(1.0f);
	setRotation(0.0f);

	

	auto actionScaleUp = cocos2d::ScaleTo::create(CARD_FLIP_TIME, 1.3f, 1.3f);
	auto actionScaleDown = cocos2d::ScaleTo::create(CARD_FLIP_TIME / 2, 1.0f, 1.0f);

	auto easyUp = cocos2d::EaseSineIn::create(actionScaleUp);
	auto easyDown = cocos2d::EaseBackOut::create(actionScaleDown);

	auto sequenceScale = cocos2d::Sequence::create(easyUp, easyDown, NULL);

	runAction(sequenceScale);
}

void CellSprite::hide() {

	stopAllActions();

	setScaleX(1.0f);
	setSkewY(0);



	auto actionScale = cocos2d::ScaleTo::create(CARD_FLIP_TIME / 2, 0.0f, 0.0f);
	auto easyScale = cocos2d::EaseBackOut::create(actionScale);
	auto hideFn = cocos2d::CallFunc::create(this, callfunc_selector(cocos2d::Sprite::removeFromParent));
	runAction(cocos2d::Sequence::create(actionScale, hideFn, NULL));
}

void CellSprite::show(float delayTimer) {

	setScaleX(0);
	setSkewY(-SKEW_AMOUNT);

	auto delay = cocos2d::DelayTime::create(delayTimer);
	auto actionScale = cocos2d::ScaleTo::create(CARD_FLIP_TIME, 1.0f, 1.0f);
	auto actionSkew = cocos2d::SkewTo::create(CARD_FLIP_TIME, 0, 0);

	auto easyScale = cocos2d::EaseBackOut::create(actionScale);
	auto easySkew = cocos2d::EaseBackOut::create(actionSkew);

	runAction(cocos2d::Sequence::create(delay, easyScale, NULL));
	runAction(cocos2d::Sequence::create(delay, easySkew, NULL));
}

void CellSprite::select() {

	if (_isSelected) {
		return;
	}

	//stopAllActions();
	auto actionScale = cocos2d::ScaleTo::create(CARD_FLIP_TIME, PRESSED_SCALE, PRESSED_SCALE);
	auto easyScale = cocos2d::EaseBackOut::create(actionScale);
	runAction(cocos2d::Sequence::create(easyScale, NULL));
	_isSelected = true;
}

void CellSprite::unselect() {

	if (!_isSelected) {
		return;
	}
	//stopAllActions();
	auto actionScale = cocos2d::ScaleTo::create(CARD_FLIP_TIME, 1, 1);
	auto easyScale = cocos2d::EaseBackOut::create(actionScale);
	runAction(cocos2d::Sequence::create(easyScale, NULL));
	_isSelected = false;
}

TakeTen::Position& CellSprite::getCellPosition() {
	return _position;
}