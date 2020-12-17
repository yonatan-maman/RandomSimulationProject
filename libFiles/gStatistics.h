#ifndef GSTATISTICS
#define GSTATISTICS
#include <list>
#include <map>


#define INPUT(type,id)

class g_statistics
{
	static  std::map<std::pair<std::string,std::pair<unsigned int,unsigned int>>,std::list<std::list<unsigned long>>> loopBoundMap;
	static unsigned long maxLoopBound;

	public:
	static int addRunningLoop(std::string fileName,unsigned int line,unsigned int column);
	static bool plusplus(std::string fileName,unsigned int line,unsigned int column);
	static void nextRunning();
	static void printResults(std::ostream& os);
};
#endif
