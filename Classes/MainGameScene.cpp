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
	blockWidth = blockHeight = BLOCKW;

	for (int i = 0; i < ROW; i++) {
		for (int j = 0; j < COLUMN; j++) {
			allBlocks[i][j] = nullptr;
		}
	}

	for (int i = 0; i < TNUM; i++) {
		curTetris[i] = nullptr;
		nextTetris[i] = nullptr;
	}

	// background
	Sprite* bg = Sprite::create("bg.png");
	bg->setAnchorPoint(Vec2(0, 0));
	bg->setPosition(ccp(0, 0));
	addChild(bg);
	auto testsize = bg->getContentSize();
	int i = 0;
}


/*When player come into this scene, start game immediately*/
void MainGame::startGame() {
	randomTetris();
}

void MainGame::randomTetris()
{
	int rand = CCRANDOM_0_1()*(TSTYLE - 1);
	for (int i = 0; i < TNUM; i++) {
		Sprite* block = Sprite::create("block.png");
		nextTetris[i] = block;
		addChild(block);
		auto testsize = block->getContentSize();
		int a = 0;
	}
	switch (rand) {
	case 0: //I
		nextTetris[0]->setPosition(ccp(640 - BLOCKW * 4, BLOCKW * 5));
		nextTetris[1]->setPosition(ccp(640 - BLOCKW * 4, BLOCKW * 6));
		nextTetris[2]->setPosition(ccp(640 - BLOCKW * 4, BLOCKW * 7));
		nextTetris[3]->setPosition(ccp(640 - BLOCKW * 4, BLOCKW * 8));
		break;
	case 1: //L
		nextTetris[0]->setPosition(ccp(640 - BLOCKW * 4, BLOCKW * 6));
		nextTetris[1]->setPosition(ccp(640 - BLOCKW * 4, BLOCKW * 7));
		nextTetris[2]->setPosition(ccp(640 - BLOCKW * 4, BLOCKW * 8));
		nextTetris[3]->setPosition(ccp(640 - BLOCKW * 3, BLOCKW * 8));
		break;
	case 2: //J
		nextTetris[0]->setPosition(ccp(640 - BLOCKW * 3, BLOCKW * 6));
		nextTetris[1]->setPosition(ccp(640 - BLOCKW * 3, BLOCKW * 7));
		nextTetris[2]->setPosition(ccp(640 - BLOCKW * 3, BLOCKW * 8));
		nextTetris[3]->setPosition(ccp(640 - BLOCKW * 4, BLOCKW * 8));
		break;
	case 3: //Z
		nextTetris[0]->setPosition(ccp(640 - BLOCKW * 4, BLOCKW * 7));
		nextTetris[1]->setPosition(ccp(640 - BLOCKW * 3, BLOCKW * 7));
		nextTetris[2]->setPosition(ccp(640 - BLOCKW * 3, BLOCKW * 8));
		nextTetris[3]->setPosition(ccp(640 - BLOCKW * 2, BLOCKW * 8));
		break;
	case 4: //S
		nextTetris[0]->setPosition(ccp(640 - BLOCKW * 2, BLOCKW * 7));
		nextTetris[1]->setPosition(ccp(640 - BLOCKW * 2, BLOCKW * 6));
		nextTetris[2]->setPosition(ccp(640 - BLOCKW * 3, BLOCKW * 6));
		nextTetris[3]->setPosition(ccp(640 - BLOCKW * 3, BLOCKW * 5));
		break;
	case 5: //O
		nextTetris[0]->setPosition(ccp(640 - BLOCKW * 4, BLOCKW * 7));
		nextTetris[1]->setPosition(ccp(640 - BLOCKW * 4, BLOCKW * 8));
		nextTetris[2]->setPosition(ccp(640 - BLOCKW * 3, BLOCKW * 7));
		nextTetris[3]->setPosition(ccp(640 - BLOCKW * 3, BLOCKW * 8));
		break;
	case 6: //T
		nextTetris[0]->setPosition(ccp(640 - BLOCKW * 4, BLOCKW * 7));
		nextTetris[1]->setPosition(ccp(640 - BLOCKW * 3, BLOCKW * 7));
		nextTetris[2]->setPosition(ccp(640 - BLOCKW * 2, BLOCKW * 7));
		nextTetris[3]->setPosition(ccp(640 - BLOCKW * 3, BLOCKW * 8));
		break;
	default:
		break;
	}
}

void MainGame::nextToCur()
{
	for (int i = 0; i < TNUM; i++) {
		curTetris[i] = nextTetris[i];
	}
}

void MainGame::menuCloseCallback(Ref* pSender)
{
    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
