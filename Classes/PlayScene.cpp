#include "HelloWorldScene.h"
#include "PlayScene.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

Scene* PlayScene::createScene()
{
    // thiet lap Physics
    auto scene = Scene::createWithPhysics();
    scene->getPhysicsWorld()->setDebugDrawMask(cocos2d::PhysicsWorld::DEBUGDRAW_ALL);
    // cài đặt không trọng lực
    scene->getPhysicsWorld()->setGravity(Vec2(0, 0));

    auto layer = PlayScene::create();
    layer->setPhysicsWorld(scene->getPhysicsWorld());

    scene->addChild(layer);

    return scene;
}

// Print useful error message instead of segfaulting when files are not there.
// in thông báo lỗi khi file không tìm thấy
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
// bạn cần init để khởi tạo các yêu cầu của bạn
bool PlayScene::init()
{
    //////////////////////////////
    // 1. super init first
    if (!Scene::init())
    {
        return false;
    }

    // lấy size của màn hình 
    // lấy kích thước màn hình. trả về height và width
    // kiểu dữ liệu trả về cocos2d::size
    auto visibleSize = Director::getInstance()->getVisibleSize();

    // lấy gốc tọa độ của màn hình
    // ở một số thiết bị điện thoại, gốc tọa độ không phải 0 0
    // nên chúng ta cần lấy gốc tọa độ để tính chính xác vị trí
    // kiểu dữ liệu trả về cocos2d::Vec2 x = 0; y = 0
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // tạo một box có kích thước trùng với khung hình và độ rộng bằng 5
    auto edgeBody = PhysicsBody::createEdgeBox(visibleSize, PHYSICSBODY_MATERIAL_DEFAULT, 5);
    edgeBody->setCollisionBitmask(2);
    edgeBody->setContactTestBitmask(true);
    auto edgeNode = Node::create();
    edgeNode->setPosition(visibleSize.width / 2, visibleSize.height / 2);
    edgeNode->setPhysicsBody(edgeBody);
    this->addChild(edgeNode);

    auto background = Sprite::create("background.png");
    background->setPosition(origin + visibleSize / 2);
    this->addChild(background);

    // khởi tạo con chim. 
    // chim to quá nên scale nhỏ lại
    bird = Sprite::create("bird.png");
    bird->setScale(0.1);
    bird->setPosition(visibleSize.width / 2, visibleSize.height / 2);

    // tạo box vật lý cho con chim -> xử lý di chuyển và va chạm
    auto birdBody = PhysicsBody::createEdgeBox((bird->getContentSize()));
    birdBody->setName("Bird");
    birdBody->setCollisionBitmask(4);
    birdBody->setContactTestBitmask(true);
    bird->setPhysicsBody(birdBody);
    this->addChild(bird);

    // cài đặt lable để hiển thị điểm tra màn hình
    __String* tempscore = __String::createWithFormat("%i", score);
    scoreLaber = Label::createWithTTF(tempscore->getCString(), "fonts/Marker Felt.ttf", visibleSize.height * 0.1);
    scoreLaber->setColor(Color3B::GREEN);
    scoreLaber->setPosition(visibleSize.width / 2, visibleSize.height * 0.9);
    this->addChild(scoreLaber);

    // caì đặt sự kiện va chạm
    auto contactListener = EventListenerPhysicsContact::create();
    contactListener->onContactBegin = CC_CALLBACK_1(PlayScene::onContactBegin, this);
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(contactListener, this);

    // cài đặt sự kiện chạm tay vào màn hình
    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->onTouchBegan = CC_CALLBACK_2(PlayScene::onTouchBegan, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);

    // lên lịch gọi hàm chạy đường ống theo 0.05 * kích thước màn hình
    // sau 0.05*width thì sẽ gọi vào hàm spawPipe
    this->schedule(schedule_selector(PlayScene::spawnPipe, ), 0.005 * visibleSize.width);

    // tự cập nhật cho hàm update;
    this->scheduleUpdate();

    return true;
}


