/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserver.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zkasmi <zkasmi@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/31 17:23:06 by ren-nasr          #+#    #+#             */
/*   Updated: 2022/11/21 16:56:55 by zkasmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERVER_HPP
# define WEBSERVER_HPP
#include <common.hpp>

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

	class bind_error : public std::exception {
		virtual const char* what() const throw() {
			return "error";
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
	mime_t _mime_types;

	/**************** private member types *****************/
	typedef vector<string> v_str_t;
	typedef vector<string>::iterator v_str_it;
	typedef map<string, map<string, string> >::iterator locations_it;
	typedef multimap<string, string>::iterator config_it;
	typedef vector<Webserver>::iterator servers_it;
	typedef map<string, string>::iterator map_str_it;
	typedef vector<int> v_int_t;
	typedef vector<int>::iterator v_int_it;


	/**************** Parsing private member functions ******************/

	static multimap<string, string> parse_server_data(string& line, config_t _server_data);
	static map<string, map<string, string> > parse_locations(string& line, locations_t _locations);
	static map<string, string> mime_parsing(config_t _server_data);
	static vector<string> split_servers(char* path);
	static void error_parsing(config_t server_data, locations_t locations);
	static void	duplicate_server_name(config_t next, config_t prev);
	static void exit_error(config_t server_data, locations_t locations, string err_str, int exit_type);
	static int path_duplicate(string path, locations_t _locations);
	static int location_value_comp(string key, string value, locations_it path);
	static int location_value_comp_2(string key, string value, string* names);
	static int duplicate_location_data(string path, mime_t location);
	static int data_value_duplicate(config_t server_data);
	static int value_comp(string key, string value);
	static void	root_block(locations_t &_locations, config_t _server_data);

	/*******************************************************************/

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
