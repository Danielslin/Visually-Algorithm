#include "Astar.h"

using namespace std;

//用于记录开启列表中元素的个数
int openListCount = 0;

//用于记录关闭列表中元素的个数
int closeListCount = 0;

//用于记录邻居的个数
int neibNum = 0;

//用来记录路径经过的点的个数
int AstackCount = 0;

//用于记录关闭列表里是否有终点
int FLAG = 0;

int startPointx=0, startPointy=0, endPointx=0, endPointy=0, sizex=0, sizey=0;

//Render List
extern vector<vector<int>> renderList;

vector<vector<int>> Astar(int startPointX, int startPointY, int endPointX, int endPointY, const vector<vector<int>> &firstMap)
{
	auto be = firstMap.begin();

	const int sizeX = firstMap.size();
	const int sizeY = be->size();

	vector<vector<baseNode>> AstarMap = inputMap(sizeX,sizeY,firstMap);

	//开启列表，用于A*算法
	vector<baseNode> OpenList(sizeX*sizeY);

	//关闭列表，用于A*算法
	vector<baseNode> CloseList(sizeX*sizeY);

	//用来储存整理后的路径
	vector<baseNode> Astack(sizeX*sizeY);

	//邻居节点
	baseNode Neibo[8];

	startPointx = startPointX;
	startPointy = startPointY;
	endPointx = endPointX;
	endPointy = endPointY;
	sizex = sizeX;
	sizey = sizeY;

	return AstarO(Astack,Neibo,AstarMap,OpenList,CloseList);
}

vector<vector<baseNode>> inputMap(int sizeX,int sizeY, const vector<vector<int>> &firstMap)
{
	vector<vector<baseNode>> AstarMap;
	vector<baseNode> temp(sizeY);

	for (int i=0;i<sizeX;++i)
	{
		AstarMap.push_back(temp);
	}

	for (int i=0;i<sizeX;++i)
	{
		for (int j=0;j<sizeY;++j)
		{
			AstarMap[i][j].weigt=firstMap[i][j];

			AstarMap[i][j].i = i;
			AstarMap[i][j].j = j;
		}
	}

	return AstarMap;
}

//以下函数都是A*算法的一部分///////////////////////////

//把一个元素插入开启列表中，使开启列表符合最小二叉堆的性质/////////
void putInOpenList(baseNode &inList, vector<baseNode> &OpenList)
{
	//Push Diamonds into Render List
	vector<int> RenderTemp(3,0);
	RenderTemp[0] = inList.i;
	RenderTemp[1] = inList.j;
	RenderTemp[2] = 1;
	renderList.push_back(RenderTemp);

	++openListCount;

	OpenList[openListCount - 1] = inList;
	for (int i = openListCount - 1; i >0; i = (i - 1) / 2)
	{
		if (OpenList[i].f<OpenList[(i - 1) / 2].f)
		{
			OpenList[i] = OpenList[(i - 1) / 2];
			OpenList[(i - 1) / 2] = inList;
		}
		else
		{
			break;
		}
	}
}

//取出开启列表（最小二叉堆）中最小的数
baseNode readTopOpenList(vector<baseNode> &OpenList)
{
	return OpenList[0];
}

//把一个元素加入关闭列表中，使关闭列表有序
void putInCloseList(baseNode &temp, vector<baseNode> &CloseList)
{
	vector<int> RenderTemp(3, 0);
	RenderTemp[0] = temp.i;
	RenderTemp[1] = temp.j;
	RenderTemp[2] = 2;
	renderList.push_back(RenderTemp);

	CloseList[closeListCount] = temp;

	struct baseNode iter;
	for (int i = closeListCount; i>0; --i)
	{
		if ((CloseList[i].i*1000 + CloseList[i].j + 1)<(CloseList[i - 1].i*1000 + CloseList[i - 1].j + 1))
		{
			iter = CloseList[i];
			CloseList[i] = CloseList[i - 1];
			CloseList[i - 1] = iter;
		}
		else
		{
			break;
		}
	}
	++closeListCount;
}

