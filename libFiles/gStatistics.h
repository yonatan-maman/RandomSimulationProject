#ifndef GSTATISTICS
#define GSTATISTICS
#include <list>
#include <map>


#define C2RTL_INPUT(type,id) id = _## id ## _rs
#define C2RTL_OUTPUT(type,id) 

class g_statistics
{
	public:
	static  int runningCount;
	typedef struct 
	{
		std::string name;
		std::string type;
		std::string value;
	} input;
	class loopInfo 
	{
		public:
		std::string fileName;
		unsigned int line;
		unsigned int col;
		bool operator<(const loopInfo&)const;
	};
	typedef struct 
	{
		std::list<unsigned long> loopUnrollingResults;
		int inputId;
	} run;
	typedef enum {WHILE,FOR,DO_WHILE,FOREACH,NUM_OF_ELEMENTS} loopType;
	
	
	static void addRunningLoop(std::string fileName,unsigned int loopLine,unsigned int loopColumn,std::string loop_type);
	static bool plusplus(std::string fileName,unsigned int line,unsigned int column);
	static void nextRunning();
	static void printResults(std::ostream& os, std::ostream& xml);
	
	private:
	static loopType stringToEnum(std::string);
	static void printLoopsXml(std::ostream& xml);
	static void printCountMinMaxAvarage(std::ostream& xml,std::list<run> loop);
	static void printLoopsOutputFile(std::ostream& os);
	static void printFullInformationForRunning(std::ostream& xml,std::list<run> loop);
	static  std::map< loopInfo, std::pair<std::list<run>,loopType> > loopBoundMap;
	static unsigned long maxLoopBound;
};
#endif
