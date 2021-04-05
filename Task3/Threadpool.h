#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <thread>
#include <vector>
#include <mutex>

/*Structure with all output info*/
struct OutputData
{
	std::string file_path;
	int total_lines = 0;
	int string_empty_total_value = 0;
	int string_comment_total_value = 0;
	int string_code_total_value = 0;
	OutputData(std::string file_path_, int empty = 0, int comment = 0, int code = 0, int total = 0) : file_path(file_path_), string_empty_total_value(empty), string_comment_total_value(comment), string_code_total_value(code), total_lines(total) {};
	void PrintOutputData()
	{
		std::cout << "\nPath : " << file_path << "\nEmpty : " << string_empty_total_value << "\nComment : " << string_comment_total_value << "\nCode : " << string_code_total_value << "\n\n";
	}
};



class Threadpool
{
	/*Pool of working threads*/
	std::vector<std::thread> pool;
	/*Vector of unprocessed files pathes*/
	std::vector<std::string> queue;
	/*Mutex for poping from queue*/
	std::mutex queue_mutex;
	/*Mutex for pushing to result*/
	std::mutex write_mutex;
	std::vector<OutputData> result;

	//Function for threads to wait for next file
	void InfiniteLoop();
	//Functions for threads to read line from file
	void Work(std::string file_path);
	//Function to check category of line
	bool LineCheck(std::string& line, const bool is_in_comment, OutputData& data);

public:

	Threadpool(std::vector<std::string> queue_);
	/*Calculates quantity of each line type*/
	void Calculate();
	/*Outputs vector result*/
	void PrintData();
	/*Returns number of files*/
	uint32_t GetSize();
	/*Save result in file*/
	void PrintToFile();
};

