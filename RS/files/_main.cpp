#include <unistd.h>
#include "fcntl.h"
#include <iostream>
#include "libFiles/gStatistics.h"
#include "libFiles/getRandom.h"
#define N 4
int _main_original(char    x,long   y);
unsigned long g_statistics::maxLoopBound = 40;
std::map<std::pair<std::string,std::pair<unsigned int,unsigned int>>,std::list<std::list<unsigned long>>> g_statistics::loopBoundMap;int main(){
for(int i=0;i<200;i++){
g_statistics::nextRunning();
_main_original(getRandomOfSpecificType<char >(), getRandomOfSpecificType<long >());
}
int out = open("out", O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IRGRP | S_IWGRP | S_IWUSR);
dup2(out,fileno(stdout));
g_statistics::printResults(std::cout);
}