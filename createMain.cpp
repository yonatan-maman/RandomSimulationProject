#include <string>
#include <iostream>
#include <fstream>
#include <algorithm>
#include "assert.h"
#include "string.h"

static inline std::string removeWhiteSpaces(std::string s)
{
	auto start = s.begin();
    for (;start != s.end(); ++start)
        if (!isspace(*start))
			break;
	auto end = s.end();
    for (;end != s.begin(); --end)
        if (!isspace(*end))
			break;
	
    return std::string(start, end);
}






inline void createRunMainOriginalFunction(std::string& function_call_main_original , std::string& inputs,std::string& runMainOriginalFunctionDefinition,std::string& function_call_run_main)
{
	runMainOriginalFunctionDefinition = "void _runMainOriginalFunctionAndPrintInput(std::ofstream& xml";
	std::string runMainOriginalFunctionDefinitionBody = "xml<<\"<i\"<<g_statistics::runningCount<<\">\"<<std::endl;\nxml"; 
	function_call_run_main = "_runMainOriginalFunctionAndPrintInput(xml";

	size_t IndexEndInputs = inputs.find("\n");
	size_t nextIndex = inputs.find("$",0);
	size_t previousIndex = 0;
	
	
	while(nextIndex < IndexEndInputs){

		size_t separatorTypeAndVariableName = inputs.find("-",previousIndex);
		std::string type (inputs.substr(previousIndex/*starting point */ ,separatorTypeAndVariableName - previousIndex /* size of type */));
		std::string variableName (inputs.substr(separatorTypeAndVariableName+1 /*starting point */,nextIndex-separatorTypeAndVariableName -1 /* size of type */)); 
		function_call_run_main += ", getRandomOfSpecificType<" + type + ">()";
		runMainOriginalFunctionDefinition += ", " + removeWhiteSpaces(type) +" "+removeWhiteSpaces(variableName);
		runMainOriginalFunctionDefinitionBody += "<< \"<" + removeWhiteSpaces(variableName)+">\"<<"+removeWhiteSpaces(variableName)+"<< \"</" + removeWhiteSpaces(variableName)+ ">\" <<std::endl";
		
		previousIndex = nextIndex + 1;
		nextIndex = inputs.find("$",nextIndex + 1);
	}
	runMainOriginalFunctionDefinitionBody += ";\n";
	runMainOriginalFunctionDefinitionBody += "xml<<\"</i\"<<+g_statistics::runningCount<<\">\"<<std::endl;\n"+function_call_main_original+"\n"; 
	function_call_run_main += ");";
	runMainOriginalFunctionDefinition += ")\n{\n" + runMainOriginalFunctionDefinitionBody + ";" + "\n}\n";
}







inline void createDefinitionAndFunctionCallAndDeclarition(std::string& function_call,std::string& definition, std::string& declarition , std::string& inputs)
{
	function_call += "_main_original(";
	definition += "_main_original(";
	
	size_t IndexEndInputs = inputs.find("\n");
	size_t nextIndex = inputs.find("$",0);
	size_t previousIndex = 0;
	
	
	while(nextIndex < IndexEndInputs){

		size_t separatorTypeAndVariableName = inputs.find("-",previousIndex);
		std::string type (inputs.substr(previousIndex/*starting point */ ,separatorTypeAndVariableName - previousIndex /* size of type */));
		std::string variableName (inputs.substr(separatorTypeAndVariableName+1 /*starting point */,nextIndex-separatorTypeAndVariableName -1 /* size of type */)); //we don't have to save it

		if(previousIndex != 0){
			function_call +=  ", ";
			definition += ", ";
		}
		function_call += removeWhiteSpaces(variableName);
		definition += removeWhiteSpaces(type) +" "+"_"+removeWhiteSpaces(variableName) +"_rs";
		previousIndex = nextIndex + 1;
		nextIndex = inputs.find("$",nextIndex + 1);
	}
	definition += ")";
	function_call += ");";
	declarition = "int " + definition +";"; 
}
inline void writeIncludesInNewMainFile(std::ofstream& newMainFile)
{
	newMainFile << "#include <unistd.h>"<< std::endl;
	newMainFile << "#include \"fcntl.h\"" << std::endl;
	newMainFile << "#include <iostream>"<< std::endl;
	newMainFile << "#include <fstream>"<< std::endl;
	newMainFile << "#include \"gStatistics.h\""<< std::endl;
	newMainFile << "#include \"getRandom.h\"" << std::endl;
}

