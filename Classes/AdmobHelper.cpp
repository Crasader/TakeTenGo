#include "AdmobHelper.h"
#include "cocos2d.h"

bool AdmobHelper::isAdShowing = true;

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

#include "platform/android/jni/JniHelper.h"
#include <jni.h>

const char* AppActivityClassName = "org/cocos2dx/cpp/AppActivity";

void AdmobHelper::hideAd() {
    cocos2d::JniMethodInfo t;
    if (cocos2d::JniHelper::getStaticMethodInfo(t, AppActivityClassName, "hideAd", "()V")) {
        t.env->CallStaticVoidMethod(t.classID, t.methodID);
        t.env->DeleteLocalRef(t.classID);
        isAdShowing = false;
    }
}

void AdmobHelper::showFullscreenAd() {
    cocos2d::JniMethodInfo t;
    if (cocos2d::JniHelper::getStaticMethodInfo(t, AppActivityClassName, "showFullScreenAd", "()V")) {
        t.env->CallStaticVoidMethod(t.classID, t.methodID);
        t.env->DeleteLocalRef(t.classID);
    }
}

void AdmobHelper::showAd() {

    cocos2d::JniMethodInfo t;
    if (cocos2d::JniHelper::getStaticMethodInfo(t, AppActivityClassName, "showAd", "()V")) {

        t.env->CallStaticVoidMethod(t.classID, t.methodID);
        t.env->DeleteLocalRef(t.classID);
        isAdShowing = true;
    }

}


#else


void AdmobHelper::hideAd() {
    isAdShowing = false;
}


void AdmobHelper::showAd() {
    isAdShowing = true;
}

void AdmobHelper::showFullscreenAd() { 
	
}

#endif
