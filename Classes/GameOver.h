#ifndef __GAME_OVER_SCENE_H__
#define __GAME_OVER_SCENE_H__

#include "PlayScene.h"
#include "HelloWorldScene.h"
#include "cocos2d.h"

class GameOver : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();

    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);

    // bắt sự kiện chạm vào màn hình
    bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event);

    void goToStart(cocos2d::Ref* sender);

    CREATE_FUNC(GameOver);

private:
    int score;
    cocos2d::Label* scoreLaber;



};

#endif // __GAME_OVER_SCENE_H__
