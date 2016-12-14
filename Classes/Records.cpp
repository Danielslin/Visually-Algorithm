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

//Origins and End Points
int startPointX = 0;
int startPointY = 0;
int endPointX = 0;
int endPointY = 0;

//Render List
vector<vector<int>> renderList;