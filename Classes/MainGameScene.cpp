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

	// vertical line
	Sprite* verticalLine = Sprite::create("vertical_line.png");
	addChild(verticalLine);
	verticalLine->setAnchorPoint(Vec2(0, 0));
	verticalLine->setPosition(Vec2(10 * BLOCKW + 2, 5));
}


/* When player come into this scene, start game immediately*/
void MainGame::startGame() {
	srand(time(nullptr));
	randomTetris();
	nextToCur();
	schedule(schedule_selector(MainGame::moveUpdate), 0.5f);
}

/* Generate the next tetris units by random */
void MainGame::randomTetris()
{
	//
	int rand = CCRANDOM_0_1()*(TSTYLE - 1);
	for (int i = 0; i < TNUM; i++) {
		Sprite* block = Sprite::create("block.png");
		nextTetris[i] = block;
		addChild(block);
		nextTetris[i]->setAnchorPoint(Vec2(0, 0));
	}
	switch (rand) {
	case 0: //I
		nextTetris[0]->setPosition(ccp(640 - BLOCKW * 4, 960 - BLOCKW * 5));
		nextTetris[1]->setPosition(ccp(640 - BLOCKW * 4, 960 - BLOCKW * 6));
		nextTetris[2]->setPosition(ccp(640 - BLOCKW * 4, 960 - BLOCKW * 7));
		nextTetris[3]->setPosition(ccp(640 - BLOCKW * 4, 960 - BLOCKW * 8));
		break;
	case 1: //L
		nextTetris[0]->setPosition(ccp(640 - BLOCKW * 4, 960 - BLOCKW * 6));
		nextTetris[1]->setPosition(ccp(640 - BLOCKW * 4, 960 - BLOCKW * 7));
		nextTetris[2]->setPosition(ccp(640 - BLOCKW * 4, 960 - BLOCKW * 8));
		nextTetris[3]->setPosition(ccp(640 - BLOCKW * 3, 960 - BLOCKW * 8));
		break;
	case 2: //J
		nextTetris[0]->setPosition(ccp(640 - BLOCKW * 3, 960 - BLOCKW * 6));
		nextTetris[1]->setPosition(ccp(640 - BLOCKW * 3, 960 - BLOCKW * 7));
		nextTetris[2]->setPosition(ccp(640 - BLOCKW * 3, 960 - BLOCKW * 8));
		nextTetris[3]->setPosition(ccp(640 - BLOCKW * 4, 960 - BLOCKW * 8));
		break;
	case 3: //Z
		nextTetris[0]->setPosition(ccp(640 - BLOCKW * 4, 960 - BLOCKW * 7));
		nextTetris[1]->setPosition(ccp(640 - BLOCKW * 3, 960 - BLOCKW * 7));
		nextTetris[2]->setPosition(ccp(640 - BLOCKW * 3, 960 - BLOCKW * 8));
		nextTetris[3]->setPosition(ccp(640 - BLOCKW * 2, 960 - BLOCKW * 8));
		break;
	case 4: //S
		nextTetris[0]->setPosition(ccp(640 - BLOCKW * 2, 960 - BLOCKW * 7));
		nextTetris[1]->setPosition(ccp(640 - BLOCKW * 2, 960 - BLOCKW * 6));
		nextTetris[2]->setPosition(ccp(640 - BLOCKW * 3, 960 - BLOCKW * 6));
		nextTetris[3]->setPosition(ccp(640 - BLOCKW * 3, 960 - BLOCKW * 5));
		break;
	case 5: //O
		nextTetris[0]->setPosition(ccp(640 - BLOCKW * 4, 960 - BLOCKW * 7));
		nextTetris[1]->setPosition(ccp(640 - BLOCKW * 4, 960 - BLOCKW * 8));
		nextTetris[2]->setPosition(ccp(640 - BLOCKW * 3, 960 - BLOCKW * 7));
		nextTetris[3]->setPosition(ccp(640 - BLOCKW * 3, 960 - BLOCKW * 8));
		break;
	case 6: //T
		nextTetris[0]->setPosition(ccp(640 - BLOCKW * 4, 960 - BLOCKW * 7));
		nextTetris[1]->setPosition(ccp(640 - BLOCKW * 3, 960 - BLOCKW * 7));
		nextTetris[2]->setPosition(ccp(640 - BLOCKW * 2, 960 - BLOCKW * 7));
		nextTetris[3]->setPosition(ccp(640 - BLOCKW * 3, 960 - BLOCKW * 8));
		break;
	default:
		break;
	}
}

/* Turn the next tetris to current dropping tetris */
void MainGame::nextToCur()
{
	for (int i = 0; i < TNUM; i++) {
		curTetris[i] = nextTetris[i];
		Vec2 pos = curTetris[i]->getPosition();
		curTetris[i]->setPosition(Vec2(pos.x - 7 * BLOCKW, pos.y + 9 * BLOCKW));
	}

	randomTetris();
}

/* when game started, move the current blocks */
void MainGame::moveUpdate(float dt)
{
	if (touchCheck(DIRECTION_DOWN)) {
		for (int i = 0; i < TNUM; i++) {
			Vec2 pos = curTetris[i]->getPosition();
			curTetris[i]->setPosition(Vec2(pos.x, pos.y - 1 * BLOCKW));
		}
	}
	else {
		// add all current blocks to all blocks array
		for (int i = 0; i < TNUM; i++) {
			Vec2 pos = curTetris[i]->getPosition();
			int column = (int)(pos.x / BLOCKW);
			int row = (int)(pos.y / BLOCKW);
			allBlocks[column][row] = curTetris[i];
		}

		nextToCur();
	}
}

/* when game started, check the current blocks is touching 
   other motionless blocks or the walls, if can move on, return
   true, otherwise return false */
boolean MainGame::touchCheck(int direction)
{
	for (int i = 0; i < TNUM; i++) {
		Vec2 pos = curTetris[i]->getPosition();
		int row = 0, column = 0;
		switch (direction) {
		case DIRECTION_DOWN:
			row = (int)((pos.y - BLOCKW) / BLOCKW);
			column = (int)(pos.x / BLOCKW);
			if (row < ROW && allBlocks[column][row] != nullptr) return false;
			if (pos.y - BLOCKW < 0) return false;
			break;
		case DIRECTION_LEFT:
			if (pos.x - BLOCKW < 0) return false;
			break;
		case DIRECTION_RIGHT:
			if (pos.x + BLOCKW > (10 * BLOCKW + 2)) return false;
			break;
		default:
			break;
		}
	}
	return true;
}

void MainGame::menuCloseCallback(Ref* pSender)
{
    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
