#include "HelloWorldScene.h"

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    Scene *scene = Scene::create();
    
    // 'layer' is an autorelease object
    HelloWorld *layer = HelloWorld::create();
    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
	return scene;
}

Vec2 normalizeEdit( Vec2 pos )
{
	float len = sqrt(pos.x*pos.x + pos.y*pos.y);
	
	return ccp(pos.x / len, pos.y / len);
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
		

	this->setTouchEnabled(true);
	
	_isPlaying	= true;
	_isGameOver = false;
	_isGravity = false;

	_nScore		= 0;

	auto pBg = Sprite::create(IMG_BACKGROUND);
	pBg->setPosition(POS_BACKGROUND);
	this->addChild(pBg, (int)ZOrder::One);


	_pNodeContaintHorizital = Node::create();	
	this->addChild(_pNodeContaintHorizital, (int)ZOrder::Two);

	/*_pBall = Ball::createBall( POS_BALL );
	_pNodeContaintHorizital->addChild(_pBall, (int)ZOrder::Two);*/
	_pBall = nullptr;
	_pBall = Sprite::create(IMG_BALL);
	_pBall->setPosition(POS_BALL);
	_pNodeContaintHorizital->addChild(_pBall, (int)ZOrder::Two);

	//_streak = CCMotionStreak::create(0.4, 1, 15, ccc3(255, 255, 255), "images/streak.png");
	//_streak->setPosition(ccp(_pBall->getPositionX(),
	//							_pBall->getPositionY() - HEIGHT_SPRITE(_pBall)*0.5f) );
	//_streak->setBlendFunc(ADDITIVE);
	//_pBall->setTrail(pStreak);
	//addChild(_streak, (int)ZOrder::Two);

	_pLabelTTFScore = Label::createWithTTF("Score: ", "fonts/arial.ttf", 50);
	_pLabelTTFScore->setPosition(ccp(_pLabelTTFScore->getContentSize().width*0.5f, HEIGHT_SCREEN - _pLabelTTFScore->getContentSize().height*0.5f));
	_pLabelTTFScore->setColor(COLOR_FONT_SCORE);
	this->addChild(_pLabelTTFScore, (int)ZOrder::Highest);

	_pLabelBMFScore = Label::createWithTTF("Score: ", "fonts/arial.ttf", 50);
	_pLabelBMFScore->setPosition(HALF_WIDTH_SCREEN, HEIGHT_SCREEN - 100);
	addChild(_pLabelBMFScore, (int)ZOrder::Highest);

	createObstacle();	

	auto touchlistener = EventListenerTouchOneByOne::create();
	touchlistener->onTouchBegan = CC_CALLBACK_2(HelloWorld::onTouchBegan, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchlistener, this);

	this->scheduleUpdate();

    return true;
}

void HelloWorld::update(float dt)
{
	if (!_isPlaying)
	{
		return;
	}
	
	for (auto pHonizontal : _vtObstacle)
	{
		if (pHonizontal->boundingBox().intersectsRect(_pBall->boundingBox()))
		{
			_isPlaying = false;
			gameOver();
			return;
		}
		if (pHonizontal->getTag() == TAG_FOR_GET_SCORE && _pBall->getPositionY() - _pBall->getContentSize().width*0.5f > pHonizontal->getPositionY())
		{
			_nScore += 1;
			createObstacle();
		}
		
	}
	
	if (_pBall->getPositionY() + _pNodeContaintHorizital->getPositionY() > DISTANCE_NEED_RANDOM)
	{
		_pNodeContaintHorizital->setPositionY(_pNodeContaintHorizital->getPositionY()- DISTANCE_NEED_RANDOM);
		//createObstacle();
	}

	_pLabelBMFScore->setString(std::to_string(_nScore));
	//_streak->setPositionY(_pBall->getPositionY() - HEIGHT_SPRITE(_pBall)*0.5f*SCALE_FACTOR);
}

void HelloWorld::gameOver()
{
	setPause();
	this->unscheduleUpdate();
	auto pSpriteGameOver = Sprite::create("images/gameOver.png");
	pSpriteGameOver->setPosition(ccp(HALF_WIDTH_SCREEN, HALF_HEIGHT_SCREEN));
	addChild(pSpriteGameOver, (int)ZOrder::Highest);
	this->setTouchEnabled(false);
}


