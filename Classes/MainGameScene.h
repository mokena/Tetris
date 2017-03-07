#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
static const int COLUMN = 10;
static const int ROW = 20;
static const int TNUM = 4;
static const int TSTYLE = 7;
static const float BLOCKW = 45;

class MainGame : public cocos2d::Layer
{
private:	
	Sprite* allBlocks[ROW][COLUMN];
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
	void randomTetris(bool cur);
};

#endif // __HELLOWORLD_SCENE_H__
