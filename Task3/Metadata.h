#pragma once
#include <fstream>
#include <iostream>
#include <filesystem>
#include <vector>
#include <sstream>

class Metadata
{
	/*Path to user defined directory*/
	std::string path;
	/*Default list of extensions*/
	std::vector<std::string> ext = { ".c" ,".cpp" ,".h" ,".hpp" };

public:
	/*Files with needed extensions*/
	std::vector<std::string> files;

	Metadata(std::string path_);
	/*Outputs all extensions*/
	void GetExt();
	/*Deletes all extensions*/
	void ClearExt();
	/*Add extentions using console*/
	void AddExt();
	/*Add extentions using arguments*/
	void AddExtAsArg(char* arg);
	/*Search for files with needed extensions*/
	void SearchForFiles();
	/*Outputs all files with needed extensions*/
	void PrintFiles();

};

