#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <regex>
#include <thread>
#include <vector>
#include <mutex>

struct OutputData
{
	std::string file_path;
	int string_empty_total_value = 0;
	int string_comment_total_value = 0;
	int string_code_total_value = 0;
	OutputData(std::string file_path_, int empty, int comment, int code) : file_path(file_path_), string_empty_total_value(empty), string_comment_total_value(comment), string_code_total_value(code) {};
	void PrintOutputData()
	{
		std::cout << "\nPath : " << file_path << "\nEmpty : " << string_empty_total_value << "\nComment : " << string_comment_total_value << "\nCode : " << string_code_total_value << "\n\n";
	}
};



class Threadpool
{
	std::vector<std::thread> pool;
	std::vector<std::string> queue;
	std::mutex queue_mutex;
	std::mutex write_mutex;
	std::vector<OutputData> result;
	/* DEBUG purposes
	std::atomic<int> string_empty_total_value = 0;
	std::atomic<int> string_comment_total_value = 0;
	std::atomic<int> string_code_total_value = 0;
	*/
	std::regex comment_regex = { '(','[','^','"',']','|','(','\\','"','.','*','\\','"',')',')','*','(','/','/',')','+','.','*' };
	std::regex long_comment_start_regex = { '[','^','"','(','/','/',')',']','*','(','\\','"','.','*','\\','"',')','*','(','/','\\','*',')','+','.','*' };
	std::regex long_comment_end_regex = { '\\','*','/' };
	std::regex code_regex = { '(','^','\\','s','*','[','^','(','/','\\','*',')','(','/','/',')','(','\\','s',')',']','+','.','*',')' };
	std::regex empty_regex = { '\\','s','*' };

	void InfiniteLoop();
	void Work(std::string file_path);

public:
	int threads_num;

	Threadpool(std::vector<std::string> queue_);
	void Calculate();
	void PrintData();
	int GetSize();
};

