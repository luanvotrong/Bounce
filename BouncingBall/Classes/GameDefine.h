#ifndef _DEFINE_GAME_H
#define _DEFINE_GAME_H

#define HALF_WIDTH_SCREEN						480
#define HALF_HEIGHT_SCREEN						800
#define WIDTH_SCREEN							960
#define HEIGHT_SCREEN							1600
#define SCALE_FACTOR							1

#define IMG_BALL								"images/ball.png"
#define IMG_BACKGROUND							"images/bgIngame_black.png"

#define POS_BALL								ccp(HALF_WIDTH_SCREEN, 64)
#define POS_BACKGROUND							ccp(HALF_WIDTH_SCREEN, HALF_HEIGHT_SCREEN)

#define WIDTH_SPRITE(_x)						_x->getContentSize().width
#define HEIGHT_SPRITE(_x)						_x->getContentSize().height

#define COLOR_FONT_SCORE						ccc3( 255, 0, 0)
#define DISTANCE_NEED_RANDOM					800
#define ADDITIVE								ccBlendFunc{GL_SRC_ALPHA, GL_ONE}

#define MOTION_STREAK_CONFIG					0.4, 1, WIDTH_SPRITE(_pBall), ccc3(255, 255, 255), "images/streak.png"

enum class ZOrder
{
	One=1,
	Two,
	Three,
	Four,
	Five,
	Six,
	Highest,
};

enum enumTagGame
{
	TAG_JUMP_ACTION = 30,
	TAG_FOR_RANDOM,
	TAG_FOR_GET_SCORE,
};

#endif