void PlayScene::menuCloseCallback(Ref* pSender)
{
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();

    /*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() as given above,instead trigger a custom event created in RootViewController.mm as below*/

    //EventCustom customEndEvent("game_scene_close_event");
    //_eventDispatcher->dispatchEvent(&customEndEvent);


}

bool PlayScene::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event)
{
    isFalling = false;
    // sau 0.25s thì gọi đến hàm stopFly
    this->schedule(schedule_selector(PlayScene::stopFly, 0.25));
    return false;
}

void PlayScene::spawnPipe(float dt)
{
    // lấy kích thước của màn hình hiển thị. trả về height và width
    Size visibleSize = Director::getInstance()->getVisibleSize();
    // lấy tọa độ gốc của màn hình hiển thị, trả về x, y
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // khoi tao hai Sprite
    auto topPipe = Sprite::create("topPipe.png");
    auto bottomPipe = Sprite::create("butonPipe.png");

    // tao box vật lí có kích thước bằng với kích thước của ống nước
    // mục đích để xác định va chạm. đường bo đỏ bao quanh ông nước
    auto topPipeBody = PhysicsBody::createBox(topPipe->getContentSize());
    topPipeBody->setCollisionBitmask(2);                // số 2 là đặt tên như thằng 1 với thằng 2
    topPipeBody->setContactTestBitmask(true);           // cho phép tương tác, nếu không có thì sẽ ko có va chạm
    topPipeBody->setDynamic(false);                     // không bị dịch chuyển do trọng lực và va chạm
    topPipe->setPhysicsBody(topPipeBody);


    // tạo box vật lý cho ống nước phía dưới
    auto bottomPipeBody = PhysicsBody::createBox(bottomPipe->getContentSize());
    bottomPipeBody->setCollisionBitmask(2);
    bottomPipeBody->setContactTestBitmask(true);
    bottomPipeBody->setDynamic(false);
    bottomPipe->setPhysicsBody(bottomPipeBody);

    // random số giữa 0 với 1;
    auto random = CCRANDOM_0_1();
    if (random < 0.35)
    {
        random = 0.35;
    }
    else if (random > 0.65)
    {
        random = 0.65;
    }
    // vị trí của ống nước trên được random
    auto topPipePositon = (random * visibleSize.height) + (topPipe->getContentSize().height / 2);

    // cài đặt vị trí ban đầu
    topPipe->setPosition(Point(visibleSize.width + topPipe->getContentSize().width + origin.x, topPipePositon));
    bottomPipe->setPosition(Point(topPipe->getPositionX()
        , topPipePositon - (bird->getContentSize().height * 0.5) - topPipe->getContentSize().height));

    this->addChild(topPipe);
    this->addChild(bottomPipe);

    // cài đặt di chuyển cho các đường ống
    // sau 0.01*width 
    auto topMove = MoveBy::create(0.01 * visibleSize.width, Vec2(-visibleSize.width * 1.5, 0));
    auto bottomMove = MoveBy::create(0.01 * visibleSize.width, Vec2(-visibleSize.width * 1.5, 0));
    bottomPipe->runAction(bottomMove);
    topPipe->runAction(topMove);

    // cài đặt node để tính điểm
    auto pointNode = Node::create();
    // tại sao Size lại là 1 ???
    // tạo một PhysicsBox có chiều rộng 1 và chiều cao như dưới
    auto pointBody = PhysicsBody::createBox(Size(1, (bird->getContentSize().height * 0.5)));
    // cài đặt thuộc tính để xét va chạm
    pointBody->setCollisionBitmask(3);  // gán tên của vật là 3
    pointBody->setContactTestBitmask(true);
    pointBody->setDynamic(false);
    // sau khi tạo thuộc tính vật lý xong thì gán nó vào pointNode;
    pointNode->setPhysicsBody(pointBody);
    // cài đặt vị trí ban đầu
    pointNode->setPosition(Point(topPipe->getPositionX(), topPipe->getPositionY()
        - (topPipe->getContentSize().height / 2) - (bird->getContentSize().height * 0.5) / 2));
    pointNode->setName("solid");
    // cài đặt cho pointNode di chuyển theo đường ống
    auto pointNodeAction = MoveBy::create(0.01 * visibleSize.width, Vec2(-visibleSize.width * 1.5, 0));
    pointNode->runAction(pointNodeAction);
    this->addChild(pointNode);


}

