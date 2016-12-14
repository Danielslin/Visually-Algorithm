#ifndef _MAP_EDITOR_H_
#define _MAP_EDITOR_H_

#include "cocos2d.h"

class MapEditorScene : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();

	void menuCallback(cocos2d::Ref* sender);

	void TouEvent();
	virtual bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_ecent);
	virtual void onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *unused_ecent);
	virtual void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_ecent);

	CREATE_FUNC(MapEditorScene);
};

#endif // !_MAP_EDITOR_H_