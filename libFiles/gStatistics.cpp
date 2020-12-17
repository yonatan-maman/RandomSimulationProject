
#include <list>
#include <map>
#include <utility>
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
int g_statistics::addRunningLoop(std::string fileName,unsigned int line,unsigned int column)
{
	//std::cout << "addRunningLoop - start " << std::endl;
	//std::cout << "addRunningLoop - mid1" << std::endl;
	if(loopBoundMap[{fileName,{line,column}}].empty()){
		std::list<unsigned long> s;
		loopBoundMap[{fileName,{line,column}}].push_back(s);
	}
	//std::cout << "addRunningLoop - mid2 " << std::endl;
	(loopBoundMap[{fileName,{line,column}}]).back().push_back(0);
	//std::cout << "addRunningLoop - end " << std::endl;
}
bool g_statistics::plusplus(std::string fileName,unsigned int line,unsigned int column)
{
	//std::cout << "plusplus - start" << std::endl;
	//std::cout << "plusplus - mid" << std::endl;
	int current = ++(loopBoundMap[{fileName,{line,column}}].back().back());
	return current > maxLoopBound;
	//std::cout << "plusplus - end" << std::endl;
}
void g_statistics::nextRunning()
{
	//std::cout << "nextRunning start" << std::endl;
	//std::cout << "addRunningLoop - mid " << std::endl;
	for (auto it=loopBoundMap.begin(); it!=loopBoundMap.end(); ++it)
	{
		std::list<unsigned long> s;
		it->second.push_back(s);
	}
	//std::cout << "addRunningLoop - end " << std::endl;
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



