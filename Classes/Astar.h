#ifndef _ASTAR_H_
#define _ASTAR_H_

#include <cmath>
#include <vector>

//一个比较基础的结构体，用于和地图联动
struct baseNode
{
	int i=0;
	int j=0;
	int weigt=0;

	int f=0;
	int g=0;
	int h=0;

	struct baseNode *father=nullptr;
};

std::vector<std::vector<baseNode>> inputMap(int sizeX, int sizeY, const std::vector<std::vector<int>> &firstMap);
std::vector<std::vector<int>> Astar(int startPointX, int startPointY, int endPointX, int endPointY, const std::vector<std::vector<int>> &firstMap);

void putInOpenList(baseNode &inList, std::vector<baseNode> &OpenList);

//取出开启列表（最小二叉堆）中最小的数
baseNode readTopOpenList(std::vector<baseNode> &OpenList);

//把一个元素加入关闭列表中，使关闭列表有序
void putInCloseList(baseNode &temp, std::vector<baseNode> &CloseList);

//把开启列表中的当前节点删除，使其符合最小二叉堆的性质
void outOpenList(std::vector<baseNode> &OpenList);

//（注：这个可以用很多方法实现，示例并非最优）
void outOpenList2(baseNode &iter, std::vector<baseNode> &OpenList);

//对于一路上的每个点，分析它的最多八个邻居，并加入邻居列表
void addNeibo(baseNode &iter, baseNode(&Neibo)[8], std::vector<std::vector<baseNode>> &AstarMap);

//查看临近格是否在开启列表中的函数
int isInOpenList(baseNode &neibo, std::vector<baseNode> &OpenList);

//查看指定的temp在不在关闭列表中的函数，使用了折半查找
int isInCloseList(baseNode &temp, std::vector<baseNode> &CloseList);

//A*中的启发式函数，用于求指定位置和终点之间的曼哈顿距离
int manhatten(int i, int j);

//求当前点与父亲节点的距离
int increment(baseNode &nthis);

//求出用当前点作为父节点时这个点的G值
int NewG(baseNode &nthis, baseNode &father);

//把A*算法的节点按倒序整理到Astack里面
void arrange(baseNode &iter, std::vector<baseNode> &Astack, std::vector<std::vector<baseNode>> &AstarMap);

//Astar的本体
std::vector<std::vector<int>> AstarO(std::vector<baseNode> &Astack, baseNode(&Neibo)[8], std::vector<std::vector<baseNode>> &AstarMap, std::vector<baseNode> &OpenList, std::vector<baseNode> &CloseList);

#endif