#ifndef	REQUEST_HPP
# define REQUEST_HPP

# include "../../../include/webserv.hpp"
# include <map>
# include <string>
# include <map>
# include <vector>
# include <sstream>
# include <cstddef>

class	Request
{
	public:
		Request(void);
		Request(const Request & src);
		~Request(void);

		Request	&	operator=(const Request & src);


		std::string	getURL() const;
		int			getRet() const;
		std::string	getMethod() const;
		std::string	getHTTPVersion() const;
		std::string	getRequestBody() const;
		std::string	getRequestContent() const;
		std::string	getElInHeader(const std::string & key);
		std::map<std::string, std::string>	getHeaderM();

		void	setRequestContent(std::string str);
		bool	badFirstLine();
		std::vector<std::string>	splitURL();

		void	parseHeader(const std::string & req);
		void	parseBody();
	
	private:
		std::vector<std::string>			_data;
		std::map<std::string, std::string>	_headerM;

		std::string							_requestContent;
		std::string							_reqBody;
		int									_ret;

};

#endif