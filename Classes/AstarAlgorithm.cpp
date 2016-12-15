#include <cmath>
#include <algorithm>
#include "AstarAlgorithm.h"

using namespace std;

//Render List
extern vector<vector<int>> renderList;

//Result List 
vector<baseNode> Astack;

//Open List
vector<baseNode> OpenList;

//Close List
vector<baseNode> CloseList;

//Neibo Nodes
baseNode Neibo[8];

//Map Size
int MapSizeW = 0;
int MapSizeH = 0;

//Astar Map
vector<vector<baseNode>> AstarMap;

//Origin and End Point
int startCol = 0, startLine = 0, endCol = 0, endLine = 0;

//We Can Use FLAG to Check if End Point is Already in Close List
bool FLAG = false;

vector<vector<int>> Astar(int startPointColumn, int startPointLine, int endPointColumn, int endPointLine, const vector<vector<int>> &firstMap)
{
	//Initialize Map
	auto be = firstMap.begin();

	MapSizeH = firstMap.size();
	MapSizeW = be->size();

	inputMap(firstMap);

	//Initialize Origin and End Point
	startCol = startPointColumn; startLine = startPointLine; endCol = endPointColumn; endLine = endPointLine;

	//Run Astar Kernel
	auto Result = AstarKernel();

	Astack.clear(); OpenList.clear(); CloseList.clear(); AstarMap.clear();
	MapSizeW = 0; MapSizeH = 0; startCol = 0, startLine = 0, endCol = 0, endLine = 0;FLAG = false;

	for (int i=0;i<8;++i){ Neibo[i].Clear(); }

	return Result;
}

void inputMap(const vector<vector<int>> &firstMap)
{
	vector<baseNode> temp(MapSizeW);
	for (int i = 0; i<MapSizeH; ++i)
	{
		AstarMap.push_back(temp);
	}

	for (int i = 0; i<MapSizeH; ++i)
	{
		for (int j = 0; j<MapSizeW; ++j)
		{
			AstarMap[i][j].weigt = firstMap[i][j];

			AstarMap[i][j].i = i;
			AstarMap[i][j].j = j;
		}
	}
}

vector<vector<int>> AstarKernel()
{
	//Build A Iter,To Mark The Node that Has Smallest F Value Each Loop
	baseNode iter;

	//Set Iter Equel To Start Point at First
	iter.i = startLine;
	iter.j = startCol;
	iter.weigt = AstarMap[startLine][startCol].weigt;

	//Start Ponit Don't Have A Father Node, and It is the only one that G Value is 0
	iter.g = 0;
	iter.h = Archimedes(iter.i, iter.j);
	iter.f = iter.g + iter.h;

	//Set End Point
	baseNode ender;

	ender.i = endLine;
	ender.j = endCol;

	//Put Start Point into Openlist
	putInOpenList(iter);

	//if Openlist is Empty Or End Point is already in Closelist,Astar Finish
	while(OpenList.size() != 0 && !FLAG)
	{
		//Remove one of the smallest F values in the open list, and set to ITER(current point)
		iter = OpenList[0];

		//Remove the minimum point from the open list
		outOpenList();

		//Record the current point on the closed list
		putInCloseList(iter);

		//Push Diamonds into Render List
		vector<int> RenderTemp(3, 0);
		RenderTemp[0] = iter.i;
		RenderTemp[1] = iter.j;
		RenderTemp[2] = 2;
		renderList.push_back(RenderTemp);

		int neibNum = addNeibo(iter);

		for (int i = 0; i < neibNum; ++i)
		{
			//If the neighbor node can not be passed, or the neighbor node is in the closed list, skip it
			if (Neibo[i].weigt == 0 || isInCloseList(Neibo[i]))
			{
			}
			//If the neighbor node is already in the open list
			else if (isInOpenList(Neibo[i]))
			{
				if (NewG(Neibo[i], iter)<Neibo[i].g)
				{
					AstarMap[Neibo[i].i][Neibo[i].j].father = &AstarMap[iter.i][iter.j];
					AstarMap[Neibo[i].i][Neibo[i].j].g = AstarMap[iter.i][iter.j].g + increment(Neibo[i]);
					AstarMap[Neibo[i].i][Neibo[i].j].f = AstarMap[Neibo[i].i][Neibo[i].j].g + AstarMap[Neibo[i].i][Neibo[i].j].h;

					outOpenList2(Neibo[i]);
					putInOpenList(Neibo[i]);
				}
			}
			//If the neighbor node is not in the open list
			else
			{
				AstarMap[Neibo[i].i][Neibo[i].j].father = Neibo[i].father = &AstarMap[iter.i][iter.j];
				AstarMap[Neibo[i].i][Neibo[i].j].g = AstarMap[iter.i][iter.j].g + increment(Neibo[i]);
				AstarMap[Neibo[i].i][Neibo[i].j].f = AstarMap[Neibo[i].i][Neibo[i].j].g + AstarMap[Neibo[i].i][Neibo[i].j].h;

				Neibo[i] = AstarMap[Neibo[i].i][Neibo[i].j];
				putInOpenList(Neibo[i]);
			}
		}
	}

	arrange(AstarMap[ender.i][ender.j]);

	vector<vector<int>> finalMat;

	for (int i = Astack.size()-1; i>=0; --i)
	{
		vector<int> t2;

		t2.push_back(Astack[i].i);
		t2.push_back(Astack[i].j);

		finalMat.push_back(t2);
	}

	return finalMat;
}

