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

//Astar Map,A integral Matrix
vector<vector<int>> aStarMap;

//Origins and End Points
int startPointLine = 0;
int startPointColumn = 0;
int endPointLine = 0;
int endPointColumn = 0;

//Render List
vector<vector<int>> renderList;