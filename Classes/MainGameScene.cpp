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
	Sprite* bg = Sprite::create("bg_1.png");
	bg->setAnchorPoint(Vec2(0, 0));
	bg->setPosition(Vec2(0, 0));
	addChild(bg);
	auto testsize = bg->getContentSize();
	int i = 0;

	// vertical line
	Sprite* verticalLine = Sprite::create("vertical_line.png");
	addChild(verticalLine);
	verticalLine->setAnchorPoint(Vec2(0, 0));
	verticalLine->setPosition(Vec2(10 * BLOCKW + 2, 5));

	// keys 
	upBtn = ui::Button::create("key_red.png");
	upBtn->setAnchorPoint(Vec2(0.5f, 0.5f));
	upBtn->setPosition(Vec2(550, GAME_HEIGHT - 760));
	addChild(upBtn);
	upBtn->addClickEventListener(CC_CALLBACK_1(MainGame::clickUp, this));

	downBtn = ui::Button::create("key_red.png");
	downBtn->setAnchorPoint(Vec2(0.5f, 0.5f));
	downBtn->setPosition(Vec2(550, GAME_HEIGHT - 860));
	addChild(downBtn);
	downBtn->addClickEventListener(CC_CALLBACK_1(MainGame::clickDown, this));

	leftBtn = ui::Button::create("key_black.png");
	leftBtn->setAnchorPoint(Vec2(0.5f, 0.5f));
	leftBtn->setPosition(Vec2(500, GAME_HEIGHT - 810));
	addChild(leftBtn);
	leftBtn->addClickEventListener(CC_CALLBACK_1(MainGame::clickLeft, this));

	rightBtn = ui::Button::create("key_black.png");
	rightBtn->setAnchorPoint(Vec2(0.5f, 0.5f));
	rightBtn->setPosition(Vec2(600, GAME_HEIGHT - 810));
	addChild(rightBtn);
	rightBtn->addClickEventListener(CC_CALLBACK_1(MainGame::clickRight, this));
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
		nextTetris[0]->setPosition(ccp(GAME_WIDTH  - BLOCKW * 4, GAME_HEIGHT - BLOCKW * 6));
		nextTetris[1]->setPosition(ccp(GAME_WIDTH  - BLOCKW * 4, GAME_HEIGHT - BLOCKW * 7));
		nextTetris[2]->setPosition(ccp(GAME_WIDTH  - BLOCKW * 4, GAME_HEIGHT - BLOCKW * 8));
		nextTetris[3]->setPosition(ccp(GAME_WIDTH  - BLOCKW * 4, GAME_HEIGHT - BLOCKW * 9));
		break;
	case 1: //L
		nextTetris[0]->setPosition(ccp(GAME_WIDTH  - BLOCKW * 4, GAME_HEIGHT - BLOCKW * 7));
		nextTetris[1]->setPosition(ccp(GAME_WIDTH  - BLOCKW * 4, GAME_HEIGHT - BLOCKW * 8));
		nextTetris[2]->setPosition(ccp(GAME_WIDTH  - BLOCKW * 4, GAME_HEIGHT - BLOCKW * 9));
		nextTetris[3]->setPosition(ccp(GAME_WIDTH  - BLOCKW * 3, GAME_HEIGHT - BLOCKW * 9));
		break;
	case 2: //J
		nextTetris[0]->setPosition(ccp(GAME_WIDTH  - BLOCKW * 3, GAME_HEIGHT - BLOCKW * 7));
		nextTetris[1]->setPosition(ccp(GAME_WIDTH  - BLOCKW * 3, GAME_HEIGHT - BLOCKW * 8));
		nextTetris[2]->setPosition(ccp(GAME_WIDTH  - BLOCKW * 3, GAME_HEIGHT - BLOCKW * 9));
		nextTetris[3]->setPosition(ccp(GAME_WIDTH  - BLOCKW * 4, GAME_HEIGHT - BLOCKW * 9));
		break;
	case 3: //Z
		nextTetris[0]->setPosition(ccp(GAME_WIDTH  - BLOCKW * 4, GAME_HEIGHT - BLOCKW * 7));
		nextTetris[1]->setPosition(ccp(GAME_WIDTH  - BLOCKW * 3, GAME_HEIGHT - BLOCKW * 7));
		nextTetris[2]->setPosition(ccp(GAME_WIDTH  - BLOCKW * 3, GAME_HEIGHT - BLOCKW * 8));
		nextTetris[3]->setPosition(ccp(GAME_WIDTH  - BLOCKW * 2, GAME_HEIGHT - BLOCKW * 8));
		break;
	case 4: //S
		nextTetris[0]->setPosition(ccp(GAME_WIDTH  - BLOCKW * 2, GAME_HEIGHT - BLOCKW * 8));
		nextTetris[1]->setPosition(ccp(GAME_WIDTH  - BLOCKW * 2, GAME_HEIGHT - BLOCKW * 7));
		nextTetris[2]->setPosition(ccp(GAME_WIDTH  - BLOCKW * 3, GAME_HEIGHT - BLOCKW * 7));
		nextTetris[3]->setPosition(ccp(GAME_WIDTH  - BLOCKW * 3, GAME_HEIGHT - BLOCKW * 6));
		break;
	case 5: //O
		nextTetris[0]->setPosition(ccp(GAME_WIDTH  - BLOCKW * 4, GAME_HEIGHT - BLOCKW * 7));
		nextTetris[1]->setPosition(ccp(GAME_WIDTH  - BLOCKW * 4, GAME_HEIGHT - BLOCKW * 8));
		nextTetris[2]->setPosition(ccp(GAME_WIDTH  - BLOCKW * 3, GAME_HEIGHT - BLOCKW * 7));
		nextTetris[3]->setPosition(ccp(GAME_WIDTH  - BLOCKW * 3, GAME_HEIGHT - BLOCKW * 8));
		break;
	case 6: //T
		nextTetris[0]->setPosition(ccp(GAME_WIDTH  - BLOCKW * 4, GAME_HEIGHT - BLOCKW * 7));
		nextTetris[1]->setPosition(ccp(GAME_WIDTH  - BLOCKW * 3, GAME_HEIGHT - BLOCKW * 7));
		nextTetris[2]->setPosition(ccp(GAME_WIDTH  - BLOCKW * 2, GAME_HEIGHT - BLOCKW * 7));
		nextTetris[3]->setPosition(ccp(GAME_WIDTH  - BLOCKW * 3, GAME_HEIGHT - BLOCKW * 8));
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
		curTetris[i]->setPosition(Vec2(pos.x - 7 * BLOCKW - 10, pos.y + 9 * BLOCKW));
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
			log("%d, %d added.", column, row);
		}

		// check game over
		if (!gameOverCheck()) {
			nextToCur();
		}
		else {
		}
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
			
			if (row < ROW && allBlocks[column][row] != nullptr) {
				return false;
			}
			if (pos.y - BLOCKW <= 0) {
				return false;
			}
			break;
		case DIRECTION_LEFT:
			row = (int)(pos.y / BLOCKW);
			column = (int)((pos.x - BLOCKW) / BLOCKW);

			if (column >= 0 && allBlocks[column][row] != nullptr) {
				return false;
			}
			if (pos.x - BLOCKW < 0) return false;
			break;
		case DIRECTION_RIGHT:
			row = (int)(pos.y / BLOCKW);
			column = (int)((pos.x + BLOCKW) / BLOCKW);

			if (column < COLUMN && allBlocks[column][row] != nullptr) {
				return false;
			}
			if (pos.x + BLOCKW >= (10 * BLOCKW)) return false;
			break;
		default:
			break;
		}
	}
	return true;
}

