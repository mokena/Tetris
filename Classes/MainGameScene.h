#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
USING_NS_CC;

static const int GAME_WIDTH = 640;
static const int GAME_HEIGHT = 960;
static const int COLUMN = 10;
static const int ROW = 20;
static const int TNUM = 4;
static const int TSTYLE = 7;
static const float BLOCKW = 45.0f;
//static const int DIRECTION_UP = 0;
static const int DIRECTION_DOWN = 1;
static const int DIRECTION_LEFT = 2;
static const int DIRECTION_RIGHT = 3;

// tetrominos
static const int TETROMINO_I = 0;
static const int TETROMINO_O = 1;
static const int TETROMINO_T = 2;
static const int TETROMINO_S = 3;
static const int TETROMINO_Z = 4; 
static const int TETROMINO_J = 5;
static const int TETROMINO_L = 6;

// tetromino directions 
static const int TETROMINO_DIRECTION_UP = 0;
static const int TETROMINO_DIRECTION_DOWN = 1;
static const int TETROMINO_DIRECTION_LEFT = 2;
static const int TETROMINO_DIRECTION_RIGHT = 3;

class MainGame : public cocos2d::Layer
{
private:	
	Sprite* allBlocks[COLUMN][ROW];
	Sprite* curTetris[TNUM];
	Sprite* nextTetris[TNUM];
	float blockWidth, blockHeight;
	float rightAreaWidth;
	ui::Button *upBtn;
	ui::Button* downBtn;
	ui::Button* leftBtn;
	ui::Button* rightBtn;
	int tetrominoType = 0;
	int tetrominoDirection = 0;

public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    // implement the "static create()" method manually
    CREATE_FUNC(MainGame);

	void initUI();
	void startGame();

	// generate the next blocks randomly
	void randomTetris();

	// make the next blocks to be the current ones
	void nextToCur();

	// make the current blocks falls when game started
	void moveUpdate(float dt);

	// touch check
	boolean touchCheck(int direction);

	// check weither game over
	boolean gameOverCheck();

	void clickUp(Ref* ref);
	void clickDown(Ref* ref);
	void clickLeft(Ref* ref);
	void clickRight(Ref* ref);

	void turnTetrominoI();
	void turnTetrominoO();
	void turnTetrominoT();
	void turnTetrominoS();
	void turnTetrominoZ();
	void turnTetrominoJ();
	void turnTetrominoL();
};

#endif // __HELLOWORLD_SCENE_H__
