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

    // Param init
	initParam();

	// Layout UI
	initUI();

	// Start game
	startGame();
    
    return true;
}

void MainGame::initParam() {
	level = 0;
	score = 0;

	for (int i = 0; i < COLUMN; i++) {
		for (int j = 0; j < ROW; j++) {
			if (allBlocks[i][j] != NULL) {
				allBlocks[i][j]->removeFromParent();
				allBlocks[i][j] = NULL;
			}
		}
	}
}

/*Initiate the UI and layout*/
void MainGame::initUI() {
	auto visibleSize = Director::getInstance()->getVisibleSize();
	blockWidth = blockHeight = BLOCKW;

	// background
	Sprite* bg = Sprite::create("bg1.png");
	bg->setAnchorPoint(Vec2(0, 0));
	bg->setPosition(Vec2(0, 0));
	addChild(bg);

	// up frame
	Sprite* upFrame = Sprite::create("up.png");
	addChild(upFrame);
	upFrame->setAnchorPoint(Vec2(0, 1));
	upFrame->setPosition(Vec2(0, GAME_HEIGHT));
	upFrame->setZOrder(1);

	// score
	LabelTTF* scoreTitle = LabelTTF::create("Score", "arial", 20);
	addChild(scoreTitle);
	scoreTitle->setPosition(Vec2(GAME_WIDTH - 100, GAME_HEIGHT - 40));

	scoreLbl = LabelTTF::create("0", "arial", 20);
	addChild(scoreLbl);
	scoreLbl->setPosition(Vec2(GAME_WIDTH - 100, GAME_HEIGHT - 75));

	// hight score
	LabelTTF* highScoreTitle = LabelTTF::create("High score", "arial", 20);
	addChild(highScoreTitle);
	highScoreTitle->setPosition(Vec2(GAME_WIDTH - 100, GAME_HEIGHT - 105));

	highScoreLbl = LabelTTF::create("0", "arial", 20);
	addChild(highScoreLbl);
	highScoreLbl->setPosition(Vec2(GAME_WIDTH - 100, GAME_HEIGHT - 135));
	int highS = UserDefault::getInstance()->getIntegerForKey(HIGH_SCORE, 0);
	highScoreLbl->setString(String::createWithFormat("%d", highS)->getCString());

	// level 
	LabelTTF* levelTitle = LabelTTF::create("Level", "arial", 20);
	addChild(levelTitle);
	levelTitle->setPosition(Vec2(GAME_WIDTH - 100, GAME_HEIGHT - 165));

	levelLbl = LabelTTF::create("0", "arial", 20);
	addChild(levelLbl);
	levelLbl->setPosition(Vec2(GAME_WIDTH - 100, GAME_HEIGHT - 195));

	// keys 
	upBtn = ui::Button::create("keyRed.png");
	upBtn->setAnchorPoint(Vec2(0.5f, 0.5f));
	upBtn->setPosition(Vec2(550, GAME_HEIGHT - 760));
	addChild(upBtn);
	upBtn->addClickEventListener(CC_CALLBACK_1(MainGame::clickUp, this));

	downBtn = ui::Button::create("keyRed.png");
	downBtn->setAnchorPoint(Vec2(0.5f, 0.5f));
	downBtn->setPosition(Vec2(550, GAME_HEIGHT - 860));
	addChild(downBtn);
	downBtn->addClickEventListener(CC_CALLBACK_1(MainGame::clickDown, this));

	leftBtn = ui::Button::create("keyBlack.png");
	leftBtn->setAnchorPoint(Vec2(0.5f, 0.5f));
	leftBtn->setPosition(Vec2(500, GAME_HEIGHT - 810));
	addChild(leftBtn);
	leftBtn->addClickEventListener(CC_CALLBACK_1(MainGame::clickLeft, this));

	rightBtn = ui::Button::create("keyBlack.png");
	rightBtn->setAnchorPoint(Vec2(0.5f, 0.5f));
	rightBtn->setPosition(Vec2(600, GAME_HEIGHT - 810));
	addChild(rightBtn);
	rightBtn->addClickEventListener(CC_CALLBACK_1(MainGame::clickRight, this));

	pauseBtn = ui::Button::create("pause.png");
	pauseBtn->setAnchorPoint(Vec2(0.5f, 0.5f));
	pauseBtn->setPosition(Vec2(550, GAME_HEIGHT - 650));
	addChild(pauseBtn);
	pauseBtn->addClickEventListener(CC_CALLBACK_1(MainGame::pause, this));

	// exit and restart 
	auto exitLbl = LabelTTF::create("Exit Game", "arial", 30);
	menuItemExit = MenuItemLabel::create(exitLbl, 
		CC_CALLBACK_1(MainGame::menuExitCallback, this));
	menuItemExit->setPosition(Vec2(visibleSize.width - menuItemExit->getContentSize().width / 2,
		menuItemExit->getContentSize().height / 2 - 50));
	menuItemExit->setVisible(false);

	auto restartLbl = LabelTTF::create("Try Again!", "arial", 30);
	menuItemRestart = MenuItemLabel::create(restartLbl,
		CC_CALLBACK_1(MainGame::menuRestartCallback, this));
	menuItemRestart->setPosition(Vec2(visibleSize.width - menuItemRestart->getContentSize().width / 2,
		menuItemRestart->getContentSize().height / 2));
	menuItemRestart->setVisible(false);

	auto menu = Menu::create(menuItemRestart, menuItemExit, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 1);
}


