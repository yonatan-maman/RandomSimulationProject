#include <unistd.h>
#include "fcntl.h"
#include <iostream>
#include <fstream>
#include "gStatistics.h"
#include "getRandom.h"
int _main_original(unsigned char  _a_rs);
unsigned long g_statistics::maxLoopBound = 2048;
std::map< g_statistics::loopInfo, std::pair<std::list<g_statistics::run>,g_statistics::loopType> > g_statistics::loopBoundMap;
int g_statistics::runningCount = 0;
int main(){
for(int i=0;i<20;i++){
++(g_statistics::runningCount);
_main_original(getRandomOfSpecificType<unsigned char >());
}
std::ofstream out ("./out");
std::ofstream xml ("./xml");
g_statistics::printResults(out,xml);
}