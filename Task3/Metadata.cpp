#include "Metadata.h"

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
	while (!str.empty())
	{
		ext.push_back(str);
		std::cin >> str;
	}
}

void Metadata::AddExtAsArg(char* arg)
{
	ext.push_back(arg);
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