/* check if the game is over */
boolean MainGame::gameOverCheck() {
	for (int i = 0; i < TNUM; i++) {
		Vec2 pos = curTetris[i]->getPosition();
		if (pos.y > GAME_HEIGHT) {
			return true;
		}
	}
	return false;
}

void MainGame::clickUp(Ref* ref) {
}

void MainGame::clickDown(Ref* ref) {
	if (touchCheck(DIRECTION_DOWN)) {
		for (int i = 0; i < TNUM; i++) {
			Vec2 pos = curTetris[i]->getPosition();
			curTetris[i]->setPosition(Vec2(pos.x, pos.y - 1 * BLOCKW));
		}
	}
}

void MainGame::clickLeft(Ref* ref) {
	if (touchCheck(DIRECTION_LEFT)) {
		for (int i = 0; i < TNUM; i++) {
			Vec2 pos = curTetris[i]->getPosition();
			curTetris[i]->setPosition(Vec2(pos.x - 1 * BLOCKW, pos.y));
		}
	}
}

void MainGame::clickRight(Ref* ref) {
	if (touchCheck(DIRECTION_RIGHT)) {
		for (int i = 0; i < TNUM; i++) {
			Vec2 pos = curTetris[i]->getPosition();
			curTetris[i]->setPosition(Vec2(pos.x + 1 * BLOCKW, pos.y));
		}
	}
}

void MainGame::menuCloseCallback(Ref* pSender)
{
    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