inline void UpdateOriginalMainFile(std::ifstream& inputOriginalMainFile, std::ofstream& outputOriginalMainFile,std::string& definition)
{
	std::string currentLine;
	while(!inputOriginalMainFile.eof())
	{
		std::getline (inputOriginalMainFile,currentLine);
		if(currentLine.find("_main_original") != std::string::npos)
			outputOriginalMainFile << currentLine.replace (currentLine.find("_main_original()"),16/*size of _main_original()*/,definition) <<std::endl;
		else
			outputOriginalMainFile <<currentLine <<std::endl;
	}
}

inline void writeNewMainFile(char* maxLoopBound,std::ofstream& newMainFile,char *numRepetitions,std::string& functionCall)
{
	newMainFile << "unsigned long g_statistics::maxLoopBound = ";
	if(!strcmp(maxLoopBound,"0")) //0 instead of - check this line 
		newMainFile << 2048/*default value*/;
	else
		newMainFile << maxLoopBound; 
	newMainFile << ";" << std::endl;
	newMainFile << "std::map< g_statistics::loopInfo, std::pair<std::list<g_statistics::run>,g_statistics::loopType> > g_statistics::loopBoundMap;" << std::endl;
	newMainFile << "int g_statistics::runningCount = 0;" << std::endl;
	newMainFile << "int main(){" << std::endl;
	newMainFile << "std::ofstream out (\"./out.txt\");" << std:: endl;
	newMainFile << "std::ofstream xml (\"./out.xml\");" << std:: endl;
	newMainFile << "xml<<\"<RandomSimulation>\"<<std::endl;" << std:: endl;
	newMainFile << "xml<<\"<inputs>\"<<std::endl;" << std:: endl;
	newMainFile << "for(int i=0;i<"<<numRepetitions<<";i++){" << std::endl;
	newMainFile << "++(g_statistics::runningCount);" << std::endl;
	newMainFile << functionCall <<std::endl;
	newMainFile << "}"<< std::endl;
	newMainFile << "xml<<\"</inputs>\"<<std::endl;" << std:: endl;
	newMainFile << "g_statistics::printResults(out,xml);"<< std::endl;
	newMainFile << "xml<<\"</RandomSimulation>\"<<std::endl;" << std:: endl;
	newMainFile << "}" ;
}
int main (int argc, char **argv)
{
	//opening files
	std::ifstream inputsFile("./files/temp/tempInputs");
	std::string inputs = std::string(std::istreambuf_iterator<char>(inputsFile), std::istreambuf_iterator<char>());
	std::string originalMainFileName =  inputs.substr(inputs.find("\n",0)+1,inputs.find_last_of("-")-1 - inputs.find("\n",0));
	std::string c2rtlEmptyName =  originalMainFileName.substr(0,originalMainFileName.find_last_of("/")+1) + "c2rtl.h";
	std::cout <<originalMainFileName<<"-"<<c2rtlEmptyName;
	std::ofstream c2rtlEmptyFile("./files/"+c2rtlEmptyName);
	c2rtlEmptyFile << "////emptyFile" <<std::endl;
	c2rtlEmptyFile.close();
	std::rename(("./files/" + originalMainFileName).c_str(),("./files/"+ originalMainFileName +".temp").c_str());
	std::ifstream inputOriginalMainFile("./files/"+ originalMainFileName+".temp");
    std::ofstream outputOriginalMainFile("./files/"+ originalMainFileName);
	std::ofstream newMainFile ("./files/libFiles/main.cpp");
	
	writeIncludesInNewMainFile(newMainFile);
	
	std::string functionCall,definition,declarition,runMainOriginalFunctionDefinition,function_call_run_main;
	
	createDefinitionAndFunctionCallAndDeclarition(functionCall,definition,declarition,inputs);
	createRunMainOriginalFunction(functionCall,inputs,runMainOriginalFunctionDefinition,function_call_run_main);
	
	newMainFile << declarition <<std::endl;
		
	newMainFile << runMainOriginalFunctionDefinition << std::endl;
	
	
	assert(argc >= 3); // must have at least two parameters 

	writeNewMainFile(argv[1],newMainFile,argv[2],function_call_run_main);	
	
	//update the new original main file
	UpdateOriginalMainFile(inputOriginalMainFile, outputOriginalMainFile, definition);

	//closing the files
	inputOriginalMainFile.close();
	newMainFile.close();
	outputOriginalMainFile.close();
	
	//remove the old originalMain File
	remove (("./files/"+originalMainFileName+".temp").c_str());
	
}
