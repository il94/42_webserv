
#ifndef SERVER_HPP
#define SERVER_HPP

#include <sys/socket.h>
#include <sys/epoll.h>

# include "../../include/webserv.hpp"
#include "../config/Config.hpp"
#include "Client.hpp"

#define DBG 0

#ifndef TIMEOUT
#define TIMEOUT 5
#endif

typedef struct s_config
{
	size_t						size;
	std::vector< std::string >	names;
	std::vector< int		 >	ports;
	std::vector< int		 >	srvfd;
}								t_config;

/*
 * Server execution order :
 * 	1/ set :
 * 		_nbSrv
 * 		_names
 * 		_ports
 * 		_sockkaddr
 * 		_eplevs
 * 		_eplfd
 * 	2/ initsrv :
 * 		make socket
 * 		setsockopt
 * 		bind
 * 		epoll_ctl ADD socket into epoll instance survey
 * 		set socket into struct epoll_event
 * 		listen on each port
 * 		run
 */ 

class Server
{
	public:
		typedef		struct sockaddr							t_sockaddr;
		typedef 	struct sockaddr_in						t_sockaddr_in;
		typedef		struct epoll_event						t_epollEv;
		typedef		std::vector < std::string	>			t_vString;
		typedef		std::vector < int			>			t_vInt;
		typedef		std::vector < t_epollEv		>			t_vEplEv;
		typedef		std::vector	< t_sockaddr	>			t_vSockaddr;
		typedef		std::vector < t_flag		>			t_vFlag;
		typedef		std::vector	< Config		>			t_vConfig;
		typedef		std::vector<std::vector <std::string> >	t_vvString;

							Server(void);
							~Server(void);

		void				run(void);
		void				closeCliSocket(int cliSocket);

		void				display(void);
		t_vvString			extractContent( const std::string & ) ;
		void				setConfigs( char **av );


		class WrongSize : public std::exception {
			public:
				const char * what(void) const throw() {
					return ("Wrong size: size parameter out of bound.");
				}
		};
		class Duplicate : public std::exception {
			public:
				const char * what(void) const throw() {
					return ("Duplicate: these parameter already exists.");
				}
		};

	private:
		std::vector<std::vector <std::string> >	_content;

		t_vConfig			_configs;
		int					_eplfd;
		int					_nbSrv;
		
		t_vString			_names;
		t_vInt				_ports;
		t_vSockaddr			_sockaddr;
		t_vEplEv			_eplevs;
		t_vInt				_srvfd;

		void				_initSrv(void);
		int					_acceptConnection(const int & j, Client & client);
		std::string		  &	_readRequest( Client & client, int & cliSocket, std::string & request );
		int					_storeResponse( Client & client, const int & cliSocket, std::string & request );
		void				_sendResponse( int & cliSocket, Client & client, size_t size );
		void				_displayError(const char *func, const int line, const char *msg);

		size_t	  			_getNbSrv(void)					const;
		const int		  &	_getEplFd(void)					const;
		const std::string &	_getName(const size_t & i)		const;
		size_t	  			_getPort(const size_t & i)		const;
		const int	 	  &	_getSrvFd(const size_t & i)		const;
		const t_sockaddr  &	_getSockaddr(const size_t & i)	const;
		const t_epollEv	  &	_getEpollEv(const size_t & i)	const;
		size_t				_getNbSockets(void)				const;

		void				_setName(const std::string & name);
		void				_setPort(const int & port);
		void				_setSrvFd(const int & fd);
		void				_setSockaddr(void);
		void				_setEplevs(void);
		t_vvString			getContent( void )				const ;
		t_vConfig			getConfigs( void )				const ;
		Config				getConfig( int & port )			const ;

		void				setContent( const t_vvString & );

							Server(const Server & src);
		Server			 &	operator=(const Server & srv);
};

#endif
