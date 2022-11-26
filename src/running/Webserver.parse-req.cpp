/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserver.parse-req.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ren-nasr <ren-nasr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/23 18:11:50 by ren-nasr          #+#    #+#             */
/*   Updated: 2022/11/25 18:24:58 by ren-nasr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <common.hpp>
#include <webserver.hpp>

string Webserver::_extract_body_len(int client_fd, string& rest, unsigned long long len, bool* closed)
{
    string body;
    unsigned long long total = 0;
    body = rest;
    total += rest.size();
    if (total >= len) {
        body = rest.substr(0, len);
        rest = rest.substr(len);
        return body;
    }
    else
    {
        while (total < len)
        {
            char buf[1024];
            int ret = recv(client_fd, buf, 1024, 0);
            if (ret != 1024)
            {
                if ((total + ret) >= len)
                {
                    total += ret;
                    body += string(buf, ret);
                    break;
                }
            }
            if (ret == 0)
            {
                *closed = true;
                if (total == len)
                    break;
                else
                {
                    _set_error_code("400", "Bad Request");
                    return NULL;
                }
            }
            else if (ret == -1)
            {
                _set_error_code("500", "Internal Server Error");
                return body;
            }
            total += ret;
            body += string(buf, ret);
        }
    }
    return body;
}

string Webserver::_extract_body_chunked(int client_fd, string& rest, bool* closed)
{

    char buf[1024];
    string body;
    string chunk;
    long long chunk_size_int;
    // string chunk_size;
    string chunk_size_hex;
    // first check if first chunk is in rest
    if (rest.find("\r\n") != string::npos)
    {
        chunk_size_hex = rest.substr(0, rest.find("\r\n"));
        chunk_size_int = strtol(chunk_size_hex.c_str(), NULL, 16);
        chunk = rest.substr(rest.find("\r\n") + 2, chunk_size_int);
        rest = rest.substr(rest.find("\r\n") + 2 + chunk_size_int);
        body += chunk;
    }
    while (true)
    {
        int ret = recv(client_fd, buf, 1024, 0);
        if (ret == 0)
        {
            *closed = true;
            _set_error_code("400", "Bad Request");
            return body;
        }
        if (ret == -1)
        {
            _set_error_code("500", "Internal Server Error");
            return body;
        }
        chunk_size_hex = string(buf, ret).substr(0, string(buf, ret).find("\r\n"));
        chunk_size_int = strtol(chunk_size_hex.c_str(), NULL, 16);
        if (chunk_size_int == 0)
            return body;
        chunk = string(buf, ret).substr(string(buf, ret).find("\r\n") + 2, chunk_size_int);
        body += chunk;
    }
}

long long Webserver::_get_content_len(const string& headers)
{
    string content_length = headers.substr(headers.find("Content-Length") + 16, headers.find("\r\n") - headers.find("Content-Length") - 16);

    for (string::iterator it = content_length.begin(); it != content_length.end(); it++)
    {
        if (!isdigit(*it))
        {
            _set_error_code("400", "Bad Request");
            return -1;
        }
    }
    long long content_length_int = atoi(content_length.c_str());
    if (content_length_int < 0)
    {
        _set_error_code("400", "Bad Request");
        return -1;
    }
    return content_length_int;
}