void PlayScene::stopFly(float dt)
{
    isFalling = true;
}

bool PlayScene::onContactBegin(cocos2d::PhysicsContact& contact)
{
    PhysicsBody* a = contact.getShapeA()->getBody();
    PhysicsBody* b = contact.getShapeB()->getBody();
    if ((a->getCollisionBitmask() == 3 && b->getCollisionBitmask() == 4) ||
        (a->getCollisionBitmask() == 4 && b->getCollisionBitmask() == 3))
    {
        if (a->getCollisionBitmask() == 3 ) {
            if (a->getNode()) {
                ++score;
                a->getNode()->removeFromParent();
            }
      
        }       
        if (b->getCollisionBitmask() == 3) {
            if (b->getNode()) {
                ++score;
                b->getNode()->removeFromParent();
            }
          
        }
     
    }
    if ((a->getCollisionBitmask() == 4 && b->getCollisionBitmask() == 2) ||
        (a->getCollisionBitmask() == 2 && b->getCollisionBitmask() == 4))
    {
        //scoreLaber->setString("you die");
        //this->goToGameOverScreen(1);
        //goToGameOverScreen(1);
        gameOver();
        return false;
    }
    __String* tempscore = __String::createWithFormat("%i", score);
    scoreLaber->setString(tempscore->getCString());
    return true;
}

void PlayScene::update(float dt)
{
    Size visibleSize = Director::getInstance()->getVisibleSize();
    if (isFalling)
    {
        // bay xuống 10 pixel 
        auto birdMove = MoveBy::create(1, Vec2(0, -2));
        // chúi xuống 0 độ trong 1 giây
        auto birdRote = RotateTo::create(0.2, 20);
        // kết hợp 2 hành động cùng một lúc
        auto actionSque = Sequence::create(birdMove, birdRote, nullptr);
        bird->runAction(actionSque);

    }
    else
    {
        // bay lên 10 pixel 
        auto birdMove = MoveBy::create(0.1, Vec2(0, 20));
        // chúi lên 60 độ trong 1 giây
        auto birdRote = RotateTo::create(0.1, -20);
        // kết hợp 2 hành động cùng một lúc
        auto actionSque = Sequence::create(birdMove, birdRote, nullptr);
        bird->runAction(actionSque);

        //bird->setPositionX(visibleSize.width / 2);
        //bird->setPositionY(bird->getPositionY() + visibleSize.height * 0.05);
    }
}

void PlayScene::gameOver()
{
    this->removeAllChildren();
    this->stopAllActions();
    this->unscheduleAllSelectors();

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    auto background = Sprite::create("background.png");
    background->setPosition(visibleSize.width / 2, visibleSize.height / 2);
    this->addChild(background);

    auto playItem = MenuItemImage::create("Retry Button.png", "Retry Button Clicked.png", CC_CALLBACK_1(PlayScene::goToStart, this));
    playItem->setPosition(Point(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y - 10));

    auto menu = Menu::create(playItem, NULL);
    menu->setPosition(Point::ZERO);
    this->addChild(menu);

    __String* tempscore = __String::createWithFormat("%i", score);
    scoreLaber = Label::createWithTTF(tempscore->getCString(), "fonts/Marker Felt.ttf", visibleSize.height * 0.2);
    scoreLaber->setColor(Color3B::GREEN);
    scoreLaber->setPosition(visibleSize.width / 2, visibleSize.height * 0.8);
    this->addChild(scoreLaber);

}

int PlayScene::getScore()
{
    return score;
}

void PlayScene::setPhysicsWorld(cocos2d::PhysicsWorld* _world)
{
    world = _world;
}

void PlayScene::goToStart(cocos2d::Ref* sender)
{
    auto start = HelloWorld::createScene();
    Director::getInstance()->replaceScene(start);
}

