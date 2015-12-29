#include "HelloWorldScene.h"
#include "json/rapidjson.h"
#include "json/document.h"
#include "json/filestream.h"

USING_NS_CC;
#define HAS_MEMBER(var,data) data.HasMember(#var)
#define GET_VALUE_FROM_JSON(var, data, typedata)	if(HAS_MEMBER(var,data))	var = (typedata)data[#var].GetDouble()
#define POS_BALL			Vec2( 480, 100)
//#define MOVEUP_BOUNDARY		430
//#define DURATION_UP			0.6f
//#define DURATION_DOWN		0.6f
#define OBSTACLE_IMG		"images/horizital.png"
//#define OBSTACLE_START_Y	600.0f
//#define OBSTACLE_RAND_MIN	250.0f
//#define OBSTACLE_RAND_MAX	450.0f
//#define OBSTACLE_TIME_MIN	1.5f
//#define OBSTACLE_TIME_MAX	3.0f

float Random(float min, float max)
{
	float value = CCRANDOM_0_1();
	return (value * (max - min)) + min;
}

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
	
	return Vec2(pos.x / len, pos.y / len);
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
		
	LoadDefine("Blance.json");
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

	_pBall = nullptr;
	_pBall = Sprite::create(IMG_BALL);
	_pBall->setPosition(POS_BALL);
	addChild(_pBall, (int)ZOrder::Two);
	
	_streak = MotionStreak::create(MOTION_STREAK_CONFIG);
	_streak->setPosition(ccp(_pBall->getPositionX(),
								_pBall->getPositionY() ) );
	_streak->setBlendFunc(ADDITIVE);
	addChild(_streak, (int)ZOrder::One);

	_pLabelTTFScore = Label::createWithTTF("Score: ", "fonts/arial.ttf", 50);
	_pLabelTTFScore->setPosition(ccp(_pLabelTTFScore->getContentSize().width*0.5f, HEIGHT_SCREEN - _pLabelTTFScore->getContentSize().height*0.5f));
	_pLabelTTFScore->setColor(COLOR_FONT_SCORE);
	this->addChild(_pLabelTTFScore, (int)ZOrder::Highest);

	_pLabelBMFScore = Label::createWithTTF("Score: ", "fonts/arial.ttf", 50);
	_pLabelBMFScore->setColor(Color3B::RED);
	_pLabelBMFScore->setPosition(HALF_WIDTH_SCREEN, HEIGHT_SCREEN - 50);
	addChild(_pLabelBMFScore, (int)ZOrder::Highest);

	createObstacles(true);	

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
		Rect rect1 = _pBall->boundingBox();
		Rect rect2 = pHonizontal->boundingBox();
		rect2.origin += pHonizontal->convertToWorldSpace(pHonizontal->getPosition());
		if (rect1.intersectsRect(rect2))
		{
			_isPlaying = false;
			gameOver();
			return;
		}
	}
	
	if (_vtObstacle[_vtObstacle.size()-1]->getPositionY() < HEIGHT_SCREEN)
	{
		createObstacles(false);
	}

	if (_pBall->getPositionY() > MOVEUP_BOUNDARY)
	{
		auto action = EaseIn::create(MoveBy::create(0.5f, Vec2(0, -20)), 0.5f);
		_pNodeContaintHorizital->runAction(action);
	}

	_pLabelBMFScore->setString(std::to_string(_nScore));
	_streak->setPositionX(_pBall->getPositionX()/* - WIDTH_SPRITE(_pBall)*0.5f*SCALE_FACTOR*/);
	_streak->setPositionY(_pBall->getPositionY()/* - HEIGHT_SPRITE(_pBall)*0.5f*SCALE_FACTOR*/);
}

void HelloWorld::moveCameraAndPauseGame(float fYBall)
{
	_isPlaying = false;
	for ( auto pChild : _pNodeContaintHorizital->getChildren())
	{
		pChild->pause();
	}
	auto pCallResumeGame = CallFunc::create(CC_CALLBACK_0(HelloWorld::resumeGame, this));
	auto pMoveUp	= MoveBy::create(0.75f, Vec2(0, fYBall));
	auto pSequence = Sequence::create(pMoveUp, pCallResumeGame, nullptr);
	_pNodeContaintHorizital->runAction(pSequence);
}

void HelloWorld::resumeGame()
{
	_isPlaying = true;
	for (auto pChild : _pNodeContaintHorizital->getChildren())
	{
		pChild->resume();
	}
}

void HelloWorld::gameOver()
{
	setPause();
	this->unscheduleUpdate();
	auto pSpriteGameOver = Sprite::create("images/gameOver.png");
	pSpriteGameOver->setPosition(Vec2(HALF_WIDTH_SCREEN, HALF_HEIGHT_SCREEN));
	addChild(pSpriteGameOver, (int)ZOrder::Highest);
	this->setTouchEnabled(false);
}


Action* HelloWorld::moveAction(float fDeltaMove, float fDelayTime)
{	
	auto pMoveRight = MoveBy::create(fDelayTime, Vec2(fDeltaMove, 0));
	auto pSequence = Sequence::create(pMoveRight, pMoveRight->reverse(), NULL);
	
	return RepeatForever::create(pSequence);
}

