#pragma once
#ifndef __SCORES_SCENE_H__
#define __SCORES_SCENE_H__

#include "cocos2d.h"

namespace TakeTen {
	class ScoresScene : public cocos2d::LayerColor {
	public:
		static cocos2d::Scene* createScene();
		virtual bool init();
		CREATE_FUNC(ScoresScene);

	private:

		void createPlayerLabels();
		void createWorldLabels();

		void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
		void menuBack(Ref* pSender);

		float _delayTimer;
	};
}



#endif // __HELLOWORLD_SCENE_H__
