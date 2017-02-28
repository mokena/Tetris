#include "MainGameScene.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

Scene* MainGame::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = MainGame::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool MainGame::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
	//Layout UI
	initUI();

	//Start game
	startGame();
    
    return true;
}

/*Initiate the UI and layout*/
void MainGame::initUI() {
	auto visibleSize = Director::getInstance()->getVisibleSize();
	blockHeight = visibleSize.height / 20;
	blockWidth = blockHeight;
}

/*When player come into this scene, start game immediately*/
void MainGame::startGame() {

}

void MainGame::menuCloseCallback(Ref* pSender)
{
    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
