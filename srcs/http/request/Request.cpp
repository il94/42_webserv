#include "Request.hpp"

/*=============================== Constructors ===============================*/
Request::Request(void): _requestContent(""), _reqBody(""),  _ret(200){}

Request::Request(const Request & src)
{
	_data = src._data;
	_headerM = src._headerM;
	_requestContent = src._requestContent;
	_reqBody = src._reqBody;
	_ret = src._ret;
}

Request	&	Request::operator=(const Request & src)
{
	_data = src._data;
	_headerM = src._headerM;
	_requestContent = src._requestContent;
	_reqBody = src._reqBody;
	_ret = src._ret;
	return (*this);
}


Request::~Request(void){}

/*================================= Methods ==================================*/

bool	Request::badFirstLine()
{
	_ret = 400;
	if(_data.size() != 3 || _data[2].find("HTTP/") == std::string::npos)
	{
		std::cerr << RED << "Error : the request cannot be processed, somes elements are missing" << END << std::endl;
		return true;
	}
	if (_data[2] != "HTTP/1.0" && _data[2] != "HTTP/1.1")
	{
		std::cerr << RED << "Error : the request cannot be processed, BAD HTTP VERSION" << END << std::endl;
		return true;
	}
	if (_data[0] != "POST"  && _data[0] != "DELETE" && _data[0] != "GET")
	{
		std::cerr << RED << "Error : the request cannot be processed, INVALID METHOD" << END << std::endl;
		return true;
	}
	_ret = 200;
	return false;
}

std::vector<std::string>	Request::splitURL()
{
	std::vector<std::string>	splitedURL;
	std::string	URL = 			getURL();

	URL = URL[URL.size() - 1]  != '/' ? URL + "/" : URL;

	do
	{
		splitedURL.push_back(URL);
		URL = URL.substr(0, rfind(URL, "/"));
	} while (URL != "");
	return (splitedURL);
}

void	Request::parseHeader(const std::string & req)
{
	std::string	firstReqLine(req, 0, req.find("\n"));
	std::string	tmp;
	std::stringstream	ss(firstReqLine);

	while (ss >> tmp)
		_data.push_back(tmp);

	if (badFirstLine() == true)
		return ;

	size_t	bodyPosition = req.find("\r\n\r\n");
	if (bodyPosition == std::string::npos)
	{
		_ret = 400;
		return;
	}

	std::string	header = req.substr(0, bodyPosition + 4);
	size_t	boundary = std::string::npos;
	std::istringstream	stream(header);

	while (std::getline(stream, tmp))
	{
		if (tmp == "")
			break;
		boundary = tmp.find(":");
		if (boundary != std::string::npos)
		{
			std::string	key(tmp, 0, boundary);
			std::string	value(tmp, boundary + 2);
			_headerM[key] = value;
		}
	}
	if (bodyPosition != std::string::npos)
		_reqBody = req.substr(bodyPosition + 4);
}

void	Request::parseBody()
{
	std::string	queryString = "";

	_requestContent = "";
	if (getMethod() == "GET")
	{
		std::string	URL = getURL();
		size_t	queryPos = URL.find("?");
		if (queryPos == std::string::npos)
			return;
		_data[1] = URL.substr(0, queryPos);
		queryString = URL.substr(queryPos + 1);
	}
	else if (getMethod() == "POST")
		queryString = _reqBody;
	else
		return ;
	_requestContent = queryString;
}

/*================================ Accessors =================================*/

std::string Request::getURL() const { return _data[1]; }

std::string Request::getMethod() const { return _data[0]; }

std::string Request::getRequestContent() const { return _requestContent; }

std::string Request::getHTTPVersion() const { return _data[2]; }

std::string Request::getElInHeader(const std::string & key) { return _headerM[key]; }

std::map<std::string, std::string>	Request::getHeaderM() { return _headerM; }

std::string Request::getRequestBody() const { return _reqBody; }

int	Request::getRet() const {return _ret; }

void	Request::setRequestContent(std::string src) {_requestContent = src; }