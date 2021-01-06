#include <limits.h>
#include <string>
#include <iostream>
#include "gStatistics.h"
#define MAX(x,y) (x>y?x:y)
#define MIN(x,y) (x<y?x:y)
static std::string LOOP_NAME[] = {"WHILE","FOR","DO WHILE","FOREACH"};
g_statistics::loopType g_statistics::stringToEnum(std::string s)
{
	if(s == "while")
		return WHILE;
	else if (s == "for")
		return FOR;
	else if (s == "do")
		return DO_WHILE;
	else if (s == "FOREACH")
		return FOREACH;
	else
		std::cout << s;
}

bool g_statistics::loopInfo::operator<(const g_statistics::loopInfo& l)const
{
	if(this->fileName.compare(l.fileName) > 0)
		return true;
	else if(this->fileName.compare(l.fileName) == 0)
	{
		if(this->line < l.line)
			return true;
		else if(this->line == l.line && this->col < l.col)
			return true;
	}
	return false;
}

void g_statistics::addRunningLoop(std::string fileName,unsigned int loopLine,unsigned int loopColumn,std::string loop_type)
{
	 g_statistics::loopInfo l1 = {fileName,loopLine,loopColumn};
	if(loopBoundMap[l1].first.empty())
			loopBoundMap[l1].second = stringToEnum(loop_type);
	if(loopBoundMap[l1].first.empty() || loopBoundMap[l1].first.back().inputId != runningCount){
		run r = {std::list<unsigned long>(),runningCount};
		loopBoundMap[l1].first.push_back(r);
	}
	loopBoundMap[l1].first.back().loopUnrollingResults.push_back(0);
}
bool g_statistics::plusplus(std::string fileName,unsigned int loopLine,unsigned int loopColumn)
{
	loopInfo l1 = {fileName,loopLine,loopColumn};
	int currentLoopTimes = ++(loopBoundMap[l1].first.back()./*specific run*/loopUnrollingResults.back());
	return currentLoopTimes > maxLoopBound;
}

void g_statistics::printResults(std::ostream& os, std::ostream& xml)
{
	printLoopsOutputFile(os);
	printInputsXml(xml);
	printLoopsXml(xml);
}
void g_statistics::printInputsXml(std::ostream& xml)
{
		xml << "<inputs>" << std:: endl;
		/*int i=1;
		for (auto itInput=m_Inputs_Cont.begin(); itInput!=m_Inputs_Cont.end(); ++itInput){
			xml << "<input" <<i<<">" << std::endl;
			for(auto it=itInput->begin();it != itInput->end();it++ )
				xml <<"<" << it->name << " type = " << it->type << ">"<< it->value <<"<" << it->name << ">" << std::endl;
			xml << "</input" <<i<<">" << std::endl;
			i++;
		}*/
		xml << "</inputs>" << std::endl;
}
void g_statistics::printLoopsXml(std::ostream& xml)
{
		xml << "<loops>" << std:: endl;
		xml << "<numberOfLoops>"<<loopBoundMap.size()<<"</numberOfLoops>" << std::endl;
		for(auto itLoop = loopBoundMap.begin(); itLoop != loopBoundMap.end(); itLoop++){
			xml << "<loop type =\""<<LOOP_NAME[itLoop->second.second]<<"\" file=\"" << itLoop->first.fileName<<"\" row=\" "<< itLoop->first.line <<"\" col=\" "<< itLoop->first.col << "\" >";
			printCountMinMaxAvarage(xml,itLoop->second.first);
			printFullInformationForRunning(xml,itLoop->second.first);
			xml << "</loop>";
		}
		xml << "</loops>" << std::endl;
}
void g_statistics::printLoopsOutputFile(std::ostream& os)
{
	for(auto itLoop = loopBoundMap.begin(); itLoop != loopBoundMap.end(); itLoop++){
		unsigned int sum =0,count = 0,maxTimes = 0,minTimes = ULLONG_MAX;
		os << LOOP_NAME[itLoop->second.second]<<"(" <<itLoop->first.fileName<<","<<itLoop->first.line <<","<< itLoop->first.col << ")" <<std::endl;
		for(auto itRunning = itLoop->second.first.begin(); itRunning != itLoop->second.first.end();itRunning++){
			for(auto itLoopCalling = itRunning->loopUnrollingResults.begin(); itLoopCalling != itRunning->loopUnrollingResults.end(); itLoopCalling++,++count){
				sum+= *itLoopCalling;
				maxTimes = MAX(*itLoopCalling,maxTimes);
				minTimes = MIN(*itLoopCalling,minTimes);
			}
		}
		os << "num of runs: " <<count << std::endl;
		os << "max loop unrolling: " << maxTimes<< std::endl;
		os << "min loop unrolling:" <<maxTimes << std::endl;
		os << "avarage  unrolling:" <<sum/count << std::endl;
	}


}
void g_statistics::printCountMinMaxAvarage(std::ostream& xml,std::list<run> loop)
{
	unsigned int sum =0,count = 0,maxTimes = 0,minTimes = ULLONG_MAX;
	for(auto itRunning = loop.begin(); itRunning != loop.end();itRunning++){
		for(auto itLoopCalling = itRunning->loopUnrollingResults.begin(); itLoopCalling != itRunning->loopUnrollingResults.end(); itLoopCalling++,++count){
			sum+= *itLoopCalling;
			maxTimes = MAX(*itLoopCalling,maxTimes);
			minTimes = MIN(*itLoopCalling,minTimes);
		}
	}
	xml << "<count>" <<count<<"</count>" << std::endl;
	xml << "<min>" <<minTimes<<"</min>" << std::endl;
	xml << "<max>" <<maxTimes<<"</max>" << std::endl;
	xml << "<avarage>" <<sum/count<<"</avarage>" << std::endl;

}
void g_statistics::printFullInformationForRunning(std::ostream& xml,std::list<run> loop)
{
	unsigned int i=1;
	for(auto itRunning = loop.begin(); itRunning != loop.end();itRunning++,i++){
		unsigned int sum =0,count = 0,maxTimes = 0,minTimes = UINT_MAX;
		xml << "<run"<<i<<">"<< std::endl;
		xml << "<input>i"<<itRunning->inputId<<"</input>"<< std::endl;
		xml << "<result>";
		for(auto itLoopCalling = itRunning->loopUnrollingResults.begin(); itLoopCalling != itRunning->loopUnrollingResults.end(); itLoopCalling++,++count){
			sum+= *itLoopCalling;
			maxTimes = MAX(*itLoopCalling,maxTimes);
			minTimes = MIN(*itLoopCalling,minTimes);
			xml << *itLoopCalling << " ";
		}
		xml << "/<result>"<<std::endl;
		xml << "<count>" <<count<<"</count>" << std::endl;
		xml << "<min>" <<minTimes<<"</min>" << std::endl;
		xml << "<max>" <<maxTimes<<"</max>" << std::endl;
		xml << "<avarage>" <<sum/count<<"</avarage>" << std::endl;
		xml << "</run"<<i<<">"<< std::endl;
	}
	

}