/* When player come into this scene, start game immediately*/
void MainGame::startGame() {
	srand(time(NULL));
	randomTetris();
	nextToCur();
	schedule(schedule_selector(MainGame::moveUpdate), 0.5f);
}

/* Generate the next tetris units by random */
void MainGame::randomTetris()
{
	nextTetrominoType = CCRANDOM_0_1()*(TSTYLE - 1);
	for (int i = 0; i < TNUM; i++) {
		Sprite* block = Sprite::create("block.png");
		nextTetris[i] = block;
		addChild(block);
		nextTetris[i]->setAnchorPoint(Vec2(0, 0));
	}
	
	nextTetrominoDirection = CCRANDOM_0_1()*TETROMINO_DIRECTION_LEFT;
	switch (nextTetrominoType) {
	case TETROMINO_I: //I
		nextTetrominoI(nextTetrominoDirection);
		break;
	case TETROMINO_O: //O
		nextTetrominoO(nextTetrominoDirection);
		break;
	case TETROMINO_T: //T
		nextTetrominoT(nextTetrominoDirection);
		break;
	case TETROMINO_S: //S
		nextTetrominoS(nextTetrominoDirection);
		break;
	case TETROMINO_Z: //Z
		nextTetrominoZ(nextTetrominoDirection);
		break;
	case TETROMINO_J: //J
		nextTetrominoJ(nextTetrominoDirection);
		break;
	case TETROMINO_L: //L
		nextTetrominoL(nextTetrominoDirection);
		break;
	default:
		break;
	}
}

