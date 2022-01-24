#ifndef __PLAY_SCENE_H__
#define __PLAY_SCENE_H__

#include "cocos2d.h"

class PlayScene : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();

    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);

    // bắt sự kiện chạm vào màn hình
    bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);

    // chạy đường ống
    void spawnPipe(float dt);

    // set giá trị cho isFalling
    void stopFly(float dt);

    // xet va cham va tinh diem
    bool onContactBegin(cocos2d::PhysicsContact& contact);

    // cập nhật vị trí cho chim
    void update(float dt);

    void goToStart(cocos2d::Ref* sender);

    void gameOver();

    int getScore();
    // implement the "static create()" method manually
    CREATE_FUNC(PlayScene);
private:
    int score = 0;
    cocos2d::Label* scoreLaber;
    cocos2d::PhysicsWorld* world;
    cocos2d::Sprite* bird;
    bool isFalling = true;
    void setPhysicsWorld(cocos2d::PhysicsWorld* _world);
};

#endif // __PLAY_SCENE_H__