void HelloWorld::createObstacle(float y)
{
	//Create
	auto leftTemp = Sprite::create(OBSTACLE_IMG);
	auto rightTemp = Sprite::create(OBSTACLE_IMG);
	float rightX = WIDTH_SCREEN + WIDTH_SPRITE(rightTemp) * 1 / 3 - rand() % 50; // obstacle right				
	float leftX = -WIDTH_SPRITE(leftTemp) * 1 / 3 + rand() % 50; // obstacle left

	//Set position, add to screen
	leftTemp->setPosition(ccp(leftX, y));
	rightTemp->setPosition(ccp(rightX, y));
	_pNodeContaintHorizital->addChild(leftTemp, (int)ZOrder::Two);
	_pNodeContaintHorizital->addChild(rightTemp, (int)ZOrder::Two);
	_vtObstacle.push_back(leftTemp);
	_vtObstacle.push_back(rightTemp);

	//Action
	float fDistanceNeedMove = HALF_WIDTH_SCREEN - (leftTemp->getPositionX() + WIDTH_SPRITE(leftTemp)*0.5f);
	float time = Random(OBSTACLE_TIME_MIN, OBSTACLE_TIME_MAX);
	auto pMoveRight = MoveBy::create(time, Vec2(fDistanceNeedMove, 0));
	auto pSequence = Sequence::create(pMoveRight, pMoveRight->reverse(), NULL);
	leftTemp->runAction(RepeatForever::create(pSequence));

	fDistanceNeedMove = (rightTemp->getPositionX() - WIDTH_SPRITE(rightTemp)*0.5f) - HALF_WIDTH_SCREEN;
	auto pMoveLeft = MoveBy::create(time, Vec2(-fDistanceNeedMove, 0));
	pSequence = Sequence::create(pMoveLeft, pMoveLeft->reverse(), NULL);
	rightTemp->runAction(RepeatForever::create(pSequence));
}

void HelloWorld::createObstacles(bool isInit)
{
	if (isInit)
	{
		float y = OBSTACLE_START_Y;
		while (y <= HEIGHT_SCREEN)
		{
			//Create obstacle.
			createObstacle(y);
			//Sprite* obstacle = Sprite::create(OBSTACLE_IMG);
			//_vtObstacle.push_back(obstacle);
			y += (rand() % (int)(OBSTACLE_RAND_MAX-OBSTACLE_RAND_MIN)) + OBSTACLE_RAND_MIN;
		}
	}
	else
	{
		float y = _vtObstacle[_vtObstacle.size() - 1]->getPositionY();
		y += (rand() % (int)(OBSTACLE_RAND_MAX - OBSTACLE_RAND_MIN)) + OBSTACLE_RAND_MIN;
		//Create obstacle.
		createObstacle(y);
		//Sprite* obstacle = Sprite::create(OBSTACLE_IMG);
		//_vtObstacle.push_back(obstacle);
	}
}

bool HelloWorld::onTouchBegan(Touch* pTouch, Event* pEvent)
{	
	if (!_isPlaying)
	{
		return false;
	}

	if (_pBall->getActionByTag(TAG_JUMP_ACTION) )
	{
		_pBall->stopAllActions();
	}
	
	auto pJumpUp	= JumpTo::create(DURATION_UP, POS_BALL, 450, 1);
	auto pEasyOut	= EaseIn::create(pJumpUp, DURATION_UP);
	//auto pFallDown	= JumpBy::create(1.0f, Vec2(0, ( _pBall->getPositionY() + WIDTH_SPRITE(_pBall)*0.5f) ), 150, 1);
	//auto pFallDown = MoveTo::create(DURATION_DOWN, POS_BALL);
	//auto pEasyIn	= EaseOut::create(pFallDown, DURATION_DOWN);
	//auto pSequence	= Sequence::create(pEasyOut, pEasyIn, NULL
	auto pSequence = Sequence::create(pEasyOut, NULL);
	pSequence->setTag(TAG_JUMP_ACTION);
	
	_pBall->runAction(pSequence);
	return true;
}

void HelloWorld::onTouchMoved(Touch* pTouch, Event* pEvent)
{

}

void HelloWorld::onTouchCancelled(Touch* pTouch, Event* pEvent)
{
	onTouchEnded(pTouch, pEvent);
}

void HelloWorld::onTouchEnded(Touch* pTouch, Event* pEvent)
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

void HelloWorld::LoadDefine(char * jsonName)
{
	std::ifstream myJson(CCFileUtils::sharedFileUtils()->fullPathForFilename(jsonName));

	std::string contents((std::istreambuf_iterator<char>(myJson)),std::istreambuf_iterator<char>());

	rapidjson::Document balanceJson;
	balanceJson.Parse<0>(contents.c_str());

	if (balanceJson.IsNull())
		return;

	GET_VALUE_FROM_JSON(MOVEUP_BOUNDARY,		balanceJson, int);
	GET_VALUE_FROM_JSON(DURATION_UP,			balanceJson, float);
	GET_VALUE_FROM_JSON(DURATION_DOWN,			balanceJson, float);
	GET_VALUE_FROM_JSON(OBSTACLE_START_Y,		balanceJson, float);
	GET_VALUE_FROM_JSON(OBSTACLE_RAND_MIN,		balanceJson, float);
	GET_VALUE_FROM_JSON(OBSTACLE_RAND_MAX,		balanceJson, float);
	GET_VALUE_FROM_JSON(OBSTACLE_TIME_MIN,		balanceJson, float);
	GET_VALUE_FROM_JSON(OBSTACLE_TIME_MAX,		balanceJson, float);
}