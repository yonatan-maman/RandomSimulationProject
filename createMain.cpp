
#include <string>
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <limits.h>
#include "assert.h"
#define SPACE 32

int main (int argc, char **argv)
{
	std::ifstream inputs("./files/temp/tempInputs");
	std::string s = std::string(std::istreambuf_iterator<char>(inputs), std::istreambuf_iterator<char>());
	std::cout << s << std::endl;
	std::string mainFile =  s.substr(s.find("\n",0)+1,s.find_last_of("-")-1 - s.find("\n",0));
	std::cout <<"mainFile -"<< mainFile << std::endl;
	std::rename(("./files/"+mainFile).c_str(),("./files/"+mainFile+".temp").c_str());
	std::ifstream filein("./files/"+mainFile+".temp");
    std::ofstream fileout("./files/"+mainFile);
	std::ofstream mainCode ("./files/_main.cpp");
	mainCode << "#include <unistd.h>"<< std::endl;
	mainCode << "#include \"fcntl.h\"" << std::endl;
	mainCode << "#include <iostream>"<< std::endl;
	mainCode << "#include \"libFiles/gStatistics.h\""<< std::endl;
	mainCode << "#include \"libFiles/getRandom.h\"" << std::endl;
	mainCode << "#define N 4" << std::endl;
	std::cout << 1 <<std::endl;
	std::string function_call,definition;
	std::cout << s<<std::endl;
	size_t length = s.find("\n");
	size_t index = s.find("$",0);
		std::cout <<"index -" <<index<<std::endl;
	std::cout <<"length" << length<<std::endl;
	size_t previous = 0;
	mainCode << "int _main_original(";
	function_call += "_main_original(";
	definition += "_main_original(";
		std::cout << 2<<std::endl;
	while(index < length && index != std::string::npos){
			std::cout << index<<std::endl;
		size_t mid = s.find("-",previous);
		std::string stype (s.substr(previous,mid-previous));
		std::string variable (s.substr(mid+1,index-mid -1)); //we don't have to save it
		std::cout <<"s " <<stype << "v " << variable;
		if(previous == 0){
			function_call += "getRandomOfSpecificType<" + stype + ">()";
			mainCode <<  stype <<" "<< variable;
			definition += stype + variable;
		}
		else{
			function_call +=  ", getRandomOfSpecificType<" + stype+">()";
			mainCode <<"," << stype << "" << variable;
			definition += ','+ stype + variable;
		}
		previous = index + 1;
		index = s.find("$",index + 1);
	}
	mainCode << ");" << std::endl;
	definition += ")";
	function_call += ");";
	std::string line;
	assert(argc >= 2);
	while(!filein.eof())
	{
		std::getline (filein,line);
		if(line.find("_main_original") != std::string::npos)
			fileout << line.replace (line.find("_main_original()"),16/*size of _main_original()*/,definition) <<std::endl;
		else
			fileout <<line <<std::endl;
	}
	mainCode << "unsigned long g_statistics::maxLoopBound = ";
	if(**(argv+2) != 'x')
		mainCode << *(argv+2);
	else 
		mainCode << ULONG_MAX;
	mainCode << ";" << std::endl;
	mainCode << "std::map<std::pair<std::string,std::pair<unsigned int,unsigned int>>,std::list<std::list<unsigned long>>> g_statistics::loopBoundMap;";
	mainCode << "int main(){" << std::endl;
	mainCode << "for(int i=0;i<"<<*(argv+1)<<";i++){" << std::endl;
	mainCode << "g_statistics::nextRunning();" << std::endl;
	mainCode << function_call <<std::endl;
	mainCode << "}"<< std::endl;
	mainCode << "int out = open(\"out\", O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IRGRP | S_IWGRP | S_IWUSR);" <<std::endl;
	mainCode << "dup2(out,fileno(stdout));" <<std::endl;
	mainCode << "g_statistics::printResults(std::cout);"<< std::endl;
	mainCode << "}" ;
	mainCode.close();
	filein.close();
	fileout.close();
	remove (("./files/"+mainFile+".temp").c_str());
}