/* generate next tetromino */
void MainGame::nextTetrominoI(int direction) {
	float width = GAME_WIDTH - GAME_W_GAP;
	float height = GAME_HEIGHT - GAME_H_GAP;
	switch (direction) {
	case TETROMINO_DIRECTION_UP:
		nextTetris[0]->setPosition(ccp(width - BLOCKW * 4, height - BLOCKW * 6));
		nextTetris[1]->setPosition(ccp(width - BLOCKW * 3, height - BLOCKW * 6));
		nextTetris[2]->setPosition(ccp(width - BLOCKW * 2, height - BLOCKW * 6));
		nextTetris[3]->setPosition(ccp(width - BLOCKW * 1, height - BLOCKW * 6));
		break;
	case TETROMINO_DIRECTION_RIGHT:
		nextTetris[0]->setPosition(ccp(width - BLOCKW * 2, height - BLOCKW * 6));
		nextTetris[1]->setPosition(ccp(width - BLOCKW * 2, height - BLOCKW * 7));
		nextTetris[2]->setPosition(ccp(width - BLOCKW * 2, height - BLOCKW * 8));
		nextTetris[3]->setPosition(ccp(width - BLOCKW * 2, height - BLOCKW * 9));
		break; 
	case TETROMINO_DIRECTION_DOWN:
		nextTetris[0]->setPosition(ccp(width - BLOCKW * 1, height - BLOCKW * 8));
		nextTetris[1]->setPosition(ccp(width - BLOCKW * 2, height - BLOCKW * 8));
		nextTetris[2]->setPosition(ccp(width - BLOCKW * 3, height - BLOCKW * 8));
		nextTetris[3]->setPosition(ccp(width - BLOCKW * 4, height - BLOCKW * 8));
		break;
	case TETROMINO_DIRECTION_LEFT:
		nextTetris[0]->setPosition(ccp(width - BLOCKW * 3, height - BLOCKW * 9));
		nextTetris[1]->setPosition(ccp(width - BLOCKW * 3, height - BLOCKW * 8));
		nextTetris[2]->setPosition(ccp(width - BLOCKW * 3, height - BLOCKW * 7));
		nextTetris[3]->setPosition(ccp(width - BLOCKW * 3, height - BLOCKW * 6));
		break;
	default:
		break;
	}
}
void MainGame::nextTetrominoO(int direction) {
	float width = GAME_WIDTH - GAME_W_GAP;
	float height = GAME_HEIGHT - GAME_H_GAP;
	nextTetris[0]->setPosition(ccp(width - BLOCKW * 4, height - BLOCKW * 7));
	nextTetris[1]->setPosition(ccp(width - BLOCKW * 4, height - BLOCKW * 8));
	nextTetris[2]->setPosition(ccp(width - BLOCKW * 3, height - BLOCKW * 7));
	nextTetris[3]->setPosition(ccp(width - BLOCKW * 3, height - BLOCKW * 8));
}
void MainGame::nextTetrominoT(int direction) {
	float width = GAME_WIDTH - GAME_W_GAP;
	float height = GAME_HEIGHT - GAME_H_GAP;
	switch (direction) {
	case TETROMINO_DIRECTION_UP:
		nextTetris[0]->setPosition(ccp(width - BLOCKW * 4, height - BLOCKW * 8));
		nextTetris[1]->setPosition(ccp(width - BLOCKW * 3, height - BLOCKW * 8));
		nextTetris[2]->setPosition(ccp(width - BLOCKW * 2, height - BLOCKW * 8));
		nextTetris[3]->setPosition(ccp(width - BLOCKW * 3, height - BLOCKW * 7));
		break;
	case TETROMINO_DIRECTION_RIGHT:
		nextTetris[0]->setPosition(ccp(width - BLOCKW * 3, height - BLOCKW * 7));
		nextTetris[1]->setPosition(ccp(width - BLOCKW * 3, height - BLOCKW * 8));
		nextTetris[2]->setPosition(ccp(width - BLOCKW * 3, height - BLOCKW * 9));
		nextTetris[3]->setPosition(ccp(width - BLOCKW * 2, height - BLOCKW * 8));
		break;
	case TETROMINO_DIRECTION_DOWN:
		nextTetris[0]->setPosition(ccp(width - BLOCKW * 2, height - BLOCKW * 8));
		nextTetris[1]->setPosition(ccp(width - BLOCKW * 3, height - BLOCKW * 8));
		nextTetris[2]->setPosition(ccp(width - BLOCKW * 4, height - BLOCKW * 8));
		nextTetris[3]->setPosition(ccp(width - BLOCKW * 3, height - BLOCKW * 9));
		break;
	case TETROMINO_DIRECTION_LEFT:
		nextTetris[0]->setPosition(ccp(width - BLOCKW * 3, height - BLOCKW * 9));
		nextTetris[1]->setPosition(ccp(width - BLOCKW * 3, height - BLOCKW * 8));
		nextTetris[2]->setPosition(ccp(width - BLOCKW * 3, height - BLOCKW * 7));
		nextTetris[3]->setPosition(ccp(width - BLOCKW * 4, height - BLOCKW * 8));
		break;
	default:
		break;
	}
}
void MainGame::nextTetrominoS(int direction) {
	float width = GAME_WIDTH - GAME_W_GAP;
	float height = GAME_HEIGHT - GAME_H_GAP;
	switch (direction) {
	case TETROMINO_DIRECTION_UP:
		nextTetris[0]->setPosition(ccp(width - BLOCKW * 2, height - BLOCKW * 7));
		nextTetris[1]->setPosition(ccp(width - BLOCKW * 3, height - BLOCKW * 7));
		nextTetris[2]->setPosition(ccp(width - BLOCKW * 3, height - BLOCKW * 8));
		nextTetris[3]->setPosition(ccp(width - BLOCKW * 4, height - BLOCKW * 8));
		break;
	case TETROMINO_DIRECTION_RIGHT:
		nextTetris[0]->setPosition(ccp(width - BLOCKW * 2, height - BLOCKW * 9));
		nextTetris[1]->setPosition(ccp(width - BLOCKW * 2, height - BLOCKW * 8));
		nextTetris[2]->setPosition(ccp(width - BLOCKW * 3, height - BLOCKW * 8));
		nextTetris[3]->setPosition(ccp(width - BLOCKW * 3, height - BLOCKW * 7));
		break;
	case TETROMINO_DIRECTION_DOWN:
		nextTetris[0]->setPosition(ccp(width - BLOCKW * 4, height - BLOCKW * 9));
		nextTetris[1]->setPosition(ccp(width - BLOCKW * 3, height - BLOCKW * 9));
		nextTetris[2]->setPosition(ccp(width - BLOCKW * 3, height - BLOCKW * 8));
		nextTetris[3]->setPosition(ccp(width - BLOCKW * 2, height - BLOCKW * 8));
		break;
	case TETROMINO_DIRECTION_LEFT:
		nextTetris[0]->setPosition(ccp(width - BLOCKW * 4, height - BLOCKW * 7));
		nextTetris[1]->setPosition(ccp(width - BLOCKW * 4, height - BLOCKW * 8));
		nextTetris[2]->setPosition(ccp(width - BLOCKW * 3, height - BLOCKW * 8));
		nextTetris[3]->setPosition(ccp(width - BLOCKW * 3, height - BLOCKW * 9));
		break;
	default:
		break;
	}
}
void MainGame::nextTetrominoZ(int direction) {
	float width = GAME_WIDTH - GAME_W_GAP;
	float height = GAME_HEIGHT - GAME_H_GAP;
	switch (direction) {
	case TETROMINO_DIRECTION_UP:
		nextTetris[0]->setPosition(ccp(width - BLOCKW * 4, height - BLOCKW * 7));
		nextTetris[1]->setPosition(ccp(width - BLOCKW * 3, height - BLOCKW * 7));
		nextTetris[2]->setPosition(ccp(width - BLOCKW * 3, height - BLOCKW * 8));
		nextTetris[3]->setPosition(ccp(width - BLOCKW * 2, height - BLOCKW * 8));
		break;
	case TETROMINO_DIRECTION_RIGHT:
		nextTetris[0]->setPosition(ccp(width - BLOCKW * 2, height - BLOCKW * 7));
		nextTetris[1]->setPosition(ccp(width - BLOCKW * 2, height - BLOCKW * 8));
		nextTetris[2]->setPosition(ccp(width - BLOCKW * 3, height - BLOCKW * 8));
		nextTetris[3]->setPosition(ccp(width - BLOCKW * 3, height - BLOCKW * 9));
		break;
	case TETROMINO_DIRECTION_DOWN:
		nextTetris[0]->setPosition(ccp(width - BLOCKW * 2, height - BLOCKW * 9));
		nextTetris[1]->setPosition(ccp(width - BLOCKW * 3, height - BLOCKW * 9));
		nextTetris[2]->setPosition(ccp(width - BLOCKW * 3, height - BLOCKW * 8));
		nextTetris[3]->setPosition(ccp(width - BLOCKW * 4, height - BLOCKW * 8));
		break;
	case TETROMINO_DIRECTION_LEFT:
		nextTetris[0]->setPosition(ccp(width - BLOCKW * 4, height - BLOCKW * 9));
		nextTetris[1]->setPosition(ccp(width - BLOCKW * 4, height - BLOCKW * 8));
		nextTetris[2]->setPosition(ccp(width - BLOCKW * 3, height - BLOCKW * 8));
		nextTetris[3]->setPosition(ccp(width - BLOCKW * 3, height - BLOCKW * 7));
		break;
	default:
		break;
	}
}
void MainGame::nextTetrominoJ(int direction) {
	float width = GAME_WIDTH - GAME_W_GAP;
	float height = GAME_HEIGHT - GAME_H_GAP;
	switch (direction) {
	case TETROMINO_DIRECTION_UP:
		nextTetris[0]->setPosition(ccp(width - BLOCKW * 4, height - BLOCKW * 7));
		nextTetris[1]->setPosition(ccp(width - BLOCKW * 4, height - BLOCKW * 8));
		nextTetris[2]->setPosition(ccp(width - BLOCKW * 3, height - BLOCKW * 8));
		nextTetris[3]->setPosition(ccp(width - BLOCKW * 2, height - BLOCKW * 8));
		break;
	case TETROMINO_DIRECTION_RIGHT:
		nextTetris[0]->setPosition(ccp(width - BLOCKW * 2, height - BLOCKW * 7));
		nextTetris[1]->setPosition(ccp(width - BLOCKW * 3, height - BLOCKW * 7));
		nextTetris[2]->setPosition(ccp(width - BLOCKW * 3, height - BLOCKW * 8));
		nextTetris[3]->setPosition(ccp(width - BLOCKW * 3, height - BLOCKW * 9));
		break;
	case TETROMINO_DIRECTION_DOWN:
		nextTetris[0]->setPosition(ccp(width - BLOCKW * 2, height - BLOCKW * 9));
		nextTetris[1]->setPosition(ccp(width - BLOCKW * 2, height - BLOCKW * 8));
		nextTetris[2]->setPosition(ccp(width - BLOCKW * 3, height - BLOCKW * 8));
		nextTetris[3]->setPosition(ccp(width - BLOCKW * 4, height - BLOCKW * 8));
		break;
	case TETROMINO_DIRECTION_LEFT:
		nextTetris[0]->setPosition(ccp(width - BLOCKW * 4, height - BLOCKW * 9));
		nextTetris[1]->setPosition(ccp(width - BLOCKW * 3, height - BLOCKW * 9));
		nextTetris[2]->setPosition(ccp(width - BLOCKW * 3, height - BLOCKW * 8));
		nextTetris[3]->setPosition(ccp(width - BLOCKW * 3, height - BLOCKW * 7));
		break;
	default:
		break;
	}
}
void MainGame::nextTetrominoL(int direction) {
	float width = GAME_WIDTH - GAME_W_GAP;
	float height = GAME_HEIGHT - GAME_H_GAP;
	switch (direction) {
	case TETROMINO_DIRECTION_UP:
		nextTetris[0]->setPosition(ccp(width - BLOCKW * 4, height - BLOCKW * 8));
		nextTetris[1]->setPosition(ccp(width - BLOCKW * 3, height - BLOCKW * 8));
		nextTetris[2]->setPosition(ccp(width - BLOCKW * 2, height - BLOCKW * 8));
		nextTetris[3]->setPosition(ccp(width - BLOCKW * 2, height - BLOCKW * 7));
		break;
	case TETROMINO_DIRECTION_RIGHT:
		nextTetris[0]->setPosition(ccp(width - BLOCKW * 3, height - BLOCKW * 7));
		nextTetris[1]->setPosition(ccp(width - BLOCKW * 3, height - BLOCKW * 8));
		nextTetris[2]->setPosition(ccp(width - BLOCKW * 3, height - BLOCKW * 9));
		nextTetris[3]->setPosition(ccp(width - BLOCKW * 2, height - BLOCKW * 9));
		break;
	case TETROMINO_DIRECTION_DOWN:
		nextTetris[0]->setPosition(ccp(width - BLOCKW * 2, height - BLOCKW * 8));
		nextTetris[1]->setPosition(ccp(width - BLOCKW * 3, height - BLOCKW * 8));
		nextTetris[2]->setPosition(ccp(width - BLOCKW * 4, height - BLOCKW * 8));
		nextTetris[3]->setPosition(ccp(width - BLOCKW * 4, height - BLOCKW * 9));
		break;
	case TETROMINO_DIRECTION_LEFT:
		nextTetris[0]->setPosition(ccp(width - BLOCKW * 3, height - BLOCKW * 9));
		nextTetris[1]->setPosition(ccp(width - BLOCKW * 3, height - BLOCKW * 8));
		nextTetris[2]->setPosition(ccp(width - BLOCKW * 3, height - BLOCKW * 7));
		nextTetris[3]->setPosition(ccp(width - BLOCKW * 4, height - BLOCKW * 7));
		break;
	default:
		break;
	}
}