Action* HelloWorld::moveAction(float fDeltaMove, float fDelayTime)
{	
	auto pMoveRight = MoveBy::create(fDelayTime, ccp(fDeltaMove, 0));
	auto pSequence = Sequence::create(pMoveRight, pMoveRight->reverse(), NULL);
	
	return RepeatForever::create(pSequence);
}

void HelloWorld::createObstacle()
{
	static bool isCreate = false;
	float fSpriteX = 0.0f;
	float fSpriteY = 0.0f;
	float fDistanceNeedMove = 0.0f;
	float fTimeRandom = 0.5f + CCRANDOM_0_1();

	if ( !isCreate )
	{
		for (int i = 0; i < 4; i++)
		{
			auto pObstacleTemp = Sprite::create("images/horizital.png");			
			fSpriteY = 800 + 800 * (i > 1 ? 0 : 1);
			if (i % 2 == 0)
			{
				if (i == 2)
				{
					fTimeRandom = 0.5f + CCRANDOM_0_1();
				}
				fSpriteX = WIDTH_SCREEN + WIDTH_SPRITE(pObstacleTemp) * 1 / 3 - rand() % 50; // obstacle right				
				
			}
			else
			{
				fSpriteX = -WIDTH_SPRITE(pObstacleTemp) * 1 / 3 + rand() % 50; // obstacle left
			}			
			pObstacleTemp->setPosition(ccp(fSpriteX, fSpriteY));
			_pNodeContaintHorizital->addChild(pObstacleTemp, (int)ZOrder::Two);
			_vtObstacle.push_back(pObstacleTemp);

			if (i % 2 != 0)
			{
				fDistanceNeedMove = HALF_WIDTH_SCREEN - (pObstacleTemp->getPositionX() + WIDTH_SPRITE(pObstacleTemp)*0.5f);
				auto pMoveRight = MoveBy::create(fTimeRandom, ccp(fDistanceNeedMove, 0));
				auto pSequence = Sequence::create(pMoveRight, pMoveRight->reverse(), NULL);
				pObstacleTemp->runAction(RepeatForever::create(pSequence));
			}
			else
			{
				fDistanceNeedMove = (pObstacleTemp->getPositionX() - WIDTH_SPRITE(pObstacleTemp)*0.5f) - HALF_WIDTH_SCREEN;
				auto pMoveLeft = MoveBy::create(fTimeRandom, ccp(-fDistanceNeedMove, 0));
				auto pSequence = Sequence::create(pMoveLeft, pMoveLeft->reverse(), NULL);
				pObstacleTemp->runAction(RepeatForever::create(pSequence));
			}

		}
		isCreate = true;
		_vtObstacle.at(0)->setTag(TAG_FOR_GET_SCORE);
		_vtObstacle.at(2)->setTag(TAG_FOR_RANDOM);
	}
	else
	{
		auto pSpriteAtIndexZero		= _vtObstacle.at(0);
		auto pSpriteAtIndexOne		= _vtObstacle.at(1);
		auto pSpriteAtIndexTwo		= _vtObstacle.at(2);
		auto pSpriteAtIndexThree	= _vtObstacle.at(3);

		float fSpriteLeftX			= -WIDTH_SPRITE(pSpriteAtIndexZero) * 1 / 3 + rand() % 50;
		float fSpriteRightX			= WIDTH_SCREEN + WIDTH_SPRITE(pSpriteAtIndexZero) * 1 / 3 - rand() % 50;

		float fTimeRandom			= 0.8f + CCRANDOM_0_1();
		float fDisToRight			= 0.0f;
		float fDisToLeft			= 0.0f; 
		Action* pActionMoveRight	= nullptr;
		Action* pActionMoveLeft	= nullptr;

		if (pSpriteAtIndexZero->getTag() == TAG_FOR_RANDOM)
		{
			pSpriteAtIndexZero->setTag(TAG_FOR_GET_SCORE);
			pSpriteAtIndexTwo->setTag(TAG_FOR_RANDOM);				
			pSpriteAtIndexZero->setPosition(ccp(fSpriteLeftX, pSpriteAtIndexTwo->getPositionY() + 800));
			pSpriteAtIndexOne->setPosition(ccp(fSpriteRightX, pSpriteAtIndexZero->getPositionY()));

			
			fDisToRight			= HALF_WIDTH_SCREEN - (pSpriteAtIndexZero->getPositionX() + WIDTH_SPRITE(pSpriteAtIndexZero)*0.5f);
			fDisToLeft			= (pSpriteAtIndexOne->getPositionX() - WIDTH_SPRITE(pSpriteAtIndexOne)*0.5f) - HALF_WIDTH_SCREEN;
			pActionMoveRight	= moveAction(fDisToRight, fTimeRandom);
			pActionMoveLeft		= moveAction(-fDisToLeft, fTimeRandom);

			pSpriteAtIndexZero->stopAllActions();
			pSpriteAtIndexOne->stopAllActions();
			pSpriteAtIndexZero->runAction(pActionMoveRight);
			pSpriteAtIndexOne->runAction(pActionMoveLeft);
		}
		else
		{
			pSpriteAtIndexTwo->setTag(TAG_FOR_GET_SCORE);
			pSpriteAtIndexZero->setTag(TAG_FOR_RANDOM);
			pSpriteAtIndexTwo->setPosition(ccp(fSpriteLeftX, pSpriteAtIndexZero->getPositionY() + 800));
			pSpriteAtIndexThree->setPosition(ccp(fSpriteRightX,pSpriteAtIndexTwo->getPositionY()));
			
			fDisToRight			= HALF_WIDTH_SCREEN - (pSpriteAtIndexTwo->getPositionX() + WIDTH_SPRITE(pSpriteAtIndexZero)*0.5f);
			fDisToLeft			= (pSpriteAtIndexThree->getPositionX() - WIDTH_SPRITE(pSpriteAtIndexThree)*0.5f) - HALF_WIDTH_SCREEN;
			pActionMoveRight	= moveAction(fDisToRight, fTimeRandom);
			pActionMoveLeft		= moveAction(-fDisToLeft, fTimeRandom);

			pSpriteAtIndexTwo->stopAllActions();
			pSpriteAtIndexThree->stopAllActions();
			pSpriteAtIndexTwo->runAction(pActionMoveRight);
			pSpriteAtIndexThree->runAction(pActionMoveLeft);
		}
	}	
}

