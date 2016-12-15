#include <vector>
#include "AudioEngine.h"
#include "MapEditorScene.h"
#include "AstarScene.h"

USING_NS_CC;
using namespace std;
using namespace cocos2d::experimental;

//Extern Records Data
extern double visibleSizeW;
extern double visibleSizeH;

extern double originX;
extern double originY;

extern bool haveSound;

//Astar Map
extern vector<vector<int>> aStarMap;

//Origins and End Point
extern int startPointLine;
extern int startPointColumn;
extern int endPointLine;
extern int endPointColumn;

//Map Selected Items Pointer
MenuItemImage *MSIP[3];

//Sprite Map
vector<vector<Sprite*>> SpriteMatrix;

//Touch Listener
EventListenerTouchOneByOne *TouchListener;

Scene *MapEditorScene::createScene()
{
	auto scene = Scene::create();

	auto backGroundLayer = LayerGradient::create(Color4B(0x48, 0x3D, 0x88, 255), Color4B(0x00, 0x64, 0x00, 255));
	auto layer = MapEditorScene::create();

	scene->addChild(backGroundLayer);
	scene->addChild(layer);

	return scene;
}

bool MapEditorScene::init()
{
	if (!Layer::init())
	{
		return false;
	}

	//Matrix Map Initialize
	int startX = originX+35, startY = visibleSizeH - 35;
	for (int i=0;i<14;++i)
	{
		vector<Sprite*> temp;
		for (int j=0;j<22;++j)
		{
			auto Diamond = Sprite::create("IMG/Map/baseMapDiamond.png");
			Diamond->setPosition(startX + 50 * j, startY - 50 * i);
			addChild(Diamond);
			temp.push_back(Diamond);
		}
		SpriteMatrix.push_back(temp);
	}

	vector<int> temp(22,1);
	for(int i=0;i<14;++i)
	{
		aStarMap.push_back(temp);
	}

	//Select Menu Initialize
	auto WallSelectButton = MenuItemImage::create("IMG/Map/WallSelectButton.png", "IMG/Map/WallSelectButtonPushed.png", "IMG/Map/WallSelectButtonUsed.png",CC_CALLBACK_1(MapEditorScene::menuCallback, this));
	auto OriginSelectButton = MenuItemImage::create("IMG/Map/OriginSelectButton.png", "IMG/Map/OriginSelectButtonPushed.png", "IMG/Map/OriginSelectButtonUsed.png",CC_CALLBACK_1(MapEditorScene::menuCallback, this));
	auto EndPointSelectButton = MenuItemImage::create("IMG/Map/EndPointSelectButton.png", "IMG/Map/EndPointSelectButtonPushed.png", "IMG/Map/EndPointSelectButtonUsed.png",CC_CALLBACK_1(MapEditorScene::menuCallback, this));
	auto NextButton = MenuItemImage::create("IMG/Map/NextButton.png", "IMG/Map/NextButtonPushed.png","IMG/Map/NextButtonUsed.png", CC_CALLBACK_1(MapEditorScene::menuCallback, this));
	auto BackButton = MenuItemImage::create("IMG/Map/BackButton.png", "IMG/Map/BackButtonPushed.png","IMG/Map/BackButtonUsed.png", CC_CALLBACK_1(MapEditorScene::menuCallback, this));

	MSIP[0] = WallSelectButton;
	MSIP[1] = OriginSelectButton;
	MSIP[2] = EndPointSelectButton;

	BackButton->setPosition(visibleSizeW - 90,visibleSizeH - 80);
	WallSelectButton->setPosition(visibleSizeW - 90, visibleSizeH - 220);
	OriginSelectButton->setPosition(visibleSizeW - 90, visibleSizeH - 360);
	EndPointSelectButton->setPosition(visibleSizeW - 90, visibleSizeH - 500);
	NextButton->setPosition(visibleSizeW - 90, visibleSizeH - 640);

	BackButton->setTag(0);
	WallSelectButton->setTag(1);
	OriginSelectButton->setTag(2);
	EndPointSelectButton->setTag(3);
	NextButton->setTag(4);

	auto Menu = Menu::create(WallSelectButton, OriginSelectButton, EndPointSelectButton, NextButton, BackButton, nullptr);
	Menu->setPosition(originX, originY);
	addChild(Menu);

	//Touch Event Feed Back
	TouEvent();

	return true;
}

