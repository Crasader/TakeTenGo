#include <time.h>

#include "SimpleAudioEngine.h"

#include "Definitions.h"
#include "GameManager.h"
#include "SplashScene.h"
#include "AppDelegate.h"

#include "BoardManager.h"

#include "MainMenuScene.h"

USING_NS_CC;

AppDelegate::AppDelegate() { }
AppDelegate::~AppDelegate()  { }

struct Resource {
	cocos2d::Size size;
	char directory[100];
};

static Resource smallResource = { cocos2d::Size(480, 800), "small" };
static Resource mediumResource = { cocos2d::Size(720, 1280), "medium" };
static Resource largeResource = { cocos2d::Size(1080, 1920), "large" };

void AppDelegate::initGLContextAttrs() {
	GLContextAttrs glContextAttrs = { 8, 8, 8, 8, 24, 8 };
	GLView::setGLContextAttrs(glContextAttrs);
}

bool AppDelegate::applicationDidFinishLaunching() {

	std::vector<std::string> searchPaths;
	searchPaths.push_back("localisation");
	auto director = Director::getInstance();
	director->setDisplayStats(false);
	director->setAnimationInterval(1.0 / 60);
	auto glview = director->getOpenGLView();

	auto designSize = Size(1080, 1920);
	

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) 
	TakeTen::BoardManager::getInstance();
	if (!glview) {
		glview = GLViewImpl::create("Take Ten: Go!");
		director->setOpenGLView(glview);
	}
	searchPaths.push_back(largeResource.directory);
	director->setContentScaleFactor(largeResource.size.height / designSize.height);
	glview->setDesignResolutionSize(designSize.width, designSize.height, ResolutionPolicy::FIXED_HEIGHT);
	cocos2d::FileUtils::getInstance()->setSearchPaths(searchPaths);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) 
	if (!glview) {
		glview = GLViewImpl::create("Take Ten: Go!");
		director->setOpenGLView(glview);
	}
	auto height = 850.0f;
	auto width = height * 9.0f / 16.0f;
	glview->setFrameSize(width, height);
	searchPaths.push_back(largeResource.directory);
	director->setContentScaleFactor(largeResource.size.height / designSize.height);
	glview->setDesignResolutionSize(designSize.width, designSize.height, ResolutionPolicy::FIXED_HEIGHT);
	cocos2d::FileUtils::getInstance()->setSearchPaths(searchPaths);
#else
	if (!glview) {
		glview = GLViewImpl::create("Take Ten: Go!");
		director->setOpenGLView(glview);
	}
	auto frameSize = director->getWinSizeInPixels();
	
	if (frameSize.height > mediumResource.size.height) {
		CCLOG("LARGE SIZE RESOURCE");
		searchPaths.push_back(largeResource.directory);
		director->setContentScaleFactor(largeResource.size.height / designSize.height);
	}
	else if (frameSize.height > smallResource.size.height) {
		CCLOG("MEDIUM SIZE RESOURCE");
		searchPaths.push_back(mediumResource.directory);
		director->setContentScaleFactor(mediumResource.size.height / designSize.height);
	}
	else {
		CCLOG("SMALL SIZE RESOURCE");
		searchPaths.push_back(smallResource.directory);
		director->setContentScaleFactor(smallResource.size.height / designSize.height);
	}
	glview->setDesignResolutionSize(designSize.width, designSize.height, ResolutionPolicy::FIXED_HEIGHT);
	cocos2d::FileUtils::getInstance()->setSearchPaths(searchPaths);
#endif

#if GENERATE_BOARDS
	return true;
#else
	auto scene = TakeTen::SplashScene::createScene();
	director->runWithScene(scene);
    return true;
#endif
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
	CCLOG("BACKGROUND MODE");
    Director::getInstance()->stopAnimation();
	TakeTen::GameManager::getInstance()->save();
	TakeTen::GameManager::getInstance()->setPause();
	CocosDenshion::SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();
	TakeTen::GameManager::getInstance()->setPause(false);
	CocosDenshion::SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}
