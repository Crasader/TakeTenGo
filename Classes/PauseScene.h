#pragma once
#ifndef __PAUSE_SCENE_H__
#define __PAUSE_SCENE_H__

#include "cocos2d.h"
#include "MenuButton.h"

namespace TakeTen {
	class PauseScene : public cocos2d::LayerColor {
	public:
		static cocos2d::Scene* createScene();
		virtual bool init();
		CREATE_FUNC(PauseScene);

		void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);

	private:
		void menuExit(Ref* pSender);
		void menuPlay(Ref* pSender);
		void menuSound(Ref* pSender);

		MenuButton* _soundOnButton;
		MenuButton* _soundOffButton;

	};
}
#endif