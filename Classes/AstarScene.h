#ifndef _A_STAR_SCENE_H_
#define _A_STAR_SCENE_H_

#include "cocos2d.h"

class AstarScene : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();

	void menuCallback(cocos2d::Ref* sender);

	CREATE_FUNC(AstarScene);
};

#endif