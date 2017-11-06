#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <queue>
using namespace std;

//********************************************************************************************************************
// Class City is to store city information, including city name and x,y-coordinates
//********************************************************************************************************************
class City {
public:
    string name;
    int x;
    int y;
    City(string name, int x, int y) { this->name = name; this->x = x; this->y = y; };
};
