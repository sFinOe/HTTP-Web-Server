/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserver.validate-req.cpp                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ren-nasr <ren-nasr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/23 18:12:28 by ren-nasr          #+#    #+#             */
/*   Updated: 2022/11/25 18:50:51 by ren-nasr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <webserver.hpp>
#include <common.hpp>


bool Webserver::_valid_host(const string& host) {
    // if host doesn't contain an ip and a port it's invalid
    if (host.find(':') == string::npos)
        return false;
    // if host contains an ip and a port it's valid
    string ip = host.substr(0, host.find(':'));
    string port = host.substr(host.find(':') + 1);
    size_t port_num = atoi(port.c_str());
    if (!(ip == "localhost")) {
        // check if ip is valid
        // if no dot in ip it's invalid
        if (ip.find('.') == string::npos)
            return false;
        // split ip by dots
        vector<string> ip_parts;
        for (size_t i = 0; i < ip.size(); i++) {
            if (ip[i] == '.') {
                ip_parts.push_back(ip.substr(0, i));
                ip = ip.substr(i + 1);
                i = 0;
            }
        }
        // if it doesn't contain 4 parts it's invalid
        if (ip_parts.size() != 4)
            return false;
        for (size_t i = 0; i < 4; i++) {
            // checking the size of each part of ip address (should be 1-3 digits) ex: 192
            if (ip_parts[i].size() > 3)
                return false;
            // if each part of ip address is not a number it's invalid
            for (size_t j = 0; j < ip_parts[i].size(); j++) {
                if (!isdigit(ip_parts[i][j]))
                    return false;
            }
            // if it's greater than 255 it's invalid
            if (port_num > 255)
                return false;
        }
    }
    // check if port is valid
    // if port is not a number it's invalid
    for (size_t i = 0; i < port.size(); i++) {
        if (!isdigit(port[i]))
            return false;
    }
    // if port is greater than 65535 it's invalid
    if (port_num > 65535 || port_num < 1024)
        return false;
    return true;

}
bool Webserver::_valid_req(const req_t* req)
{
    // validating all the syntax of the request
    // if the request is valid, return true, else return false
    // check if the status line is valid
    if (!_valid_req_line(req->status_line))
        return false;
    if (!_valid_req_headers(req->headers))
        return false;
    return true;
}

bool Webserver::_valid_req_headers(string headers)
{
    // checking if all the headers are valid syntactically
    // if they are valid return true, else return false
    // if host is present, check if it's a valid host
    if (headers.find("Host") != string::npos)
    {
        // check if the host is valid
        string host = headers.substr(headers.find("Host") + 6);
        host = host.substr(0, host.find("\r\n"));
        if (!_valid_host(host))
            return false;
    }
    return true;
}

bool Webserver::_valid_req_line(const string& status_line)
{

    if (status_line.empty())
    {
        _set_error_code("400", "Bad Request");
        return false;
    }
    if (status_line.find("HTTP/") == string::npos)
    {
        _set_error_code("400", "Bad Request");
        return false;
        // if http version is not 1.1 return false
        if (status_line.find("HTTP/1.1") == string::npos)
        {
            _set_error_code("505", "HTTP Version Not Supported");
            return false;
        }
    }
    if (status_line.find("GET") == string::npos && status_line.find("POST") == string::npos && status_line.find("DELETE") == string::npos)
    {
        _set_error_code("405", "Method Not Allowed");
        return false;
    }
    return true;
}