//把开启列表中的当前节点删除，使其符合最小二叉堆的性质
void outOpenList(vector<baseNode> &OpenList)
{
	--openListCount;

	OpenList[0] = OpenList[openListCount];
	baseNode temp;

	for (int i = 0, p = 0; 2 * i + 1<openListCount; i = p)
	{
		if (2 * i + 2>openListCount - 1)
		{
			p = 2 * i + 1;
		}
		else
		{
			if (OpenList[2 * i + 1].f<OpenList[2 * i + 2].f)
			{
				p = 2 * i + 1;
			}
			else
			{
				p = 2 * i + 2;
			}
		}

		if (OpenList[p].f<OpenList[i].f)
		{
			temp = OpenList[p];
			OpenList[p] = OpenList[i];
			OpenList[i] = temp;
		}
		else
		{
			break;
		}
	}
}

//删除开启列表中指定的元素，重构最小二叉堆
//（注：这个可以用很多方法实现，示例并非最优）
void outOpenList2(baseNode &iter, vector<baseNode> &OpenList)
{
	int number = openListCount - 1;
	vector<baseNode> openlist2(sizex*sizey);
	for (int i = 0; i < openListCount; ++i)
	{
		if (OpenList[i].i == iter.i&&OpenList[i].j == iter.j)
		{
			continue;
		}
		openlist2[i] = OpenList[i];
	}

	openListCount = 0;
	for (int i = 0; i < number; ++i)
	{
		putInOpenList(openlist2[i],OpenList);
	}
}

//对于一路上的每个点，分析它的最多八个邻居，并加入邻居列表
void addNeibo(baseNode &iter,baseNode (&Neibo)[8], vector<vector<baseNode>> &AstarMap)
{
	neibNum = 0;

	for (int i = iter.i - 1; i <= iter.i + 1; ++i)
	{
		for (int j = iter.j - 1; j <= iter.j + 1; ++j)
		{
			if (i >= 0 && i <= sizex - 1 && j >= 0 && j <= sizey - 1)
			{
				if (i == iter.i&&j == iter.j)
				{
				}
				else
				{
					AstarMap[i][j].h = manhatten(i, j);

					Neibo[neibNum] = AstarMap[i][j];
					++neibNum;
				}
			}
		}
	}
}

//查看临近格是否在开启列表中的函数
int isInOpenList(baseNode &neibo, vector<baseNode> &OpenList)
{
	for (int i = 0; i < openListCount - 1; ++i)
	{
		if (OpenList[i].i == neibo.i&&OpenList[i].j == neibo.j)
		{
			return 1;
		}
	}
	return 0;
}

//查看指定的temp在不在关闭列表中的函数，使用了折半查找
int isInCloseList(baseNode &temp, vector<baseNode> &CloseList)
{
	int low = 0, high = closeListCount - 1, mid = (low + high) / 2;

	for (; low <= high; mid = (low + high) / 2)
	{
		if ((CloseList[mid].i*1000 + CloseList[mid].j + 1) == (temp.i*1000 + temp.j + 1))
		{
			return 1;
		}
		else if ((CloseList[mid].i*1000 + CloseList[mid].j + 1) > (temp.i*1000 + temp.j + 1))
		{
			high = mid - 1;
		}
		else
		{
			low = mid + 1;
		}
	}
	return 0;
}

//A*中的启发式函数，用于求指定位置和终点之间的曼哈顿距离
int manhatten(int i, int j)
{
	int a = (abs(endPointx - i) + abs(endPointy - j));
	if (a == 0)
	{
		FLAG = 1;
	}

	return a * 10;
}

//求当前点与父亲节点的距离
int increment(baseNode &nthis)
{
	if ((abs(nthis.father->i - nthis.i) == 1) && (abs(nthis.father->j - nthis.j) == 1))
	{
		return 14 * nthis.weigt;
	}
	else if ((nthis.father->i - nthis.i) == 0 && (nthis.father->j - nthis.j) == 0)
	{
		return 0;
	}
	else
	{
		return 10 * nthis.weigt;
	}
}

//求出用当前点作为父节点时这个点的G值
int NewG(baseNode &nthis, baseNode &father)
{
	if (abs(father.i - nthis.i) == 1 && abs(father.j - nthis.j) == 1)
	{
		return father.g + 14;
	}
	else if (abs(father.i - nthis.i) == 0 && abs(father.j - nthis.j) == 0)
	{
		return father.g;
	}
	else
	{
		return father.g + 10;
	}
}

