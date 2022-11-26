/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserver.error.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zkasmi <zkasmi@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/22 16:50:17 by ren-nasr          #+#    #+#             */
/*   Updated: 2022/11/26 12:35:59 by zkasmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include <common.hpp>
#include <webserver.hpp>


void Webserver::_error(int errorno) {
    cout << "\033[1;31mError:\033[0m " << strerror(errorno) << endl;
    exit(1);
}


void Webserver::_set_error_code(string status_code, const string& msg) {
    _res->status_code = status_code;
    _res->status_message = msg;
}

void Webserver::_handld_bad_req(vector<pollfd>& fds, size_t* i, bool* closed) {
    int index = *i;
    if (_res->status_code == "400" || _res->status_code == "405" || _res->status_code == "505" || _res->status_code == "500")
    {
        string resp = "HTTP/1.1 " + _res->status_code + " " + _res->status_message + "\r\n";
        char* response = (char*)resp.c_str();
        _send(fds[index].fd, response, strlen(response));
        _close_client(fds[index].fd);
        fds.erase(fds.begin() + index);
        // *i -= 1;
        // // resetting..
        *closed = 0;
        _res->status_code = "";
        _res->status_message = "";
    }
    else if (*closed) {
        _close_client(fds[index].fd);
        fds.erase(fds.begin() + index);
        // *i -= 1;
        closed = 0;
    }
}