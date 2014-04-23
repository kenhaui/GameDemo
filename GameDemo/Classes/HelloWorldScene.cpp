#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"

using namespace cocos2d;
using namespace CocosDenshion;

CCScene* HelloWorld::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
   HelloWorld *layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //Thay đổi background thành màu trắng
    if ( !CCLayerColor::initWithColor(ccc4(255,255,255,255) ) )
    {
        return false;
    }
    
    
    _targets = new CCArray();
    _projectile = new CCArray();
    
  

    //Ham tao
    
    HelloWorld::HelloWorld();{
        _targets =NULL ;
        _projectile = NULL;
    }
    
    
    //Hàm huy
    
    HelloWorld::~HelloWorld();
    {
        if(_targets)
        {
            _targets->release();
            _targets = NULL;
        }
        if(_projectile)
        {
            _projectile->release();
            _projectile = NULL;
        }
    }


    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    CCMenuItemImage *pCloseItem = CCMenuItemImage::create(
                                        "CloseNormal.png",
                                        "CloseSelected.png",
                                        this,
                                        menu_selector(HelloWorld::menuCloseCallback) );
    pCloseItem->setPosition( ccp(CCDirector::sharedDirector()->getWinSize().width - 20, 20) );
    
   

    // create menu, it's an autorelease object
    CCMenu* pMenu = CCMenu::create(pCloseItem, NULL);
    pMenu->setPosition( CCPointZero );
    this->addChild(pMenu);


    //Thêm Sprite Player
    if ( CCLayer::init() )
    {
        CCSize winSize = CCDirector::sharedDirector()->getWinSize();
        CCSprite *player = CCSprite::create("Player.png",
                                            CCRectMake(0, 0, 27, 40));
        player->setPosition( ccp(player->getContentSize().width/2,
                                 winSize.height/2) );
        this->addChild(player);
        //Cho phép thao tác cảm ứng
        this->setTouchEnabled(true);
        
        
    }
    
    
    // gọi pt add quái vật vào màn hình theo khoảng thời gian thực hiện
    this->schedule( schedule_selector(HelloWorld::gameLogic), 2 );
     this->schedule(schedule_selector(HelloWorld::update));
        return true;
}
//Hàm thêm quái vật

void HelloWorld::addTarget(){
    CCSprite *target = CCSprite::create("Target.png",
                                        CCRectMake(0,0,27,40) );
   
    //Thiết lâp tag của mảng quái vật
    target->setTag(1);
    _targets->addObject(target);
    // Xác định sự xuất hiện của quái vật dọc theo trục Y một cách ngẫu nhiên
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    int minY = target->getContentSize().height/2;
    int maxY = winSize.height
    -  target->getContentSize().height/2;
    int rangeY = maxY - minY;
    // srand( TimGetTicks() );
    //
    int actualY = ( rand() % rangeY ) + minY;
    
    // Tạo ra các quái vật nấp sau màn hình, dọc theo canh phải, và có vị trí ngẫu nhiên
    target->setPosition(
                        ccp(winSize.width + (target->getContentSize().width/2),
                            actualY) );
    
   
    this->addChild(target);
    
    // Xác định tốc độ của quái vật
    int minDuration = (int)2.0;
    int maxDuration = (int)4.0;
    int rangeDuration = maxDuration - minDuration;
    // srand( TimGetTicks() );
    int actualDuration = ( rand() % rangeDuration )
    + minDuration;
    
    // Tạo chuyển động cho quái vât
    CCMoveTo* actionMove =
    CCMoveTo::create( (float)actualDuration,
                     ccp(0 - target->getContentSize().width/2, actualY) );
    CCFiniteTimeAction* actionMoveDone =
    CCCallFuncN::create( target,
                        callfuncN_selector(HelloWorld::spriteMoveFinished));
    target->runAction( CCSequence::create(actionMove,
                                          actionMoveDone, NULL) );
}



void HelloWorld::gameLogic(float dt){
    this->addTarget();
}

//Hàm phát hiện va chạm và xoá đạn va quái vật va chạm nhau


//Hàm Touch

