#pragma once
#ifndef __MESSAGE_NODE_H__
#define __MESSAGE_NODE_H__

#include "cocos2d.h"
#include "MenuButton.h"


namespace TakeTen {
	class MessageNode : public cocos2d::LayerColor {
	public:
		
		static cocos2d::Scene* createScene(const cocos2d::ccMenuCallback& acceptCallBack, int tag);
		

	private:
		~MessageNode();
		static MessageNode* create(const cocos2d::ccMenuCallback& acceptCallBack, int tag);
		bool init(const cocos2d::ccMenuCallback& acceptCallBack);
		void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);

		MenuButton* _backMenu;
		MenuButton* _acceptMenu;
	};
}
#endif