//把A*算法的节点按倒序整理到Astack里面
void arrange(baseNode &iter, vector<baseNode> &Astack, vector<vector<baseNode>> &AstarMap)
{
	AstackCount = 0;
	for (;iter.father!=nullptr ; iter = AstarMap[iter.father->i][iter.father->j])
	{
		Astack[AstackCount] = iter;
		++AstackCount;
		if (iter.i == startPointx&&iter.j == startPointy)
		{
			break;
		}
	}
}

//Astar的本体
vector<vector<int>> AstarO(vector<baseNode> &Astack, baseNode(&Neibo)[8], vector<vector<baseNode>> &AstarMap, vector<baseNode> &OpenList, vector<baseNode> &CloseList)
{
	//FLAG用于判断终点是否在关闭列表
	FLAG = 0;
	//每次执行A*算法，都初始化开启/关闭列表
	openListCount = 0;
	closeListCount = 0;

	//创建一个迭代器,每次都等于f值最小的节点
	baseNode iter;

	//让这个迭代器的初值为起点
	iter.i = startPointx;
	iter.j = startPointy;
	iter.weigt = AstarMap[startPointx][startPointy].weigt;

	//起点的没有父节点，且为唯一G值为0的点
	iter.g = 0;
	iter.h = manhatten(iter.i, iter.j);
	iter.f = iter.g + iter.h;

	//创建终点
	baseNode ender;

	ender.i = endPointx;
	ender.j = endPointy;

	//把起点放入开启列表
	putInOpenList(iter,OpenList);

	//当开启列表为空或者终点在关闭列表中，结束寻径
	for (; openListCount != 0 && FLAG == 0;)
	{
		//取出开启列表中f值最小的节点（之一），并设为iter（当前点）
		iter = readTopOpenList(OpenList);

		//把最小点从开启列表中删除
		outOpenList(OpenList);

		//把当前点记录在关闭列表中
		putInCloseList(iter,CloseList);

		//把当前点的邻居加入邻居列表
		addNeibo(iter,Neibo, AstarMap);

		//对于每个邻居，分三种情况进行操作
		for (int i = 0; i < neibNum; ++i)
		{
			//如果这个邻居节点不可通过，或者这个邻居节点在关闭列表中，略过它
			if (Neibo[i].weigt == 0 || isInCloseList(Neibo[i],CloseList))
			{
			}
			//如果这个邻居节点已经在开启列表中
			else if (isInOpenList(Neibo[i],OpenList))
			{
				if (NewG(Neibo[i], iter)<Neibo[i].g)
				{
					AstarMap[Neibo[i].i][Neibo[i].j].father = &AstarMap[iter.i][iter.j];
					AstarMap[Neibo[i].i][Neibo[i].j].g = AstarMap[iter.i][iter.j].g + increment(Neibo[i]);
					AstarMap[Neibo[i].i][Neibo[i].j].f = AstarMap[Neibo[i].i][Neibo[i].j].g + AstarMap[Neibo[i].i][Neibo[i].j].h;

					outOpenList2(Neibo[i],OpenList);
					putInOpenList(Neibo[i],OpenList);
				}
			}
			//如果这个邻居节点不在开启列表中
			else
			{
				AstarMap[Neibo[i].i][Neibo[i].j].father = Neibo[i].father = &AstarMap[iter.i][iter.j];
				AstarMap[Neibo[i].i][Neibo[i].j].g = AstarMap[iter.i][iter.j].g + increment(Neibo[i]);
				AstarMap[Neibo[i].i][Neibo[i].j].f = AstarMap[Neibo[i].i][Neibo[i].j].g + AstarMap[Neibo[i].i][Neibo[i].j].h;

				Neibo[i] = AstarMap[Neibo[i].i][Neibo[i].j];
				putInOpenList(Neibo[i],OpenList);
			}
		}
	}

	arrange(AstarMap[ender.i][ender.j], Astack, AstarMap);
	
	vector<vector<int>> finalMat;

	for (int i=0;i<AstackCount;++i)
	{
		vector<int> t2;

		t2.push_back(Astack[i].i);
		t2.push_back(Astack[i].j);

		finalMat.push_back(t2);
	}

	return finalMat;
}