#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "GameDefine.h"

USING_NS_CC;

#define shit 8

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

	//Place to declare variables loaded from Blance.json
	//Read Note.txt to use
	int MOVEUP_BOUNDARY;
	float DURATION_UP;
	float DURATION_DOWN;
	float OBSTACLE_START_Y;
	float OBSTACLE_RAND_MIN;
	float OBSTACLE_RAND_MAX;
	float OBSTACLE_TIME_MIN;
	float OBSTACLE_TIME_MAX;

public:
    virtual bool init();  
    static cocos2d::Scene* createScene();
    
	void createObstacle(float y);
	void createObstacles(bool isInit);
	void gameOver();
	void setPause();
	void setResume();
	void moveCameraAndPauseGame(float fYBall);
	void resumeGame();
	Action* moveAction( float fDeltaMove, float fDelayTime );
	bool getPlaying();
	void LoadDefine(char * jsonName);

	virtual bool onTouchBegan(Touch *pTouch, CCEvent *pEvent);
	virtual void onTouchMoved(Touch* pTouch, CCEvent* pEvent);
	virtual void onTouchEnded(Touch* pTouch, CCEvent* pEvent);
	virtual void onTouchCancelled(Touch* pTouch, CCEvent* pEvent);	
	virtual void update(float dt);

    CREATE_FUNC(HelloWorld);
};

#endif // __HELLOWORLD_SCENE_H__