void HelloWorld::ccTouchesEnded(CCSet* touches, CCEvent* event){
    //
    CCTouch* touch = (CCTouch*)(touches->anyObject());
    CCPoint location = touch->getLocationInView();
    location = CCDirector::sharedDirector()->convertToGL(location);
    
    //Thiết lập tag của mảng đạn bắn ra
    
    
    //Thiết lập vị trí bạn đầu của đạn
    
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    //thêm sprite đạn vào
    CCSprite* projectile = CCSprite::create("Projectile.png",
                                             CCRectMake(0, 0, 20, 20));
    
    
    projectile->setTag(2);
    _projectile->addObject(projectile);
    
    //set vị trí cho đạn
    projectile ->setPosition(ccp(20, winSize.height/2));
    
    //Xác định vị trí khoảng trống của đạn từ điểm chamj màn hình đén vị trí ban đầu
    int offX = location.x - projectile->getPosition().x;
    int offY = location.y - projectile->getPosition().y;
    
    //Biến mất nếu bắn trúng mục tiêu hoặc bay ngược lại
    if(offX<=0) return;
    
    
    
    this->addChild(projectile);
    //Xác định vị trí muốn đạn bắn đến
    int realX = winSize.width
    + (projectile->getContentSize().width/2);
    float ratio = (float)offY / (float)offX;
    int realY = (realX * ratio) + projectile->getPosition().y;
    CCPoint realDest = ccp(realX, realY);
    
    //Xác đinh quãng đường đạn bay
    
    int offRealX = realX - projectile->getPosition().x;
    int offRealY = realY - projectile->getPosition().y;
    float length = sqrtf((offRealX * offRealX)
                         + (offRealY*offRealY));
    //vận tốc
    float velocity = 480/1; // 480pixels/1sec
    //thời giạn = quãng đường /vận tốc
    float realMoveDuration = length/velocity;
    
    //Di chuyển đến điểm cuối
    
    projectile-> runAction(CCSequence::create(CCMoveTo::create(realMoveDuration, realDest),CCCallFuncN::create(this, callfuncN_selector(HelloWorld::spriteMoveFinished)),NULL));

    
}

//Hàm xoá những quái vật đã di chuyển hết màn hình
void HelloWorld::spriteMoveFinished(CCNode* sender){
    CCSprite *sprite = (CCSprite *)sender;
    this->removeChild(sprite, true);
    if(sprite->getTag() == 1)//Quái vât
    {
        _targets->removeObject(sprite);
    }
    if(sprite->getTag() == 2)//Đạn
    {
        _projectile->removeObject(sprite);
    }
}

void HelloWorld::update(float dt){
    CCArray *projecttilesTodelete = new CCArray;
    CCObject* it = NULL;
    CCObject* jt =NULL;
    CCARRAY_FOREACH(_projectile, it){
        CCSprite *projectile = dynamic_cast<CCSprite*>(it);
        CCRect projectileRect = CCRectMake(
                                           projectile->getPosition().x - (projectile->getContentSize().width/2),
                                           projectile->getPosition().y - (projectile->getContentSize().height/2),
                                           projectile->getContentSize().width,
                                           projectile->getContentSize().height);
        CCArray * targetsToDelete = new CCArray;
        
        CCARRAY_FOREACH(_targets, jt){
            CCSprite * target = dynamic_cast<CCSprite*>(jt);
            CCRect targetRect = CCRectMake(
                                           target->getPosition().x - (target->getContentSize().width/2),
                                           target->getPosition().y - (target->getContentSize().height/2),
                                           target->getContentSize().width,
                                           target->getContentSize().height);
            if (projectileRect.intersectsRect(targetRect)) {
                targetsToDelete->addObject(target);
            }
        }
        CCARRAY_FOREACH(targetsToDelete, jt){
           CCSprite * target = dynamic_cast<CCSprite*>(jt);
            _targets->removeObject(target);
            this->removeChild(target,true);
        }
        if (targetsToDelete->count() >0) {
            projecttilesTodelete->addObject(projectile);
        }
        targetsToDelete->release();
    }
    
    CCARRAY_FOREACH(projecttilesTodelete, it){
        CCSprite *projectile = dynamic_cast<CCSprite*>(it);
        _projectile->removeObject(projectile);
        this->removeChild(projectile,true);
    }
    projecttilesTodelete->release();
}
void HelloWorld::menuCloseCallback(CCObject* pSender)
{
    CCDirector::sharedDirector()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
