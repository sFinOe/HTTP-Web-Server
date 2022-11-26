/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserver.request.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ren-nasr <ren-nasr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/22 09:58:19 by ren-nasr          #+#    #+#             */
/*   Updated: 2022/11/25 18:24:13 by ren-nasr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// 1. if transfer encoding is present, and the chunked transfer encoding is the final encoding , the message body length is
// determined by reading and decoding the chunked data until the transfer coding indicates the data is complete. else if the tranfer-encoding
// is present and the chunked transfer encoding is not the final encoding, the message body cannot be determined and the server must respond with 400 bad request.

// 2.
#include <common.hpp>
#include <webserver.hpp>

req_t* Webserver::_parse_request(int client_fd, bool* closed)
{
    req_t* request = _recv(client_fd, closed);
    if (request && !_valid_req(request))
    {
        _set_error_code("400", "Bad Request");
        return NULL;
    }
    return request;
}

req_t* Webserver::_extract_req(int client_fd, const string& req, string& rest, bool* closed)
{

    req_t* ret;
    try
    {
        ret = new req_t;
    }
    catch (std::bad_alloc& e)
    {
        std::cerr << "\033[1;31mError:\033[0m " << e.what() << std::endl;
        exit(1);
    }
    string status_line = req.substr(0, req.find("\r\n"));
    string headers = req.substr(req.find("\r\n") + 2, req.find("\r\n\r\n") - req.find("\r\n") - 2);
    rest = req.substr(req.find("\r\n\r\n") + 4, req.size() - req.find("\r\n\r\n") - 4);
    ret->status_line = status_line;
    ret->headers = headers;
    // if method is get or post ignore body and return
    if (status_line.substr(0, status_line.find(" ")) == "GET" || status_line.substr(0, status_line.find(" ")) == "DELETE")
    {
        ret->body = "";
        rest = req.substr(req.find("\r\n\r\n") + 4);
        return ret;
    }
    // if the header contains a content-length, extract number of buytes for body;
    else if (headers.find("Content-Length") != string::npos)
    {
        long long content_length = _get_content_len(headers);
        if (content_length == -1)
        {
            _set_error_code("400", "Bad Request");
            return NULL;
        }
        // substracting the length of the body from the length of the request 
        ret->body = _extract_body_len(client_fd, rest, content_length, closed);
        // if body is not complete, return an error
        if (_res->status_code == "400" || _res->status_code == "500")
            return NULL;
    }
    // if there's a transfer encoding: chunked, extract the body
    else if (headers.find("Transfer-Encoding: chunked") != string::npos)
    {
        ret->body = _extract_body_chunked(client_fd, rest, closed);
        if (_res->status_code == "400" || _res->status_code == "500")
            return NULL;
    }
    else
    {
        // error
        _set_error_code("400", "Bad Request");
        return NULL;
    }
    return ret;
}

int Webserver::_send(int client_fd, const char* buff, size_t len)
{
    int total = 0;
    int bytesleft = len;
    int n = 0;

    while (total < (int)len)
    {
        n = send(client_fd, buff + total, bytesleft, 0);
        if (n == -1)
        {
            break;
        }
        total += n;
        bytesleft -= n;
    }
    return n == -1 ? -1 : total;
}

req_t* Webserver::_recv(int client_fd, bool* closed)
{
    string req;
    string rest;
    // string extracted_req;
    char buff[1024];
    int n = recv(client_fd, buff, 1024, 0);

    if (n == -1)
    {
        _set_error_code("500", "Internal Server Error");
        return NULL;
    }
    if (n == 0)
    {
        *closed = true;
        return NULL;
    }
    req += string(buff, n);
    req_t* extracted_req = _extract_req(client_fd, req, rest, closed);
    return extracted_req;
}
