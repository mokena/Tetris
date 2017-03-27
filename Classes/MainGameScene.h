#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
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

class MainGame : public cocos2d::Layer
{
private:	
	Sprite* allBlocks[COLUMN][ROW];
	Sprite* curTetris[TNUM];
	Sprite* nextTetris[TNUM];
	float blockWidth, blockHeight;
	float rightAreaWidth;

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
};

#endif // __HELLOWORLD_SCENE_H__
