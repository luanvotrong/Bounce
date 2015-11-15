#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "GameDefine.h"

USING_NS_CC;

class HelloWorld : public cocos2d::Layer
{
private:
	bool _isPlaying;
	bool _isGameOver;
	bool _isGravity;

	int _nScore;

	float _fDistanceNeedRandom;
	float _fLastHeightBall;

	//Ball* _pBall;
	Sprite* _pBall;

	CCMotionStreak* _streak;
	Label* _pLabelTTFScore;
	Label* _pLabelBMFScore;

	Node* _pNodeContaintHorizital;

	std::vector< Sprite* > _vtObstacle;

public:
    virtual bool init();  
    static cocos2d::Scene* createScene();
    
	void createObstacle();
	void gameOver();
	void setPause();
	void setResume();
	Action* moveAction( float fDeltaMove, float fDelayTime );
	bool getPlaying();

	virtual bool onTouchBegan(Touch *pTouch, CCEvent *pEvent);
	virtual void onTouchMoved(Touch* pTouch, CCEvent* pEvent);
	virtual void onTouchEnded(Touch* pTouch, CCEvent* pEvent);
	virtual void onTouchCancelled(Touch* pTouch, CCEvent* pEvent);	
	virtual void update(float dt);

    CREATE_FUNC(HelloWorld);
};

#endif // __HELLOWORLD_SCENE_H__
