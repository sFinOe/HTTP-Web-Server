/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserver.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zkasmi <zkasmi@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/31 17:23:06 by ren-nasr          #+#    #+#             */
/*   Updated: 2022/11/25 21:14:22 by zkasmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERVER_HPP
# define WEBSERVER_HPP
#include <common.hpp>

// don't forget to uncomment syntax_block condition, and error_pages

typedef struct req_s {
	string status_line;
	string headers;
	string body;
} req_t;


// typedef struct res_status_line {
// 	string version;
// 	string status_code;
// 	string status_message;
// };

typedef struct res_s {
	string status_code;
	string status_message;
} res_t;


// typedef struct res_s {

// 	// res_status_line status_line;
// 	string headers;
// 	string body;
// } res_t;



/***************** request data *******************/

typedef struct {

	string name;
	string value;
	string content_type;
	string filename;
	string content_disposition;
}	h_body;


typedef struct{
	
	string path;

} post_req;

typedef struct {

	int	status;
	string http_v;
	string path;
	string host;
	string port;
	string file_name;
	string file_type;
	string cache_type;
	string content_type;
	string content_length;
	string query;
}	get_parse;



typedef struct{
	
	int method;
	void *parsed_method;
	
} h_request;

/***************** response data *******************/

typedef struct {
	
	string date;
	string http_header;
	string code_status;
	string keep_connection;
	string content_length;
	string content_type;
	string server_type;
	string root;
	get_parse *g_parse;
	h_request *request;
	
} h_response;

/**************************************************/



class Webserver {

public:

	/*********** Parsing exception ***************/

	class parsingerror : public std::exception {
		virtual const char* what() const throw() {
			return "\033[1;31mConfig error : server syntax error\033[0m";
		}
	};

	class location_error : public std::exception {
		virtual const char* what() const throw() {
			return "\033[1;31mConfig error : location syntax error\033[0m";
		}
	};

	class server_data_error : public std::exception {
		virtual const char* what() const throw() {
			return "\033[1;31mConfig error : server data syntax error\033[0m";
		}
	};


	/*********** server exception ***************/

	class PollException : public std::exception {
		virtual const char* what() const throw() {
			return "poll() failed";
		}
	};

	class AcceptException : public std::exception {
		virtual const char* what() const throw() {
			return "accept() failed";
		}
	};

	class ListenException : public std::exception {
		virtual const char* what() const throw() {
			return "listen() failed";
		}
	};

	class BindException : public std::exception {
		virtual const char* what() const throw() {
			return "bind() failed";
		}
	};

	class SocketException : public std::exception {
		virtual const char* what() const throw() {
			return "socket() failed";
		}
	};

	class InvalidHostException : public std::exception {
		virtual const char* what() const throw() {
			return "Invalid host";
		}
	};


	class RecvException : public std::exception {
		virtual const char* what() const throw() {
			return "recv() failed";
		}
	};
	/*************** member types ***************/

	typedef multimap<string, string> config_t;
	typedef map<string, map<string, string> > locations_t;
	typedef map<string, string> mime_t;


	/************** Webserver member functions **************/

	Webserver(config_t server_vals, locations_t location_vals, mime_t mime_types);
	Webserver(vector<Webserver> servers);

	/************* Parsing public member functions ***************/

	static vector<Webserver> parse(char* path);

	/********************************************************/

	// up create socket, bind it and start lisiting on the port
	void up();
	// actually launch the infinite loop to repsonde to clients requests

	void run(vector<Webserver> servers);
	void shutdown();
	void restart();


	/// dump functions
	void dump_server_data() {
		for (config_t::iterator it = _server_data.begin(); it != _server_data.end(); it++) {
			cout << it->first << " : " << it->second << endl;
		}
	}

