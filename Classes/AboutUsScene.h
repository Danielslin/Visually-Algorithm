#ifndef _ABOUT_US_SCENE_H_
#define _ABOUT_US_SCENE_H_

#include "cocos2d.h"

class AboutUsScene : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();

	void menuCallback(cocos2d::Ref* sender);

	CREATE_FUNC(AboutUsScene);
};

#endif