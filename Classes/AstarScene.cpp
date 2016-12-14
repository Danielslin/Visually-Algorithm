#include <vector>
#include "AudioEngine.h"
#include "AstarScene.h"
#include "MainScene.h"
#include "Astar.h"

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
extern vector<vector<int>> AstarMap;

//Origins and End Point
extern int startPointX;
extern int startPointY;
extern int endPointX;
extern int endPointY;

//Sprite Map
vector<vector<Sprite*>> SpritesMatrix;

Scene *AstarScene::createScene()
{
	auto scene = Scene::create();

	auto backGroundLayer = LayerGradient::create(Color4B(0x48, 0x3D, 0x88, 255), Color4B(0x00, 0x64, 0x00, 255));
	auto layer = AstarScene::create();

	scene->addChild(backGroundLayer);
	scene->addChild(layer);

	return scene;
}

bool AstarScene::init()
{
	if (!Layer::init())
	{
		return false;
	}

	//Matrix Map initialize
	int startX = originX + 35, startY = originY + 35;
	for (int i = 0; i<14; ++i)
	{
		vector<Sprite*> temp;
		for (int j = 0; j<22; ++j)
		{
			auto Diamond = Sprite::create("IMG/Map/baseMapDiamond.png");
			Diamond->setPosition(startX + 50 * j, startY + 50 * i);

			if (AstarMap[13-i][j]==0)
			{
				Diamond->setColor(Color3B(0xFF,0x00,0x00));
			}
			else if (13-i == startPointX && j == startPointY)
			{
				Diamond->setColor(Color3B(0xFF, 0xFF, 0x00));
			}
			else if (13-i == endPointX && j == endPointY)
			{
				Diamond->setColor(Color3B(0x00, 0x00, 0xFF));
			}

			addChild(Diamond);
			temp.push_back(Diamond);
		}
		SpritesMatrix.push_back(temp);
	}

	//Initialize Menu
	auto runButton = MenuItemImage::create("IMG/Map/NextButton.png", "IMG/Map/NextButtonPushed.png", CC_CALLBACK_1(AstarScene::menuCallback, this));
	auto backButton = MenuItemImage::create("IMG/MainScene/endButton.png", "IMG/MainScene/endButtonPushed.png", CC_CALLBACK_1(AstarScene::menuCallback, this));

	runButton->setTag(0);
	backButton->setTag(1);

	runButton->setPosition(visibleSizeW - 90, visibleSizeH - 80);
	backButton->setPosition(visibleSizeW - 90, visibleSizeH - 640);

	auto Menu = Menu::create(runButton, backButton, nullptr);
	Menu->setPosition(originX, originY);
	addChild(Menu);

	return true;
}

void AstarScene::menuCallback(Ref *sender)
{
	if (haveSound)
	{
		AudioEngine::play2d("Sounds/FX/gameStart.mp3", false, 0.5);
	}

	auto selector = static_cast<MenuItemImage*>(sender);

	switch (selector->getTag())
	{
	case 0:
	{
		vector<vector<int>> FinalMatrix = Astar(startPointX, startPointY, endPointX, endPointY, AstarMap, SpritesMatrix);

		//Final Matrix Tintage
		for (auto &NodeOfRoad:FinalMatrix)
		{
			SpritesMatrix[13-NodeOfRoad[0]][NodeOfRoad[1]]->setColor(Color3B(0x00,0x00,0x00));
		}
	}
	break;

	case 1:
	{
		AstarMap.clear();
		SpritesMatrix.clear();

		startPointX = 0; startPointY = 0; endPointX = 0; endPointY = 0;

		auto NextScene = MainScene::createScene();
		Director::getInstance()->replaceScene(TransitionFadeDown::create(0.7, NextScene));
	}
	break;
	}
}