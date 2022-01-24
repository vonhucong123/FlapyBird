#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include "PlayScene.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"


USING_NS_CC;

Scene* HelloWorld::createScene()
{
    auto scene = Scene::create();
    auto layer = HelloWorld::create();
    scene->addChild(layer);

    return scene;
}

bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if (!Scene::init())
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();

    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    {
        //auto background = Sprite::create("background.png");
        //background->setPosition(visibleSize.width / 2, visibleSize.height / 2);
        //this->addChild(background);

        // tạo menu play, khi chưa ấn sẽ hiển thị ảnh 1, khi ấn vào sẽ hiển thị ảnh 2
        // sau đó gọi đến hàm
        //auto playItem = MenuItemImage::create("Play Button.png", "Play Button Clicked.png", CC_CALLBACK_1(HelloWorld::goToPlayScene, this));
        //playItem->setPosition(Point(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));

        //auto menu = Menu::create(playItem, NULL);
        //menu->setPosition(Point::ZERO);
        //this->addChild(menu);
    }

    // lấy UI từ cocosstudio
    auto mainMenu = CSLoader::getInstance()->createNode("csb/MainMenuVer1.csb");
    this->addChild(mainMenu);

    auto button = mainMenu->getChildByName<ui::Button*>("ButtonPlay");
    button->setPressedActionEnabled(true);
    button->addClickEventListener([=](Ref*)
    {
            goToPlayScene(this);
    });

    return true;
}


void HelloWorld::menuCloseCallback(Ref* pSender)
{
    Director::getInstance()->end();
}

void HelloWorld::goToPlayScene(Ref* sender)
{
    auto play = PlayScene::createScene();
    Director::getInstance()->replaceScene(play);

}


