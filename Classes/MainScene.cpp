#include "AudioEngine.h"
#include "MainScene.h"
#include "AboutUsScene.h"
#include "MapEditorScene.h"

USING_NS_CC;
using namespace cocos2d::experimental;

//Extern Records Data
extern double visibleSizeW;
extern double visibleSizeH;

extern double originX;
extern double originY;

extern int audioId;
extern bool haveSound;

Scene* MainScene::createScene()
{
    auto scene = Scene::create();
	
	auto backGroundLayer = LayerGradient::create(Color4B(0xBF, 0xEF, 0xFF, 255),Color4B(0xAB, 0x82, 0xFF,255));
	auto layer = MainScene::create();

    scene->addChild(backGroundLayer);
	scene->addChild(layer);

    return scene;
}

bool MainScene::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    //Load Title Label
	TTFConfig ttfConfig("Fonts/verdana.ttf", 60);
	auto titleLabel = Label::createWithTTF(ttfConfig,"A* Algorithm Visually Display", TextHAlignment::CENTER,visibleSizeW);
	titleLabel->setPosition(Vec2((originX + visibleSizeW) / 2, visibleSizeH + originY - 200));
	addChild(titleLabel);

	//Load Main Menu
	auto startButton = MenuItemImage::create("IMG/MainScene/startButton.png","IMG/MainScene/startButtonPushed.png",CC_CALLBACK_1(MainScene::menuCallback,this));
	auto BGMButton = MenuItemImage::create("IMG/MainScene/BGMButton.png", "IMG/MainScene/BGMButton.png",CC_CALLBACK_1(MainScene::menuCallback, this));
	auto endButton = MenuItemImage::create("IMG/MainScene/endButton.png", "IMG/MainScene/endButtonPushed.png", CC_CALLBACK_1(MainScene::menuCallback, this));
	auto aboutUsButton = MenuItemImage::create("IMG/MainScene/aboutUsButton.png", "IMG/MainScene/aboutUsButtonPushed.png", CC_CALLBACK_1(MainScene::menuCallback, this));

	startButton->setTag(0);
	BGMButton->setTag(1);
	endButton->setTag(2);
	aboutUsButton->setTag(5);

	startButton->setPosition((originX + visibleSizeW)/2, (originY + visibleSizeH) / 2);
	BGMButton->setPosition((originX + visibleSizeW - 80),(originY + visibleSizeH - 80));
	endButton->setPosition((originX + 80), (originY + visibleSizeH - 80));
	aboutUsButton->setPosition(originX+80,originY+80);

	auto Menu = Menu::create(startButton,BGMButton,endButton,aboutUsButton,nullptr);
	Menu->setPosition(originX,originY);
	addChild(Menu);

    return true;
}

void MainScene::menuCallback(Ref *sender)
{
	auto selector = static_cast<MenuItemImage*>(sender);

	switch (selector->getTag())
	{
	case 0:
	{
		if (haveSound)
		{
			AudioEngine::play2d("Sounds/FX/gameStart.mp3", false, 0.5);
		}
		
		//Start Scene Editor
		auto NextScene = MapEditorScene::createScene();
		Director::getInstance()->pushScene(TransitionFadeUp::create(0.7, NextScene));
	}
	break;

	case 1:
	{
		selector->setTag(3);
		auto No = Sprite::create("IMG/MainScene/No.png");
		No->setPosition((originX + visibleSizeW - 80), (originY + visibleSizeH - 80));
		No->setTag(4);
		addChild(No);

		haveSound = false;
		AudioEngine::pause(audioId);
	}
	break;

	case 2:
		Director::getInstance()->end();
        #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
		exit(0);
        #endif
		break;

	case 3:
		selector->setTag(1);
		removeChildByTag(4);

		haveSound = true;
		AudioEngine::resume(audioId);

		if (haveSound)
		{
			AudioEngine::play2d("Sounds/FX/gameStart.mp3", false, 0.5);
		}
		break;

	case 5:
	{
		//Switch to About Us Scene
		if (haveSound)
		{
			AudioEngine::play2d("Sounds/FX/gameStart.mp3", false, 0.5);
		}

		//Start About Us Scene
		auto NextScene = AboutUsScene::createScene();
		Director::getInstance()->pushScene(TransitionFadeDown::create(0.7, NextScene));
	}
	break;
	}
}