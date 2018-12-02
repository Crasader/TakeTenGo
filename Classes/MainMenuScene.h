#pragma once
#ifndef __MAIN_MENU_SCENE_H__
#define __MAIN_MENU_SCENE_H__

#include <vector>
#include "cocos2d.h"

namespace TakeTen {
	class MainMenuScene : public cocos2d::LayerColor {
	public:
		static cocos2d::Scene* createScene();
		virtual bool init();
		CREATE_FUNC(MainMenuScene);

		void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);

	private:
		void gotoHelpScene(cocos2d::Ref* pSender);
		void gotoScoreScene(cocos2d::Ref* pSender);
		void gotoRate(cocos2d::Ref* pSender);
		void gotoGameScene(cocos2d::Ref* pSender);

		int _difficulty;
		cocos2d::SpriteBatchNode* _spriteBatch;
		cocos2d::Menu* _mainMenu;
		cocos2d::MenuItemSprite* _lastMenuItem;
		std::vector<cocos2d::Node*> _lastItems;
	};
}



#endif
