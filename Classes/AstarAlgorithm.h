#ifndef _ASTAR_ALGORITHM_H_
#define _ASTAR_ALGORITHM_H_

#include <vector>

//A Base Struct of Map
struct baseNode
{
	int i = 0;
	int j = 0;
	int weigt = 0;

	double f = 0;
    double g = 0;
	double h = 0;

	baseNode *father = nullptr;

	void Clear() { i = 0; j = 0; weigt = 0; f = 0; g = 0; h = 0; }

	bool operator<(baseNode &temp) { if (f < temp.f) { return true; } else { return false; } }
};

//External Interface of Astar
std::vector<std::vector<int>> Astar(int startPointColumn, int startPointLine, int endPointColumn, int endPointLine, const std::vector<std::vector<int>> &firstMap);

//Convert Original Map to Astar Map
void inputMap(const std::vector<std::vector<int>> &firstMap);

//Astar Kernel
std::vector<std::vector<int>> AstarKernel();

/////////Openlist Functions////////////////////////
//Put A Base Node into Openlist
void putInOpenList(baseNode &inList);

//Delete First Basenode From Openlist
void outOpenList();

//Delete Appointed Basenode From Openlist
void outOpenList2(baseNode &iter);

//Check if The Neighbor Basenode is In Openlist 
bool isInOpenList(baseNode &neibo);
////////////////////////////////////////////////////

//Put A Base Node into Closelist
void putInCloseList(baseNode &inList);

//Check if The Appointed Basenode is In Closelist ,Bisearch is using here
bool isInCloseList(baseNode &temp);

//For each point on the road, analyze it's up to eight neighbors, and join the list of neighbors
int addNeibo(baseNode &iter);

//Heuristic Function in A Star£¬It Can Result Archimedes Distant Between This Point and End Point
double Archimedes(double i, double j);

//Find the distance between the current point and the father node
int increment(baseNode &nthis);

//Find the G value of this point with the current point as the parent node
int NewG(baseNode &nthis, baseNode &father);

//To organize a node A* algorithm to Astack in reverse order
void arrange(baseNode &iter);

#endif