#include "Metadata.h"

void Metadata::StringOut(std::string str)
{
	std::cout << str << '\n';
}

Metadata::Metadata()
{
}

Metadata::Metadata(std::string path_)
{
	path = path_;
}

void Metadata::GetExt()
{
	for (auto i_ext = ext.begin(); i_ext != ext.end(); i_ext++)
	{
		std::cout << *i_ext << '\n';
	}
}

void Metadata::ClearExt()
{
	ext.clear();
}

void Metadata::AddExt()
{
	std::string str;
	std::cin >> str;
	while(!str.empty())
	{
		ext.push_back(str);
		std::cin >> str;
	}
}

void Metadata::SearchForFiles()
{
	for (auto& p : std::filesystem::recursive_directory_iterator(path))
	{
		for (auto i_ext = ext.begin(); i_ext != ext.end(); i_ext++)
		{
			if (p.path().extension() == *i_ext)
			{
				files.push_back(p.path().string());
			}
		}
	}
}

void Metadata::PrintFiles()
{
	for (auto i_files = files.begin(); i_files != files.end(); i_files++)
	{
		std::cout << *i_files << '\n';
	}
	std::cout << '\n';
}

void Metadata::ReadFiles()
{
	std::string line;
	bool is_in_comment;
	std::smatch sm;
	for (auto i_files = files.begin(); i_files != files.end(); i_files++)
	{
		is_in_comment = false;
		std::ifstream infile(*i_files);
		if (!infile)
		{
			std::cout << "Continue" << '\n';
			continue;
		}
		while (getline(infile, line))
		{
			if (line.length() == 0 || std::regex_match(line, sm, empty_regex))
			{
				++string_empty_value;
				if (is_in_comment)
				{
					++string_comment_value;
					std::cout << "Continue" << '\n';
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
					std::cout << "End" << '\n';
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
				std::cout << "Start" << '\n';
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
				std::cout << "Comm" << '\n';
				std::cout << sm.str() << sm.suffix() << '\n';
			}
		}
	}
	std::cout << '\n' << '\n' << "Empty:" << string_empty_value << '\n' << "Comment:" << string_comment_value << '\n' << "Code:" << string_code_value;
}

