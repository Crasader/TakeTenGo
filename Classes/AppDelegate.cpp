#include "AppDelegate.h"
#include "SplashScene.h"
#include "GameManager.h"
#include <time.h>


USING_NS_CC;

AppDelegate::AppDelegate() {
	
}

AppDelegate::~AppDelegate() 
{
}

typedef struct tagResource {
	cocos2d::Size size;
	char directory[100];
} Resource;

static Resource smallResource = { cocos2d::Size(480, 800), "small" };
static Resource mediumResource = { cocos2d::Size(720, 1280), "medium" };
static Resource largeResource = { cocos2d::Size(1080, 1920), "large" };

bool AppDelegate::applicationDidFinishLaunching() {

	auto designSize = Size(1080, 1920);

    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
        glview = GLView::create("Take Ten: Go!");
        director->setOpenGLView(glview);
    }

	std::vector<std::string> searchPaths;

	auto frameSize = director->getWinSize();

	CCLOG("FRAME SIZE %f, %f", frameSize.width, frameSize.height);

	if (frameSize.height > mediumResource.size.height)
	{
		CCLOG("LARGE SIZE RESOURCE");
		searchPaths.push_back(largeResource.directory);
		director->setContentScaleFactor(largeResource.size.height / designSize.height);
	}
	// if the frame's height is larger than the height of small resource size, select medium resource.
	else if (frameSize.height > smallResource.size.height)
	{
		CCLOG("MEDIUM SIZE RESOURCE");
		searchPaths.push_back(mediumResource.directory);
		director->setContentScaleFactor(mediumResource.size.height / designSize.height);
	}
	// if the frame's height is smaller than the height of medium resource size, select small resource.
	else
	{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
		CCLOG("SMALL SIZE RESOURCE");
		searchPaths.push_back(mediumResource.directory);
		director->setContentScaleFactor(mediumResource.size.height / designSize.height);
#else
		CCLOG("SMALL SIZE RESOURCE");
		searchPaths.push_back(smallResource.directory);
		director->setContentScaleFactor(smallResource.size.height / designSize.height);
#endif
		
	}

	glview->setDesignResolutionSize(designSize.width, designSize.height, ResolutionPolicy::FIXED_HEIGHT);

	cocos2d::FileUtils::getInstance()->setSearchPaths(searchPaths);

    // turn on display FPS
    director->setDisplayStats(true);
    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 60);
    // create a scene. it's an autorelease object

	auto scene = SplashScene::createScene();
    director->runWithScene(scene);

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

	TakeTen::GameManager::getInstance()->save();

	CCLOG("BACKGROUND MODE");

    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}
