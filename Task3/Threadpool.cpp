#include "Threadpool.h"

void Threadpool::InfiniteLoop()
{
	while (true)
	{
		std::string str;
		{
			//Locks for thread-safe access to queue
			std::lock_guard<std::mutex> lock(queue_mutex);
			if (!queue.empty())
			{
				str = queue.back();
				queue.pop_back();
			}
			else
			{
				break;
			}
		}
		Work(str);
	}
}

void Threadpool::Work(std::string file_path)
{
	std::string line;
	OutputData data(file_path);
	std::ifstream infile(file_path);
	if (infile)
	{
		//Shows if comment started(/*) and didn't end(*/)
		bool is_in_comment = false;
		while (getline(infile, line))
		{
			//Erases whitespaces
			line.erase(std::remove(line.begin(), line.end(), '	'), line.end());
			line.erase(std::remove(line.begin(), line.end(), ' '), line.end());
			//Updates comment status and line type count
			is_in_comment = LineCheck(line, is_in_comment, data);
		}
	}
	{
		//Locks for thread-safe access to result
		std::lock_guard<std::mutex> lock(write_mutex);
		result.push_back(data);
	}
}

bool Threadpool::LineCheck(std::string& line, const bool is_in_comment, OutputData& data)
{
	bool result = is_in_comment;
	data.total_lines++;
	if (is_in_comment)
	{
		if (line.find("*/") != -1)
		{
			result = false;
		}
		data.string_comment_total_value++;
	}
	else if (line.length() == 0)
	{
		data.string_empty_total_value++;
	}
	else if (line[0] == '/' && line[1] == '/')
	{
		data.string_comment_total_value++;
	}
	else if (line[0] == '/' && line[1] == '*')
	{
		data.string_comment_total_value++;
		//Check if long comment ends on same line
		result = !(line.find("*/") != -1);
	}
	else
	{
		data.string_code_total_value++;
	}
	return result;
}

Threadpool::Threadpool(std::vector<std::string> queue_)
{
	queue = std::move(queue_);
}

void Threadpool::Calculate()
{
	//Take number of threads equal to maximum provided by system or file number
	uint32_t n = GetSize();
	if (std::thread::hardware_concurrency() < n)
	{
		int n = std::thread::hardware_concurrency();
	}
	//Creates threads
	for (int i = 0; i < n; ++i)
	{
		pool.push_back(std::thread(&Threadpool::InfiniteLoop, this));
	}
	//Waits for threads to finish
	for (int i = 0; i < n; ++i)
	{
		pool[i].join();
	}
	//Destroys threads
	pool.clear();
}

void Threadpool::PrintData()
{
	for (auto i_data = result.begin(); i_data != result.end(); i_data++)
	{
		i_data->PrintOutputData();
	}
	std::cout << "Number of processed files : " << result.size() << "\nTime of execution : ";
}

uint32_t Threadpool::GetSize()
{
	return queue.size();
}

void Threadpool::PrintToFile()
{
	std::ofstream result_file;
	result_file.open("result.txt");
	int processed_lines = 0;
	for (auto i_data = result.begin(); i_data != result.end(); i_data++)
	{
		result_file << "\nPath : " << i_data->file_path << "\nEmpty : " << i_data->string_empty_total_value << "\nComment : " << i_data->string_comment_total_value << "\nCode : " << i_data->string_code_total_value << "\n\n";
		processed_lines += i_data->total_lines;
	}
	result_file.close();
	std::cout << "Number of processed lines : " << processed_lines << "\nNumber of processed files : " << result.size() << "\nTime of execution : ";
}