bool HelloWorld::onTouchBegan(Touch* pTouch, Event* pEvent)
{		
	if ( _pBall->getActionByTag(TAG_JUMP_ACTION) )
	{
		_pBall->stopAllActions();
	}
	
	auto pJumpUp	= JumpBy::create(1.0f, ccp(0, 300), 300, 1);
	auto pEasyOut	= EaseOut::create(pJumpUp, 1.0f);
	auto pFallDown	= JumpBy::create(1.0f, ccp(0, - ( _pBall->getPositionY() + WIDTH_SPRITE(_pBall)*0.5f) ), -150, 1);
	auto pEasyIn	= EaseIn::create(pFallDown, 0.75f);
	auto pSequence	= Sequence::create(pEasyOut, pEasyIn, NULL);
	pSequence->setTag(TAG_JUMP_ACTION);
	
	_pBall->runAction(pSequence);
	return true;
}

void HelloWorld::onTouchMoved(Touch* pTouch, CCEvent* pEvent)
{

}

void HelloWorld::onTouchCancelled(Touch* pTouch, CCEvent* pEvent)
{
	onTouchEnded(pTouch, pEvent);
}

void HelloWorld::onTouchEnded(Touch* pTouch, CCEvent* pEvent)
{

}

void HelloWorld::setPause()
{
	_isPlaying = false;
	for (int i = 0; i < _pNodeContaintHorizital->getChildrenCount(); i++)
	{
		auto pSpriteTemp = (Sprite*)_pNodeContaintHorizital->getChildren().at(i);
		pSpriteTemp->pause();
	}
}

void HelloWorld::setResume()
{
	_isPlaying = true;
	for (int i = 0; i < _pNodeContaintHorizital->getChildrenCount(); i++)
	{
		auto pSpriteTemp = (Sprite*)_pNodeContaintHorizital->getChildren().at(i);
		pSpriteTemp->resume();
	}

}

bool HelloWorld::getPlaying()
{
	return _isPlaying;
}
