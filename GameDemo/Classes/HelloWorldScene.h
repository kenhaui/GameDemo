#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
USING_NS_CC;

class HelloWorld : public cocos2d::CCLayerColor
{
    
    
    HelloWorld();
    
    ~HelloWorld();
    
  
    public:
    // Method 'init' in cocos2d-x returns bool, instead of 'id' in cocos2d-iphone (an object pointer)
    virtual bool init();
    
    //Khai báo thực hiện cham vao man hình
    void ccTouchesEnded(CCSet* touches, CCEvent* event);

    // there's no 'id' in cpp, so we recommend to return the class instance pointer
    static cocos2d::CCScene* scene();
    
    // a selector callback
    void menuCloseCallback(CCObject* pSender);
    
    //Khai báo hàm thêm quái vât
    void addTarget();
    //Khai báo hàm xoá quái vật khi chạy hết man hình
    void spriteMoveFinished(CCNode* sender);
    //Khai báo hàm thêm quái vật vào màn hình
    void gameLogic(float dt);
    //Hàm phát hiện va chạm
   void update(float dt);
   
protected:
    CCArray * _targets;
    CCArray * _projectile;
    // preprocessor macro for "static create()" constructor ( node() deprecated )
    CREATE_FUNC(HelloWorld);
};

#endif // __HELLOWORLD_SCENE_H__
