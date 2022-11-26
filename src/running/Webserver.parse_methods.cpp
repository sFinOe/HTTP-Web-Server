/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserver.parse_methods.cpp                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zkasmi <zkasmi@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/25 19:09:57 by zkasmi            #+#    #+#             */
/*   Updated: 2022/11/26 12:28:14 by zkasmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <common.hpp>
#include <webserver.hpp>

void	query_parsing(get_parse *g_req)
{
	vector<string> data;
	for (size_t i = 0; i < g_req->file_name.length(); i++){
		if (g_req->file_name[i] == '?'){
			data = split(g_req->file_name, "?");
			g_req->file_name = data[0];
			g_req->query = data[1];
		}
	}
}

int find_last_point(string str, size_t i)
{
	for(size_t j = i + 1; j < str.length(); j++){
		if (str[j] == '.')
			return 0;
	}
	return 1;
}

string Webserver::parse_path(string path, get_parse *g_parse)
{
    string tmp_path, type;
    vector<string> data = split(path, "/");
    for(vector<string>::iterator it = data.begin(); it != data.end(); it++)
    {
        for(size_t i = 0; i < (*it).length(); i++)
            if ((*it)[i] == '.' && find_last_point((*it), i) && it + 1 == data.end())
            {
				g_parse->file_type = (*it).substr(i + 1, (*it).length() - i);
                g_parse->file_name = "/" + *it;
				query_parsing(g_parse);
                return tmp_path.substr(0, tmp_path.length() - 1);
            }
        if (it == data.begin())
            tmp_path += "/" + *it + "/";
        else
            tmp_path += *it + "/";
    }
	if (tmp_path.empty())
		return tmp_path = "/";
    return tmp_path.substr(0, tmp_path.length() - 1);
}



h_request *Webserver::parse_request(req_t* req)
{
	h_request	*header;
	v_str_t		str, tmp;

	header = new h_request;
	
	str = split(req->status_line, "\n");
	
	v_str_t::iterator str_it = str.begin();
	if ((*str_it).find("HTTP/") != string::npos){
		tmp = split((*str_it), " ");
		if (tmp[0] == "GET")
			header->method = GET;
		if (tmp[0] == "POST")
			header->method = POST;
		if (tmp[0] == "DELETE")
			header->method = DELETE;
	}
	tmp.clear();
	return header;
}

string fix_path(string str)
{
	string line;
	vector<string> data = split(str, "/");
	for (vector<string>::iterator it = data.begin(); it != data.end(); it++){
		if (*it == data[0] && it + 1 != data.end() && (*it).find(".") != string::npos)
			data.erase(it);
		line += "/" + *it + "/";
	}
	return line;
}


vector<Webserver>::iterator Webserver::target_server(get_parse *g_parse, v_servers &servers)
{
	vector<Webserver>::iterator it;
	if (g_parse->host == "localhost")
		g_parse->host.assign("0.0.0.0");
	for (it = servers.begin(); it != servers.end(); it++){
		for(config_t::iterator ptr = it->_server_data.begin(); ptr != it->_server_data.end(); ptr++)
			if (ptr->second == g_parse->host)
				for(config_t::iterator ptr_2 = it->_server_data.begin(); ptr_2 != it->_server_data.end(); ptr_2++)
					if (ptr_2->second == g_parse->port)
						return it;
	}
	return it;
}

get_parse *Webserver::get_method(req_t* req, v_servers &servers)
{
	v_str_t			str, tmp;
	get_parse		*g_parse;
	v_servers::iterator  server_it;
	string root, header, tmp_path;
	map_strings locs;

	header = req->status_line + "\r\n" + req->headers;
	// cout << header << endl;
	str = split(header, "\n");
	// fill get_parse struct from buffer
	g_parse = new get_parse;
	g_parse->status = 0;
	for (v_str_t::iterator str_it = str.begin(); str_it != str.end(); str_it++) {
		if ((*str_it).find("HTTP/") != string::npos) {
			tmp = split((*str_it), " ");
			g_parse->http_v = trim(tmp[2].substr(5, tmp[2].length() - 3).c_str(), " \n\r\f\t");
			g_parse->path = fix_path(tmp[1]);
			tmp.clear();
		}
		if ((*str_it).find("Host:") != string::npos){
			tmp = split((*str_it), ":");
			g_parse->host = trim(tmp[1], " \n\r\f\t");
			g_parse->port = trim(tmp[2], " \n\r\f\t");
			tmp.clear();
		}
        if ((*str_it).find("Cache-Control:") != string::npos){
			tmp = split((*str_it), ":");
			g_parse->cache_type = trim(tmp[1], " \n\r\f\t");
			tmp.clear();
		}
		if ((*str_it).find("Content-Length:") != string::npos){
			tmp = split((*str_it), " ");
			g_parse->content_length = atol(trim(tmp[1], " \n\r\f\t").c_str());
			tmp.clear();
		}
		if ((*str_it).find("Content-Type:") != string::npos){
			g_parse->content_type = *str_it;
			break;
		}
		
	}
	g_parse->path = parse_path(g_parse->path, g_parse); // path funtion
	// server_it = target_server(g_parse, servers);
	server_it = servers.begin();
	///////////////
	
	if(server_it->_location_data.find(g_parse->path) != server_it->_location_data.end()){
		locs = server_it->_location_data[g_parse->path];
	}
	else if (server_it->_location_data.find("/") != server_it->_location_data.end()){
		locs = server_it->_location_data["/"];
	}
	else if (server_it->_server_data.find("root") != server_it->_server_data.end()){
		locs.insert(make_pair("root", multimap_value(server_it->_server_data, "root")));
	}
	if (g_parse->file_name.empty() && locs.find("allow")->second.find("GET") != string::npos){
		if (locs.find("index") != locs.end())
			root += locs["root"] + g_parse->path + "/" + locs["index"];
		else
			root += locs["root"] + g_parse->path + "/index.html";
	}
	else if (locs.find("allow")->second.find("GET") != string::npos){
		cout << "ergerg ergergerg" << endl;
 		root += locs["root"] + g_parse->path + g_parse->file_name;
	}
	else{
		g_parse->status = 405;
		root = multimap_value(server_it->_server_data, "error_page_405");
	}
	//////////////////////
	
	tmp_path = parse_path(root, g_parse);
	if (g_parse->cache_type == "max-age=0")
        g_parse->status = 304;
	else if (g_parse->status == 0)
        g_parse->status = 200;
	// check for valid path if yes go no error page
	if (g_parse->http_v != "1.1"){
		g_parse->status = 500;
		root = multimap_value(server_it->_server_data, "error_page_500");
	}
    if (error_page(tmp_path)){
        g_parse->status = 404;
    	root = multimap_value(server_it->_server_data, "error_page_404");
    }
    else
    {
        fstream file(root.c_str(), fstream::in);
        if (!file) {
			g_parse->status = 403;
            root = multimap_value(server_it->_server_data, "error_page_403");
        }
        file.close();
    }
	///////////////////
	cout << g_parse->status << endl;
	g_parse->path = root;
	return g_parse;
}


h_response *Webserver::generate_header(h_request *request, int code_type)
{
	get_parse *g_parse;
	h_response *response = new h_response;
	if (code_type == 404)
		response->code_status = "HTTP/1.1 404 Not Found\r\n";
	if (code_type == 405)
		response->code_status = "HTTP/1.1 405 Method Not Allowed\r\n";
	if (code_type == 403)
		response->code_status = "HTTP/1.1 403 Not Found\r\n";
	if (code_type == 500)
		response->code_status = "HTTP/1.1 500 Internal Server Error\r\n";
	if (code_type == 200)
		response->code_status = "HTTP/1.1 200 OK\r\n";
	if (code_type == 304)
		response->code_status = "HTTP/1.1 304 OK\r\n";
	response->keep_connection = "Connection: Keep-Alive\r\nKeep-Alive: timeout=15, max=100\r\n";
	response->date = "Date: Mon, 21 Nov 2022 12:36:23 GMT\r\n";
	response->server_type = "Server: boostcp/1.2.0 (YumOS)\r\n";
	if(request->method == GET){
		g_parse = (get_parse*)request->parsed_method;
		response->root = g_parse->path;
		response->content_length = count_content_length(g_parse->path);
		g_parse->path = parse_path(g_parse->path, g_parse);
		response->content_type = "Content-Type: " + _mime_types[g_parse->file_type] + "; charset=UTF-8\r\n\r\n";
		if (response->content_type.empty())
			response->content_type = "Content-Type: text/html; charset=UTF-8\r\n\r\n";
		response->http_header = response->code_status + response->keep_connection
			+ response->date + response->server_type + response->content_length + response->content_type;
	}
	return (response);
		
}


h_response *Webserver::method_parsing(req_t* req, v_servers &servers)
{
	h_response *response;
	get_parse	*g_parse;
	
	if (_request->method == GET) { // GET METHOD
		_request->parsed_method = (void*)get_method(req, servers); // parse get header and check for root
		g_parse = (get_parse *)_request->parsed_method;
		response = generate_header(_request, g_parse->status);
		response->request = _request;
		response->g_parse = g_parse;
    }
	return response;
	
}

