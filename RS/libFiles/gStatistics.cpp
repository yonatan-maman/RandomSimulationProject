
#include <string>
#include <ostream>
#include "gStatistics.h"
#define MAX(x,y) (x>y?x:y)



void printListReturnMaxAndAvarage(std::list<unsigned long>& s,unsigned long int &max,std::ostream& os,unsigned long maxLoopBound)
{
	unsigned int sum =0,count = 0,maxTimes = 0;
	for(auto it = s.begin(); it != s.end();it++){
		count++;
		sum+= *it;
		if(sum > maxLoopBound)
			os << ">"<< maxLoopBound;
		else
			os << *it << " ";
		maxTimes = MAX(*it,maxTimes);
	}
	max = maxTimes;
	if(count > 1)
		os << " -max " << maxTimes <<" -times "<<count<<"- avarage " << sum/count;
	os << std::endl;
	
}
int g_statistics::addRunningLoop(std::string fileName,unsigned int loopLine,unsigned int loopColumn)
{
	if(loopBoundMap[{fileName,{loopLine,loopColumn}}].empty())
		loopBoundMap[{fileName,{loopLine,loopColumn}}].push_back(std::list<unsigned long>());
	loopBoundMap[{fileName,{loopLine,loopColumn}}].back().push_back(0);
}
bool g_statistics::plusplus(std::string fileName,unsigned int loopLine,unsigned int loopColumn)
{
	int currentLoopTimes = ++(loopBoundMap[{fileName,{loopLine,loopColumn}}].back().back());
	return currentLoopTimes > maxLoopBound;
}
void g_statistics::nextRunning()
{
	for (auto it=loopBoundMap.begin(); it!=loopBoundMap.end(); ++it)
		it->second.push_back(std::list<unsigned long>());
}

void g_statistics::printResults(std::ostream& os)
{

	for (auto it=loopBoundMap.begin(); it!=loopBoundMap.end(); ++it)
	{
		unsigned long max_it = 0,max = 0;
		os << "------------\"" << it->first.first <<"\"--------------line "<< it->first.second.first << "\" ------------- column " << it->first.second.second << std::endl;
		for(auto itList = it->second.begin(); itList != it->second.end(); ++itList){
			printListReturnMaxAndAvarage(*itList,max,os,maxLoopBound);
			max_it = MAX(max,max_it);
		}
		os <<std::endl << "max times  -" << max_it<<std::endl;
		os << "------------\"" << it->first.first <<"\"--------------line "<< it->first.second.first << "\" ------------- column " << it->first.second.second << std::endl;
		os <<std::endl <<std::endl;

	}
}



