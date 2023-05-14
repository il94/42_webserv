#include "Response.hpp"

std::string	Response::generateAutoIndex()
{
	std::string	path = "." + _request.getURL();
	DIR 		*dir = opendir(path.c_str());
	std::vector<std::string>	hosts = _config.getHost();
	std::vector<int>			ports = _config.getPort();

	if (dir == NULL)
	{
		std::cerr << RED << "Error : AutoIndex cannot processed, \n "<<
		"please check the following path --> " << path
		<< END << std::endl;
		return "";
	}
	std::string	autoIndex = \
		"<!DOCTYPE html>\n\
		<head>\n\
			<title> " + path + " </title>\n\
		</header>\n\
		<body>\n\
			<h1> Index of " + path + " </h1>\n\
		</body>\n\
		<p>\n";

	struct dirent	*readedDir = readdir(dir);
	while (readedDir)
	{
		autoIndex += "\t\t<p><a href=\"http://" + _host\
			+ ":" + to_string(_port) + "/" + path + "/" + std::string(readedDir->d_name)\
			+ "\">" + std::string(readedDir->d_name) + "</a></p>\n";
		readedDir = readdir(dir);
	}
	
	autoIndex += \
		"</p>\n\
		</body>\n\
		</html>\n";
	closedir(dir);
	return autoIndex;
}