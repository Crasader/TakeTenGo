#include <vector>
#include "Definitions.h"
#include "BackgroundNode.h" 

namespace TakeTen {

	float BackgroundNode::_yPos = -1980 * 3;

	BackgroundNode::BackgroundNode() {
		_height = 800;
		_rectangle = nullptr;
	}

	BackgroundNode::~BackgroundNode() {
		removeAllChildrenWithCleanup(true);
	}

	bool BackgroundNode::init() {

		auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
		auto width = visibleSize.width;
		_height = visibleSize.height * 2;

		std::vector<cocos2d::Vec2> vertices;

		_rectangle = cocos2d::DrawNode::create();

		vertices.push_back(cocos2d::Vec2(0, 0));
		vertices.push_back(cocos2d::Vec2(width, width));
		vertices.push_back(cocos2d::Vec2(width, width + _height));
		vertices.push_back(cocos2d::Vec2(0, _height));

		_rectangle->drawSolidPoly(&vertices[0], vertices.size(), cocos2d::Color4F(0.23f, 0.23f, 0.23f, 1));
		auto pos = cocos2d::Vec2(0, _yPos);
		_rectangle->setPosition(pos);
		this->addChild(_rectangle);

		vertices.clear();

		scheduleUpdate();
				
		return true;
	}

	void BackgroundNode::update(float dt) {
		auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
		_yPos += dt * BG_SPEED;
		if (_yPos > visibleSize.height + _height) {
			_yPos = -visibleSize.height - _height;
		}
		auto pos = _rectangle->getPosition();
		pos.y = _yPos;
		_rectangle->setPosition(pos);
	}

}