void MapEditorScene::menuCallback(Ref *sender)
{
	if (haveSound) { AudioEngine::play2d("Sounds/FX/gameStart.mp3", false, 0.5); }
	auto selector = static_cast<MenuItemImage*>(sender);

	switch (selector->getTag())
	{
	//Back Button Was Pushed
	case 0:
	{
		aStarMap.clear();
		SpriteMatrix.clear();
		MSIP[0] = nullptr; MSIP[1] = nullptr; MSIP[2] = nullptr;

		//Remove Touch Event Listener
		Director::getInstance()->getEventDispatcher()->removeEventListener(TouchListener);
		//Pop Here
		Director::getInstance()->popScene();
	}
	break;

	//Wall Select Button Was Pushed
	case 1:
	{
		MSIP[0]->setEnabled(false);
		MSIP[1]->setEnabled(true);
		MSIP[2]->setEnabled(true);
	}
	break;

	//Origin Select Button Was Pushed
	case 2:
	{
		MSIP[0]->setEnabled(true);
		MSIP[1]->setEnabled(false);
		MSIP[2]->setEnabled(true);
	}
    break;

	//End Point Select Button Was Pushed
	case 3:
	{
		MSIP[0]->setEnabled(true);
		MSIP[1]->setEnabled(true);
		MSIP[2]->setEnabled(false);
	}
	break;

	//Next Button Was Pushed
	case 4:
	{
		SpriteMatrix.clear();
		MSIP[0] = nullptr; MSIP[1] = nullptr; MSIP[2] = nullptr;

		//Remove Touch Event Listener
		Director::getInstance()->getEventDispatcher()->removeEventListener(TouchListener);

		//Goto Next Scene
		auto NextScene = AstarScene::createScene();
		Director::getInstance()->replaceScene(TransitionFadeDown::create(0.7, NextScene));
	}
	break;
	}
}

void MapEditorScene::TouEvent()
{
	TouchListener = EventListenerTouchOneByOne::create();

	TouchListener->onTouchBegan = CC_CALLBACK_2(MapEditorScene::onTouchBegan, this);
	TouchListener->onTouchMoved = CC_CALLBACK_2(MapEditorScene::onTouchMoved, this);
	TouchListener->onTouchEnded = CC_CALLBACK_2(MapEditorScene::onTouchEnded, this);

	Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(TouchListener, 1);
}

bool MapEditorScene::onTouchBegan(Touch *touch, Event *unused_ecent)
{
	Point location = touch->getLocation();
	double locationX = location.x;
	double locationY = location.y;

	int MatrixColumn = (locationX - 10) / 50;
	int MatrixLine = 14 - (locationY - 10) / 50;

	if (MatrixColumn >= 0 && MatrixColumn <= 21 && MatrixLine >= 0 && MatrixLine <= 13)
	{
		if (!MSIP[0]->isEnabled())
		{
			aStarMap[MatrixLine][MatrixColumn] = 0;
			SpriteMatrix[MatrixLine][MatrixColumn]->setColor(Color3B(0xFF,0x00,0x00));
		}
	}

	return true;
}

void MapEditorScene::onTouchMoved(Touch *touch, Event *unused_ecent)
{
	Point location = touch->getLocation();
	double locationX = location.x;
	double locationY = location.y;

	int MatrixColumn = (locationX - 10) / 50;
	int MatrixLine = 14 - (locationY - 10) / 50;

	if (MatrixColumn >= 0 && MatrixColumn <= 21 && MatrixLine >= 0 && MatrixLine <= 13)
	{
		if (!MSIP[0]->isEnabled())
		{
			aStarMap[MatrixLine][MatrixColumn] = 0;
			SpriteMatrix[MatrixLine][MatrixColumn]->setColor(Color3B(0xFF, 0x00, 0x00));
		}
	}
}

void MapEditorScene::onTouchEnded(Touch *touch, Event *unused_ecent)
{
	Point location = touch->getLocation();
	double locationX = location.x;
	double locationY = location.y;

	int MatrixColumn = (locationX - 10) / 50;
	int MatrixLine = 14 - (locationY - 10) / 50;

	if (MatrixColumn >= 0 && MatrixColumn <= 21 && MatrixLine >= 0 && MatrixLine <= 13)
	{
		if (!MSIP[1]->isEnabled())
		{
			SpriteMatrix[startPointLine][startPointColumn]->setColor(Color3B(0xFF, 0xFF, 0xFF));

			aStarMap[MatrixLine][MatrixColumn] = 1;
			startPointLine = MatrixLine;
			startPointColumn = MatrixColumn;
			
			SpriteMatrix[MatrixLine][MatrixColumn]->setColor(Color3B(0xFF, 0xFF, 0x00));
		}
		else if(!MSIP[2]->isEnabled())
		{
			SpriteMatrix[endPointLine][endPointColumn]->setColor(Color3B(0xFF, 0xFF, 0xFF));

			aStarMap[MatrixLine][MatrixColumn] = 1;
			endPointLine = MatrixLine;
			endPointColumn = MatrixColumn;

			SpriteMatrix[MatrixLine][MatrixColumn]->setColor(Color3B(0x00, 0x00, 0xFF));
		}
	}
}