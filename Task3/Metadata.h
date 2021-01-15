#pragma once
#include <fstream>
#include <iostream>
#include <filesystem>
#include <vector>
#include <sstream>
#include <regex>

class Metadata
{
	std::string path = "C:\\Users\\Oleg\\source\\repos\\Task2";
	std::vector<std::string> ext = { ".c" ,".cpp" ,".h" ,".hpp" };

	int string_empty_value = 0;
	int string_comment_value = 0;
	int string_code_value = 0; 

	std::regex comment_regex = { '(','[','^','"',']','|','(','\\','"','.','*','\\','"',')',')','*','(','/','/',')','+','.','*' };
	std::regex long_comment_start_regex = { '[','^','"','(','/','/',')',']','*','(','\\','"','.','*','\\','"',')','*','(','/','\\','*',')','+','.','*' };
	std::regex long_comment_end_regex = { '\\','*','/' };
	std::regex code_regex = { '(','^','\\','s','*','[','^','(','/','\\','*',')','(','/','/',')','(','\\','s',')',']','+','.','*',')' };
	std::regex empty_regex = { '\\','s','*' };

	void StringOut(std::string str);
	
public:
	std::vector<std::string> files;

	Metadata();
	Metadata(std::string path_);
	void GetExt();
	void ClearExt();
	void AddExt();
	void SearchForFiles();
	void PrintFiles();
	void ReadFiles();

};

