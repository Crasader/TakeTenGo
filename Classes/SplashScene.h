#pragma once
#ifndef __SPLASH_SCENE_H__
#define __SPLASH_SCENE_H__

#include "cocos2d.h"

namespace TakeTen {
	class SplashScene : public cocos2d::LayerColor {
	public:
		static cocos2d::Scene* createScene();
		virtual bool init();
		CREATE_FUNC(SplashScene);

	private:
		void MainMenuLoader(float dt);


	};
}



#endif // __HELLOWORLD_SCENE_H__