	void dump_location_data() {
		for (locations_t::iterator it = _location_data.begin(); it != _location_data.end(); it++) {
			cout << it->first << endl;
			for (map<string, string>::iterator it2 = it->second.begin(); it2 != it->second.end(); it2++) {
				cout << it2->first << " : " << it2->second << endl;
			}
		}
	};

private:
	// vector<Webserver> _servers;
	config_t _server_data;
	locations_t _location_data;
	// cgi_
	mime_t _mime_types;
	static set<int> _sockets;
	res_t* _res;
	h_response *_response;
	h_request *_request;
	// res_t* _response;
	/**************** private member types *****************/
	typedef vector<string> v_str_t;
	typedef vector<string>::iterator v_str_it;
	typedef map<string, map<string, string> >::iterator locations_it;
	typedef multimap<string, string>::iterator config_it;
	typedef vector<Webserver>::iterator servers_it;
	typedef map<string, string>::iterator map_str_it;
	typedef vector<int> v_int_t;
	typedef vector<int>::iterator v_int_it;
	typedef map<string, string> map_strings;
	typedef vector<Webserver> v_servers;


	/**************** Parsing private member functions ******************/

	static multimap<string, string> parse_server_data(string& line, config_t _server_data);
	static map<string, map<string, string> > parse_locations(string& line, locations_t _locations);
	static map<string, string> mime_parsing(config_t _server_data);
	static vector<string> split_servers(char* path);
	static void error_parsing(config_t server_data, locations_t locations);
	static void    duplicate_server_name(config_t next, config_t prev, string type);
	static void exit_error(config_t server_data, locations_t locations, string err_str, int exit_type);
	static int path_duplicate(string path, locations_t _locations);
	static int location_value_comp(string key, string value, locations_it path);
	static int location_value_comp_2(string key, string value, string* names);
	static int duplicate_location_data(string path, mime_t location);
	static int data_value_duplicate(config_t server_data);
	static int value_comp(string key, string value);
	static void    initialize_block(locations_t& _locations, config_t& _server_data);

	/*******************************************************************/

	req_t* _parse_request(int client_fd, bool* closed);
	// request stuff 
	int _send(int client_fd, const char* buf, size_t len);
	req_t* _recv(int client_fd, bool* close);

	string _get_req_headers(int client_fd, string req, string& rest, bool closed);
	req_t* _extract_req(int client_fd, const string& req, string& rest, bool* closed);

	bool _valid_req(const req_t* req);
	// errors
	bool _valid_status_line(string& status_line);
	bool _valid_req_line(const string& req_line);
	bool _valid_req_headers(string headers);
	bool _valid_host(const string& host);
	// closing
	void _close_client(int client_fd);
	
	// extract body from request
	string _extract_body_len(int client_fd, string& rest, unsigned long long len, bool* closed);
	long long _get_content_len(const string& headers);
	string _extract_body_chunked(int client_fd, string& rest, bool* closed);
	// errors
	void _error(int errono);
	void _set_error_code(string status_code, const string& status_msg);
	void _handld_bad_req(vector<pollfd>& fds, size_t* i, bool* closed);
	int _recv_wrap(int client_fd, char* buff, size_t len, int flags);
	// method parsing
	h_request *parse_request(req_t* req);
	h_response *method_parsing(req_t* req, v_servers &servers);
	get_parse *get_method(req_t* req, v_servers &servers);
	string parse_path(string path, get_parse *g_parse);
	vector<Webserver>::iterator target_server(get_parse *g_parse, v_servers &servers);
	string multimap_value(multimap<string, string> data, string key);
	h_response *generate_header(h_request *request, int code_type);
	int error_page(string path);
	string count_content_length(string root);
	void sendblaster(pollfd client_fd);
};


/*************** Parsing function  ****************/

vector<string>	split(string s, const string set);
size_t	value_len(string str, size_t i);
size_t	key_len(string str);
string	trim(string s, const string set);
int	empty_brackets(string line);
int	num_check(string value);
int	location_key_comp(string key);
int	key_comp(string key);
int syntax_block(string line);


/**************************************************/

#endif
