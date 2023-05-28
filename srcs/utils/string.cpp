#include "utils.hpp"

#include "utils.hpp"

size_t rfind(const std::string& str, const std::string& substr)
{
	size_t pos = str.size();
	size_t sub_len = substr.size();

	if (sub_len > pos)
		return std::string::npos;
	for (size_t i = pos - sub_len; i != std::string::npos; --i)
	{
		if (str.compare(i, sub_len, substr) == 0)
			return i;
	}
	return std::string::npos;
}

std::string	to_string(size_t n)
{
	std::stringstream tmp;

	tmp << n;
	return tmp.str();
}

extern bool running;
std::vector<std::string> fileToVector( std::string path )
{
	std::vector<std::string>	result;
	
	if (path.empty() == true)
		return (result);

	std::ifstream	toRead(path.c_str());

	if	(!isFile(path) or !toRead.good() or path.find(".conf") == std::string::npos)
	{
		std::cerr << "Invalid path file" << std::endl;
		running = false;
	}
	else
	{
		std::string 				buffer;

		while (std::getline(toRead, buffer))
			result.push_back(buffer);
	}
	toRead.close();
	return (result);
}

bool	closeBrace(std::string &src, const int index)
{
	std::string::iterator it = src.begin() + index - 1;
	while (it != src.end() and (*it == ' ' or *it == '\t'))
		it++;
	return (*it == '}');
}

bool	openBrace(std::string &src, const int index)
{
	std::string::iterator it = src.begin() + index - 1;

	while (it != src.end() and (*it == ' ' or *it == '\t'))
		it++;
	return (*it == '{');
}

std::string	findInFileContent(const std::vector<std::string> &file, const std::string &src)
{
	std::string					result;
	int							index;
	std::string::const_iterator it;

	for (std::vector<std::string>::const_iterator start = file.begin(); start != file.end(); start++)
	{
		it = start->begin();
		while (it != start->end() and (*it == ' ' or *it == '\t'))
			it++;
		index = start->rfind(src.c_str(), std::distance(start->begin(), it));
		if (index != -1 and (*(it + src.size()) == ' ' or *(it + src.size()) == '\t') and static_cast<int>(start->find(';', index)) != -1) 
		{
			while (it != start->end() and (*it == ' ' or *it == '\t'))
				it++;
			while (it != start->end() and *it != ' ' and *it != '\t')
				it++;
			while (it != start->end() and (*it == ' ' or *it == '\t'))
				it++;
			if (it != start->end() and *it != ';')
			{
				std::string::const_iterator it2 = it;
				
				while (*it2 != ';')
					it2++;
				it2--;
				while (*it2 == ' ' or *it2 == '\t')
					it2--;
				result = start->substr(std::distance(start->begin(), it), std::distance(it, it2 + 1));
			}
			return (result);
		}
	}
	return (result);
}

std::vector<std::string>	multipleFindInFileContent(const std::vector<std::string> &file, const std::string &src)
{
	std::vector<std::string>	result;
	int							index;
	std::string::const_iterator it;

	for (std::vector<std::string>::const_iterator start = file.begin(); start != file.end(); start++)
	{
		it = start->begin();
		while (it != start->end() and (*it == ' ' or *it == '\t'))
			it++;
		index = start->rfind(src.c_str(), std::distance(start->begin(), it));
		if (index != -1 and (*(it + src.size()) == ' ' or *(it + src.size()) == '\t') and static_cast<int>(start->find(';', index)) != -1) 
		{
			while (it != start->end() and (*it == ' ' or *it == '\t'))
				it++;
			while (it != start->end() and *it != ' ' and *it != '\t')
				it++;
			while (it != start->end() and (*it == ' ' or *it == '\t'))
				it++;
			if (it != start->end() and *it != ';')
			{
				std::string::const_iterator it2 = it;
				
				while (*it2 != ';')
					it2++;
				it2--;
				while (*it2 == ' ' or *it2 == '\t')
					it2--;
				result.push_back(start->substr(std::distance(start->begin(), it), std::distance(it, it2 + 1)));
			}
		}
	}
	return (result);
}

int	isFile(std::string path)
{
	struct stat	stats;

	if (stat(path.c_str(), &stats) == 0)
	{
		if (S_ISREG(stats.st_mode))
			return 1;
	}
	return 0;
}

bool	isValidIP( const std::string &src )
{
	std::vector<int>	srcSplitted;
	std::string			element;

	if (src == "localhost")
		return (true);
	for (std::string::const_iterator it = src.begin(); it != src.end(); it++)
	{
		if (not std::isdigit(*it) and *it != '.')
			return (false);
	}
	
	for (std::string::const_iterator it = src.begin(); it != src.end(); it++)
	{
		element.clear();
		if (*it == '.')
		{
			while (it != src.end() and *it == '.')
				it++;
		}
		std::string::const_iterator it2 = it;
		while (it2 != src.end() and *it2 != '.')
		{
			element += *it2;
			it2++;
		}
		if (element.empty() == false)
			srcSplitted.push_back(std::atoi(element.c_str()));
		it = it2 - 1;
	}
	if (srcSplitted.size() != 4)
		return (false);

	if (srcSplitted[0] == 10)
	{
		for (std::vector<int>::iterator it = srcSplitted.begin() + 1; it != srcSplitted.end(); it++)
		{
			if (*it > 255)
				return (false);
		}
		return (true);
	}
	else if (srcSplitted[0] == 172 and (srcSplitted[1] >= 16 and srcSplitted[1] <= 31))
	{
		for (std::vector<int>::iterator it = srcSplitted.begin() + 2; it != srcSplitted.end(); it++)
		{
			if (*it > 255)
				return (false);
		}
		return (true);
	}
	else if (srcSplitted[0] == 192 and srcSplitted[1] == 168)
	{
		for (std::vector<int>::iterator it = srcSplitted.begin() + 1; it != srcSplitted.end(); it++)
		{
			if (*it > 255)
				return (false);
		}
		return (true);
	}
	else if (srcSplitted[0] == 127 and srcSplitted[1] == 0 and srcSplitted[2] == 0 and srcSplitted[3] == 1)
		return (true);
	else
		return (false);
}
