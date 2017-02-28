#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
static const int COLUMN = 10;
static const int ROW = 20;

class MainGame : public cocos2d::Layer
{
private:	
	int allBlocks[ROW][COLUMN];
	float blockWidth, blockHeight;

public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    // implement the "static create()" method manually
    CREATE_FUNC(MainGame);

	void initUI();
	void startGame();
};

#endif // __HELLOWORLD_SCENE_H__
