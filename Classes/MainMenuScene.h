#ifndef __MAIN_MENU_SCENE_H__
#define __MAIN_MENU_SCENE_H__

#include "cocos2d.h"
#include "Size.h"
#include <vector>
#include "network/HttpClient.h"
#include "network/HttpRequest.h"

class MainMenuScene : public cocos2d::LayerColor
{
public:

	
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    // implement the "static create()" method manually
	CREATE_FUNC(MainMenuScene);

	void onHttpRequestCompleted(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);

private:
	void GotoGameSceneMenuItem(Ref* pSender);

	int _difficulty;

	//std::vector<Sprite*> _digits;

};

#endif // __HELLOWORLD_SCENE_H__
