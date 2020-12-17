#include "../libFiles/gStatistics.h"
#include <iostream>
int _main_original(char   x,long   y)
{
INPUT(char,x)
INPUT(long , y);

std::cout << y << "  "<< int(x) <<std::endl;
y = 10000000000;
g_statistics::addRunningLoop(__FILE__ ,9,2);
while((--y) != 0)
{
if(g_statistics::plusplus(__FILE__ ,9,2))
break;

}
std::cout <<std::endl;
}
