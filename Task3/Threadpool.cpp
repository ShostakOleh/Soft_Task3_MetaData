#include "Threadpool.h"

void Threadpool::InfiniteLoop()
{
	while (true)
	{
		std::string str;
		{
			std::lock_guard<std::mutex> lock(queue_mutex);
			if (!queue.empty())
			{
				str = queue.back();
				queue.pop_back();
				// DEBUG purposes
				//std::cout << "\nNum = " << queue.size() << '\n';
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
	// DEBUG purposes
	//std::cout << "\nPath = " << file_path << '\n';
	std::string line;
	bool is_in_comment = false;
	bool comment_end = false;
	int string_empty_value = 0;
	int string_comment_value = 0;
	int string_code_value = 0;
	std::ifstream infile(file_path);
		if (infile)
		{
			std::smatch sm;
			while (getline(infile, line) )
			{
				comment_end = false;
				if (line.length() == 0 || std::regex_match(line, sm, empty_regex))
				{
					++string_empty_value;
					if (is_in_comment)
					{
						++string_comment_value;
					}
					continue;
				}
				if (is_in_comment)
				{
					++string_comment_value;
					if (std::regex_search(line, sm, long_comment_end_regex))
					{
						is_in_comment = false;
						line = sm.suffix();
						comment_end = true;
					}
					else
					{
						continue;
					}
				}

				if (std::regex_match(line, sm, code_regex))
				{
					++string_code_value;
				}

				if (std::regex_match(line, sm, long_comment_start_regex))
				{
					++string_comment_value;
					is_in_comment = true;
					if (std::regex_search(line, sm, long_comment_end_regex))
					{
						is_in_comment = false;
						line = sm.suffix();
					}
					else
					{
						continue;
					}
				}

				if (std::regex_match(line, sm, comment_regex))
				{
					++string_comment_value;
				}
				else if (comment_end && !std::regex_match(line, sm, empty_regex))
				{
					++string_code_value;
				}
			}
		}
		/* DEBUG purposes
		string_empty_total_value += string_empty_value;
		string_comment_total_value += string_comment_value;
		string_code_total_value += string_code_value;
		*/
		{
			OutputData data(file_path, string_empty_value, string_comment_value, string_code_value);
			std::lock_guard<std::mutex> lock(write_mutex);
			result.push_back(data);
		}
}

Threadpool::Threadpool(std::vector<std::string> queue_)
{
	queue = std::move(queue_);
	threads_num = std::thread::hardware_concurrency();
}

void Threadpool::Calculate()
{
	int n = GetSize();
	if (threads_num < n)
	{
	int n = threads_num;
	}
	
	for (int i = 0; i < n; ++i)
	{
		pool.push_back(std::thread(&Threadpool::InfiniteLoop, this));
	}

	for (int i = 0; i < n; ++i)
	{
		pool[i].join();
	}

	pool.clear();
}

void Threadpool::PrintData()
{
	// DEBUG purposes
	//std::cout << '\n' << '\n' << "Empty:" << string_empty_total_value << '\n' << "Comment:" << string_comment_total_value << '\n' << "Code:" << string_code_total_value;
	for (auto i_data = result.begin(); i_data != result.end(); i_data++)
	{
		i_data->PrintOutputData();
	}
}

int Threadpool::GetSize()
{
	return queue.size();
}
