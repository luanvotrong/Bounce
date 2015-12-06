#include "AppDelegate.h"
#include "HelloWorldScene.h"
#include "GameDefine.h"


USING_NS_CC;

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
	auto director = Director::getInstance();
	auto glview = director->getOpenGLView();
	if (!glview) 
	{
		glview = GLViewImpl::create("My Game");
		glview->setFrameSize(HALF_WIDTH_SCREEN * 2 / 3, HALF_HEIGHT_SCREEN*2/3);
		director->setOpenGLView(glview);
	}

	director->getOpenGLView()->setDesignResolutionSize(WIDTH_SCREEN, HEIGHT_SCREEN, ResolutionPolicy::EXACT_FIT);
    // turn on display FPS
	director->setDisplayStats(false);
	
	srand(time(NULL));

    // set FPS. the default value is 1.0/60 if you don't call this
	director->setAnimationInterval(1.0 / 60);
	
    // create a scene. it's an autorelease object
    Scene *pScene = HelloWorld::createScene();

    // run
	director->runWithScene(pScene);

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    CCDirector::sharedDirector()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    CCDirector::sharedDirector()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
}