double Archimedes(double i, double j)
{
	double A = abs(endLine - i);
	double B = abs(endCol - j);

	double c = sqrt(A*A + B*B);
	if (!c){ FLAG = true; }

	return c * 10;
}

void putInOpenList(baseNode &inList)
{
	//Push Diamonds into Render List
	vector<int> RenderTemp(3,0);
	RenderTemp[0] = inList.i;
	RenderTemp[1] = inList.j;
	RenderTemp[2] = 1;
	renderList.push_back(RenderTemp);

	OpenList.push_back(inList);
	sort(OpenList.begin(),OpenList.end());
}

void outOpenList()
{
	OpenList.erase(OpenList.begin());
}

int addNeibo(baseNode &iter)
{
	int neibNum = 0;

	for (int i = iter.i - 1; i <= iter.i + 1; ++i)
	{
		for (int j = iter.j - 1; j <= iter.j + 1; ++j)
		{
			if (i >= 0 && i <= MapSizeH - 1 && j >= 0 && j <= MapSizeW - 1)
			{
				if (i == iter.i&&j == iter.j)
				{
				}
				else
				{
					AstarMap[i][j].h = Archimedes(i, j);

					Neibo[neibNum] = AstarMap[i][j];
					++neibNum;
				}
			}
		}
	}

	return neibNum;
}

void putInCloseList(baseNode & inList)
{
	//Push Diamonds into Render List
	vector<int> RenderTemp(3, 0);
	RenderTemp[0] = inList.i;
	RenderTemp[1] = inList.j;
	RenderTemp[2] = 2;
	renderList.push_back(RenderTemp);

	CloseList.push_back(inList);
	sort(CloseList.begin(), CloseList.end());
}

bool isInCloseList(baseNode &temp)
{
	for (auto &r:CloseList)
	{
		if (temp.i==r.i&&temp.j==r.j)
		{
			return true;
		}
	}
	return false;
}

bool isInOpenList(baseNode &neibo)
{
	for (auto &r : OpenList)
	{
		if (neibo.i == r.i&&neibo.j == r.j)
		{
			return true;
		}
	}
	return false;
}

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

void outOpenList2(baseNode &iter)
{
	for (auto i=OpenList.begin();i!=OpenList.end();++i)
	{
		if ((iter.i == i->i)&&(iter.j==i->j))
		{
			OpenList.erase(i);
			break;
		}
	}
}

void arrange(baseNode &iter)
{
	while (iter.father != nullptr)
	{
		Astack.push_back(iter);

		if (iter.i == startLine&&iter.j == startCol)
		{
			break;
		}

		iter = *iter.father;
	}
}
