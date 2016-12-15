#include "AudioEngine.h"
#include "AboutUsScene.h"

USING_NS_CC;
using namespace cocos2d::experimental;

//Extern Records Data
extern double visibleSizeW;
extern double visibleSizeH;

extern double originX;
extern double originY;

extern bool haveSound;

Scene *AboutUsScene::createScene()
{
	auto scene = Scene::create();

	auto backGroundLayer = LayerGradient::create(Color4B(0x7C, 0xCD, 0x7C, 255), Color4B(0xFF, 0xB5, 0xC5, 255),Vec2(Vec2(originX,originY),Vec2(visibleSizeW,originY)));
	auto layer = AboutUsScene::create();

	scene->addChild(backGroundLayer);
	scene->addChild(layer);

	return scene;
}

bool AboutUsScene::init()
{
	if (!Layer::init())
	{
		return false;
	}

	TTFConfig ttfConfig("Fonts/verdana.ttf", 45);
	auto titleLabel = Label::createWithTTF(ttfConfig, "[Data Structure Curriculum Design 2016]\nThis is a A* Algorithm Visually Display Demo that shows Us How Computer Deal With Pathfinding.", TextHAlignment::CENTER, visibleSizeW);
	titleLabel->setPosition(Vec2((originX + visibleSizeW) / 2, (originY + visibleSizeH) / 2));
	addChild(titleLabel);
	
	auto backButton = MenuItemImage::create("IMG/MainScene/endButton.png", "IMG/MainScene/endButtonPushed.png", CC_CALLBACK_1(AboutUsScene::menuCallback, this));
	backButton->setPosition((originX + 80), (originY + visibleSizeH - 80));

	auto Menu = Menu::create(backButton, nullptr);
	Menu->setPosition(originX, originY);
	addChild(Menu);

	return true;
}

void AboutUsScene::menuCallback(Ref *sender)
{
	if (haveSound){ AudioEngine::play2d("Sounds/FX/gameStart.mp3", false, 0.5); }

	Director::getInstance()->popScene();
}