#include <unistd.h>
#include "fcntl.h"
#include <iostream>
#include <fstream>
#include "gStatistics.h"
#include "getRandom.h"
int _main_original(unsigned char  _a_rs);
void _runMainOriginalFunctionAndPrintInput(std::ofstream& xml, unsigned char  a)
{
xml<<"<i"<<g_statistics::runningCount<<">"<<std::endl;
xml<< "<a>"<<a<< "<a>" <<std::endl;
xml<<"</i"<<+g_statistics::runningCount<<">"<<std::endl;
_main_original(a);
;
}

unsigned long g_statistics::maxLoopBound = 2048;
std::map< g_statistics::loopInfo, std::pair<std::list<g_statistics::run>,g_statistics::loopType> > g_statistics::loopBoundMap;
int g_statistics::runningCount = 0;
int main(){
std::ofstream out ("./out");
std::ofstream xml ("./xml");
xml<<"<RandomSimulation>"<<std::endl;
xml<<"<inputs>"<<std::endl;
for(int i=0;i<20;i++){
++(g_statistics::runningCount);
_runMainOriginalFunctionAndPrintInput(xml, getRandomOfSpecificType<unsigned char >());
}
xml<<"</inputs>"<<std::endl;
g_statistics::printResults(out,xml);
xml<<"</RandomSimulation>"<<std::endl;
}