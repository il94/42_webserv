#include "Response.hpp"

std::string	Response::generateAutoIndex()
{
	std::string	path = _path;
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

	std::string URL = _request.getURL();
	URL = URL.back() != '/' ? URL += '/' : URL;

	while (readedDir)
	{
		autoIndex += "\t\t<p><a href=\"http://" + std::string("localhost") \
			+ ":" + to_string(_port) + URL + std::string(readedDir->d_name)\
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