/* Turn the next tetris to current dropping tetris */
void MainGame::nextToCur()
{
	curTetrominoType = nextTetrominoType;
	curTetrominoDirection = nextTetrominoDirection;
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
	if (touchCheck(MOVE_DIRECTION_DOWN)) {
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
			if (row < ROW) {
				allBlocks[row][column] = curTetris[i];
			}
		}

		// dismiss a line 
		dismissLine();

		// check game over
		nextToCur();
		if (gameOverCheck()) {
			// show game over
			unschedule(schedule_selector(MainGame::moveUpdate));
			menuItemRestart->setVisible(true);
			menuItemExit->setVisible(true);
		}
	}
}

/* when game started, check the current blocks is touching 
   other motionless blocks or the walls, if can move on, return
   true, otherwise return false */
bool MainGame::touchCheck(int direction)
{
	for (int i = 0; i < TNUM; i++) {
		Vec2 pos = curTetris[i]->getPosition();
		int row = 0, column = 0;
		switch (direction) {
		case MOVE_DIRECTION_DOWN:
			row = (int)((pos.y - BLOCKW) / BLOCKW);
			column = (int)(pos.x / BLOCKW);
			
			if ((row < ROW && allBlocks[row][column] != NULL) ||
				(row >= ROW && allBlocks[ROW - 1][column] != NULL)) {
				return false;
			}
			if (pos.y - BLOCKW <= 0) {
				return false;
			}
			break;
		case MOVE_DIRECTION_LEFT:
			row = (int)(pos.y / BLOCKW);
			column = (int)((pos.x - BLOCKW) / BLOCKW);

			if (column >= 0 && allBlocks[row][column] != NULL) {
				return false;
			}
			if (pos.x - BLOCKW < 0) return false;
			break;
		case MOVE_DIRECTION_RIGHT:
			row = (int)(pos.y / BLOCKW);
			column = (int)((pos.x + BLOCKW) / BLOCKW);

			if (column < COLUMN && allBlocks[row][column] != NULL) {
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

/* enum all the blocks, when there is a line full of blocks, dismiss the line */
void MainGame::dismissLine() {
	int dismissedCount = -1; // current dismissed lines count, max is 4
	int dismissedLn[4] = {-1};
	for (int i = 0; i < ROW; i++) {
		int j = 0;
		for (j = 0; j < COLUMN; j++) {
			if (allBlocks[i][j] == NULL) {
				break;
			}
		}
		if (j >= COLUMN) {
			dismissedCount++;
			dismissedLines++; // total dismissed lines count of one game
			dismissedLn[dismissedCount] = i;
			for (j = 0; j < COLUMN; j++) {
				allBlocks[i][j]->removeFromParent();
				allBlocks[i][j] = NULL;
			}
		}
	}
	for (int i = dismissedCount; i >= 0; i--) {
		int row = dismissedLn[i];
		for (row; row < ROW - 1; row++) {
			for (int j = 0; j < COLUMN; j++) {
				if (allBlocks[row][j] == NULL && allBlocks[row + 1][j] != NULL) {
					allBlocks[row][j] = allBlocks[row + 1][j];
					allBlocks[row][j]->setPosition(Vec2(allBlocks[row][j]->getPositionX(),
						allBlocks[row][j]->getPositionY() - BLOCKW));
					allBlocks[row + 1][j] = NULL;
				}
			}
		}
	}
	if (dismissedCount == 0) {
		score += 40 * (level + 1);
	}
	else if (dismissedCount == 1) {
		score += 100 * (level + 1);
	}
	else if (dismissedCount == 2) {
		score += 300 * (level + 1);
	}
	else if (dismissedCount == 3) {
		score += 1200 * (level + 1);
	}
	scoreLbl->setString(String::createWithFormat("%d", score)->getCString());
	upgrade();
}

void MainGame::upgrade()
{
	level = dismissedLines / 10;
	levelLbl->setString(String::createWithFormat("%d", level)->getCString());

	// change speed
	unschedule(schedule_selector(MainGame::moveUpdate));
	schedule(schedule_selector(MainGame::moveUpdate), (0.5f - 0.5f*level/5));
}

void MainGame::pause(Ref* ref) {
	if (paused) {
		paused = false;
		schedule(schedule_selector(MainGame::moveUpdate), (0.5f - 0.5f*level / 5));
	}
	else {
		paused = true;
		unschedule(schedule_selector(MainGame::moveUpdate));
	}
}

/* check if the game is over */
bool MainGame::gameOverCheck() {
	if (touchCheck(MOVE_DIRECTION_DOWN)) {
		return false;
	}
	else {
		int highS = UserDefault::getInstance()->getIntegerForKey(HIGH_SCORE, 0);
		if (score > highS) {
			highScoreLbl->setString(String::createWithFormat("%d", score)->getCString());
			UserDefault::getInstance()->setIntegerForKey(HIGH_SCORE, score);
		}
		return true;
	}
	//for (int i = 0; i < TNUM; i++) {
	//	Vec2 pos = curTetris[i]->getPosition();
	//	if (pos.y > GAME_HEIGHT) {
	//		return true;
	//	}
	//}
	//return false;
}

void MainGame::clickUp(Ref* ref) {
	switch (curTetrominoType) {
	case TETROMINO_I: //I
		turnTetrominoI();
		break;
	case TETROMINO_O: //O
		turnTetrominoO();
		break;
	case TETROMINO_T: //T
		turnTetrominoT();
		break;
	case TETROMINO_S: //S
		turnTetrominoS();
		break;
	case TETROMINO_Z: //Z
		turnTetrominoZ();
		break;
	case TETROMINO_J: //J
		turnTetrominoJ();
		break;
	case TETROMINO_L: //L
		turnTetrominoL();
		break;
	default:
		break;
	}
	curTetrominoDirection++;
	if (curTetrominoDirection > TETROMINO_DIRECTION_LEFT) {
		curTetrominoDirection = TETROMINO_DIRECTION_UP;
	}
}

void MainGame::clickDown(Ref* ref) {
	if (touchCheck(MOVE_DIRECTION_DOWN)) {
		for (int i = 0; i < TNUM; i++) {
			Vec2 pos = curTetris[i]->getPosition();
			curTetris[i]->setPosition(Vec2(pos.x, pos.y - 1 * BLOCKW));
		}
	}
}

void MainGame::clickLeft(Ref* ref) {
	if (touchCheck(MOVE_DIRECTION_LEFT)) {
		for (int i = 0; i < TNUM; i++) {
			Vec2 pos = curTetris[i]->getPosition();
			curTetris[i]->setPosition(Vec2(pos.x - 1 * BLOCKW, pos.y));
		}
	}
}

void MainGame::clickRight(Ref* ref) {
	if (touchCheck(MOVE_DIRECTION_RIGHT)) {
		for (int i = 0; i < TNUM; i++) {
			Vec2 pos = curTetris[i]->getPosition();
			curTetris[i]->setPosition(Vec2(pos.x + 1 * BLOCKW, pos.y));
		}
	}
}

// rotate tetrmino "I"
void MainGame::turnTetrominoI() {
	Vec2 pos = curTetris[0]->getPosition();
	switch (curTetrominoDirection) {
	case TETROMINO_DIRECTION_UP: // go to right
		pos = curTetris[0]->getPosition();
		curTetris[0]->setPosition(ccp(pos.x + BLOCKW * 2, pos.y + BLOCKW));
		pos = curTetris[1]->getPosition();
		curTetris[1]->setPosition(ccp(pos.x + BLOCKW * 1, pos.y));
		pos = curTetris[2]->getPosition();
		curTetris[2]->setPosition(ccp(pos.x, pos.y - BLOCKW));
		pos = curTetris[3]->getPosition();
		curTetris[3]->setPosition(ccp(pos.x - BLOCKW, pos.y - BLOCKW * 2));
		break;
	case TETROMINO_DIRECTION_RIGHT: // go to down
		pos = curTetris[0]->getPosition();
		curTetris[0]->setPosition(ccp(pos.x + BLOCKW, pos.y - BLOCKW * 2));
		pos = curTetris[1]->getPosition();
		curTetris[1]->setPosition(ccp(pos.x, pos.y - BLOCKW));
		pos = curTetris[2]->getPosition();
		curTetris[2]->setPosition(ccp(pos.x - BLOCKW, pos.y));
		pos = curTetris[3]->getPosition();
		curTetris[3]->setPosition(ccp(pos.x - BLOCKW * 2, pos.y + BLOCKW));
		break;
	case TETROMINO_DIRECTION_DOWN: // go to left
		pos = curTetris[0]->getPosition();
		curTetris[0]->setPosition(ccp(pos.x - BLOCKW * 2, pos.y - BLOCKW));
		pos = curTetris[1]->getPosition();
		curTetris[1]->setPosition(ccp(pos.x - BLOCKW, pos.y));
		pos = curTetris[2]->getPosition();
		curTetris[2]->setPosition(ccp(pos.x, pos.y + BLOCKW));
		pos = curTetris[3]->getPosition();
		curTetris[3]->setPosition(ccp(pos.x + BLOCKW, pos.y + BLOCKW * 2));
		break;
	case TETROMINO_DIRECTION_LEFT: // go to up
		pos = curTetris[0]->getPosition();
		curTetris[0]->setPosition(ccp(pos.x - BLOCKW, pos.y + BLOCKW * 2));
		pos = curTetris[1]->getPosition();
		curTetris[1]->setPosition(ccp(pos.x, pos.y + BLOCKW));
		pos = curTetris[2]->getPosition();
		curTetris[2]->setPosition(ccp(pos.x + BLOCKW, pos.y));
		pos = curTetris[3]->getPosition();
		curTetris[3]->setPosition(ccp(pos.x + BLOCKW * 2, pos.y - BLOCKW));
		break;
	default:
		break;
	}
}

// rotate tetrmino "O"
void MainGame::turnTetrominoO() {
	switch (curTetrominoDirection) {
	case TETROMINO_DIRECTION_UP: // go to right
		break;
	case TETROMINO_DIRECTION_DOWN: // go to left 
		break;
	case TETROMINO_DIRECTION_LEFT: // go to up
		break;
	case TETROMINO_DIRECTION_RIGHT: // go to down
		break;
	default:
		break;
	}
}

// rotate tetrmino "T"
void MainGame::turnTetrominoT() {
	Vec2 pos = curTetris[0]->getPosition();
	switch (curTetrominoDirection) {
	case TETROMINO_DIRECTION_UP: // go to right
		pos = curTetris[0]->getPosition();
		curTetris[0]->setPosition(ccp(pos.x + BLOCKW, pos.y + BLOCKW));
		pos = curTetris[2]->getPosition();
		curTetris[2]->setPosition(ccp(pos.x - BLOCKW, pos.y - BLOCKW));
		pos = curTetris[3]->getPosition();
		curTetris[3]->setPosition(ccp(pos.x + BLOCKW, pos.y - BLOCKW));
		break;
	case TETROMINO_DIRECTION_RIGHT: // go to down
		pos = curTetris[0]->getPosition();
		curTetris[0]->setPosition(ccp(pos.x + BLOCKW, pos.y - BLOCKW));
		pos = curTetris[2]->getPosition();
		curTetris[2]->setPosition(ccp(pos.x - BLOCKW, pos.y + BLOCKW));
		pos = curTetris[3]->getPosition();
		curTetris[3]->setPosition(ccp(pos.x - BLOCKW, pos.y - BLOCKW));
		break;
	case TETROMINO_DIRECTION_DOWN: // go to left
		pos = curTetris[0]->getPosition();
		curTetris[0]->setPosition(ccp(pos.x - BLOCKW, pos.y - BLOCKW));
		pos = curTetris[2]->getPosition();
		curTetris[2]->setPosition(ccp(pos.x + BLOCKW, pos.y + BLOCKW));
		pos = curTetris[3]->getPosition();
		curTetris[3]->setPosition(ccp(pos.x - BLOCKW, pos.y + BLOCKW));
		break;
	case TETROMINO_DIRECTION_LEFT: // go to up
		pos = curTetris[0]->getPosition();
		curTetris[0]->setPosition(ccp(pos.x - BLOCKW, pos.y + BLOCKW));
		pos = curTetris[2]->getPosition();
		curTetris[2]->setPosition(ccp(pos.x + BLOCKW, pos.y - BLOCKW));
		pos = curTetris[3]->getPosition();
		curTetris[3]->setPosition(ccp(pos.x + BLOCKW, pos.y + BLOCKW));
		break;
	default:
		break;
	}
}

// rotate tetrmino "S"
void MainGame::turnTetrominoS() {
	Vec2 pos = curTetris[0]->getPosition();
	switch (curTetrominoDirection) {
	case TETROMINO_DIRECTION_UP: // go to right
		pos = curTetris[0]->getPosition();
		curTetris[0]->setPosition(ccp(pos.x, pos.y - BLOCKW * 2));
		pos = curTetris[1]->getPosition();
		curTetris[1]->setPosition(ccp(pos.x + BLOCKW, pos.y - BLOCKW));
		pos = curTetris[3]->getPosition();
		curTetris[3]->setPosition(ccp(pos.x + BLOCKW, pos.y + BLOCKW));
		break;
	case TETROMINO_DIRECTION_RIGHT: // go to down
		pos = curTetris[0]->getPosition();
		curTetris[0]->setPosition(ccp(pos.x - BLOCKW * 2, pos.y));
		pos = curTetris[1]->getPosition();
		curTetris[1]->setPosition(ccp(pos.x - BLOCKW, pos.y - BLOCKW));
		pos = curTetris[3]->getPosition();
		curTetris[3]->setPosition(ccp(pos.x + BLOCKW, pos.y - BLOCKW));
		break;
	case TETROMINO_DIRECTION_DOWN: // go to left
		pos = curTetris[0]->getPosition();
		curTetris[0]->setPosition(ccp(pos.x, pos.y + BLOCKW * 2));
		pos = curTetris[1]->getPosition();
		curTetris[1]->setPosition(ccp(pos.x - BLOCKW, pos.y + BLOCKW));
		pos = curTetris[3]->getPosition();
		curTetris[3]->setPosition(ccp(pos.x - BLOCKW, pos.y - BLOCKW));
		break;
	case TETROMINO_DIRECTION_LEFT: // go to up
		pos = curTetris[0]->getPosition();
		curTetris[0]->setPosition(ccp(pos.x + BLOCKW * 2, pos.y));
		pos = curTetris[1]->getPosition();
		curTetris[1]->setPosition(ccp(pos.x + BLOCKW, pos.y + BLOCKW));
		pos = curTetris[3]->getPosition();
		curTetris[3]->setPosition(ccp(pos.x - BLOCKW, pos.y + BLOCKW));
		break;
	default:
		break;
	}
}
void MainGame::turnTetrominoZ() {
	Vec2 pos = curTetris[0]->getPosition();
	switch (curTetrominoDirection) {
	case TETROMINO_DIRECTION_UP: // go to right
		pos = curTetris[0]->getPosition();
		curTetris[0]->setPosition(ccp(pos.x + BLOCKW * 2, pos.y));
		pos = curTetris[1]->getPosition();
		curTetris[1]->setPosition(ccp(pos.x + BLOCKW, pos.y - BLOCKW));
		pos = curTetris[3]->getPosition();
		curTetris[3]->setPosition(ccp(pos.x - BLOCKW, pos.y - BLOCKW));
		break;
	case TETROMINO_DIRECTION_RIGHT: // go to down
		pos = curTetris[0]->getPosition();
		curTetris[0]->setPosition(ccp(pos.x, pos.y - BLOCKW * 2));
		pos = curTetris[1]->getPosition();
		curTetris[1]->setPosition(ccp(pos.x - BLOCKW, pos.y - BLOCKW));
		pos = curTetris[3]->getPosition();
		curTetris[3]->setPosition(ccp(pos.x - BLOCKW, pos.y + BLOCKW));
		break;
	case TETROMINO_DIRECTION_DOWN: // go to left
		pos = curTetris[0]->getPosition();
		curTetris[0]->setPosition(ccp(pos.x - BLOCKW * 2, pos.y));
		pos = curTetris[1]->getPosition();
		curTetris[1]->setPosition(ccp(pos.x - BLOCKW, pos.y + BLOCKW));
		pos = curTetris[3]->getPosition();
		curTetris[3]->setPosition(ccp(pos.x + BLOCKW, pos.y + BLOCKW));
		break;
	case TETROMINO_DIRECTION_LEFT: // go to up
		pos = curTetris[0]->getPosition();
		curTetris[0]->setPosition(ccp(pos.x, pos.y + BLOCKW * 2));
		pos = curTetris[1]->getPosition();
		curTetris[1]->setPosition(ccp(pos.x + BLOCKW, pos.y + BLOCKW));
		pos = curTetris[3]->getPosition();
		curTetris[3]->setPosition(ccp(pos.x + BLOCKW, pos.y - BLOCKW));
		break;
	default:
		break;
	}
}
void MainGame::turnTetrominoJ() {
	Vec2 pos = curTetris[0]->getPosition();
	switch (curTetrominoDirection) {
	case TETROMINO_DIRECTION_UP: // go to right
		pos = curTetris[0]->getPosition();
		curTetris[0]->setPosition(ccp(pos.x + BLOCKW * 2, pos.y));
		pos = curTetris[1]->getPosition();
		curTetris[1]->setPosition(ccp(pos.x + BLOCKW, pos.y + BLOCKW));
		pos = curTetris[3]->getPosition();
		curTetris[3]->setPosition(ccp(pos.x - BLOCKW, pos.y - BLOCKW));
		break;
	case TETROMINO_DIRECTION_RIGHT: // go to down
		pos = curTetris[0]->getPosition();
		curTetris[0]->setPosition(ccp(pos.x, pos.y - BLOCKW * 2));
		pos = curTetris[1]->getPosition();
		curTetris[1]->setPosition(ccp(pos.x + BLOCKW, pos.y - BLOCKW));
		pos = curTetris[3]->getPosition();
		curTetris[3]->setPosition(ccp(pos.x - BLOCKW, pos.y + BLOCKW));
		break;
	case TETROMINO_DIRECTION_DOWN: // go to left
		pos = curTetris[0]->getPosition();
		curTetris[0]->setPosition(ccp(pos.x - BLOCKW * 2, pos.y));
		pos = curTetris[1]->getPosition();
		curTetris[1]->setPosition(ccp(pos.x - BLOCKW, pos.y - BLOCKW));
		pos = curTetris[3]->getPosition();
		curTetris[3]->setPosition(ccp(pos.x + BLOCKW, pos.y + BLOCKW));
		break;
	case TETROMINO_DIRECTION_LEFT: // go to up
		pos = curTetris[0]->getPosition();
		curTetris[0]->setPosition(ccp(pos.x, pos.y + BLOCKW * 2));
		pos = curTetris[1]->getPosition();
		curTetris[1]->setPosition(ccp(pos.x - BLOCKW, pos.y + BLOCKW));
		pos = curTetris[3]->getPosition();
		curTetris[3]->setPosition(ccp(pos.x + BLOCKW, pos.y - BLOCKW));
		break;
	default:
		break;
	}
}
void MainGame::turnTetrominoL() {
	Vec2 pos = curTetris[0]->getPosition();
	switch (curTetrominoDirection) {
	case TETROMINO_DIRECTION_UP: // go to right
		pos = curTetris[0]->getPosition();
		curTetris[0]->setPosition(ccp(pos.x + BLOCKW, pos.y + BLOCKW));
		pos = curTetris[2]->getPosition();
		curTetris[2]->setPosition(ccp(pos.x - BLOCKW, pos.y - BLOCKW));
		pos = curTetris[3]->getPosition();
		curTetris[3]->setPosition(ccp(pos.x, pos.y - BLOCKW * 2));
		break;
	case TETROMINO_DIRECTION_RIGHT: // go to down
		pos = curTetris[0]->getPosition();
		curTetris[0]->setPosition(ccp(pos.x + BLOCKW, pos.y - BLOCKW));
		pos = curTetris[2]->getPosition();
		curTetris[2]->setPosition(ccp(pos.x - BLOCKW, pos.y + BLOCKW));
		pos = curTetris[3]->getPosition();
		curTetris[3]->setPosition(ccp(pos.x - BLOCKW * 2, pos.y));
		break;
	case TETROMINO_DIRECTION_DOWN: // go to left
		pos = curTetris[0]->getPosition();
		curTetris[0]->setPosition(ccp(pos.x - BLOCKW, pos.y - BLOCKW));
		pos = curTetris[2]->getPosition();
		curTetris[2]->setPosition(ccp(pos.x + BLOCKW, pos.y + BLOCKW));
		pos = curTetris[3]->getPosition();
		curTetris[3]->setPosition(ccp(pos.x, pos.y + BLOCKW * 2));
		break;
	case TETROMINO_DIRECTION_LEFT: // go to up
		pos = curTetris[0]->getPosition();
		curTetris[0]->setPosition(ccp(pos.x - BLOCKW, pos.y + BLOCKW));
		pos = curTetris[2]->getPosition();
		curTetris[2]->setPosition(ccp(pos.x + BLOCKW, pos.y - BLOCKW));
		pos = curTetris[3]->getPosition();
		curTetris[3]->setPosition(ccp(pos.x + BLOCKW * 2, pos.y));
		break;
	default:
		break;
	}
}

void MainGame::menuExitCallback(Ref* pSender)
{
	Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}

void MainGame::menuRestartCallback(Ref* pSender)
{
	// Param init
	initParam();

	// Layout UI
	initUI();

	// Start game
	startGame();
}

void MainGame::menuCloseCallback(Ref* pSender)
{
    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
