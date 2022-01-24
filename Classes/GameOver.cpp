
#include "PlayScene.h"
#include "GameOver.h"
#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

Scene* GameOver::createScene()
{
    auto scene = Scene::create();

    auto layer = GameOver::create();

    scene->addChild(layer);
    return scene;
}

bool GameOver::init()
{
    //////////////////////////////
    // 1. super init first
    if (!Scene::init())
    {
        return false;
    }    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    auto background = Sprite::create("background.png");
    background->setPosition(visibleSize.width / 2, visibleSize.height / 2);
    this->addChild(background);

    auto playItem = MenuItemImage::create("Retry Button.png", "Retry Button Clicked.png", CC_CALLBACK_1(GameOver::goToStart, this));
    playItem->setPosition(Point(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));

    auto menu = Menu::create(playItem, NULL);
    menu->setPosition(Point::ZERO);
    this->addChild(menu);




    return true;
    
}


void GameOver::menuCloseCallback(Ref* pSender)
{
    Director::getInstance()->end();
}

void GameOver::goToStart(cocos2d::Ref* sender)
{
    auto start = HelloWorld::createScene();
    Director::getInstance()->replaceScene(start);
}


