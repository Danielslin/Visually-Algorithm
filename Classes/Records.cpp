#include <vector>
using namespace std;

//Video Data
double visibleSizeW = 0.0;
double visibleSizeH = 0.0;

double originX = 0.0;
double originY = 0.0;

//Audio Data
int audioId = 0;
bool haveSound = true;

//Astar Map
vector<vector<int>> AstarMap;

//起点、终点坐标
int startPointX = 0;
int startPointY = 0;
int endPointX = 0;
int endPointY = 0;