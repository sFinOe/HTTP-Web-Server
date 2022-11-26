/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserver.send.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zkasmi <zkasmi@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/25 19:31:27 by zkasmi            #+#    #+#             */
/*   Updated: 2022/11/25 21:29:28 by zkasmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <common.hpp>
#include <webserver.hpp>

int sendall(int socket, const char *buf, size_t *len)
{
    size_t total = 0;
    // how many bytes we’ve sent
    int bytesleft = *len; // how many we have left to send
    int n;
    while(total < *len) {
        n = send(socket, buf + total, bytesleft, 0);
        if (n == -1)
            break;
        total += n;
        bytesleft -= n;
    }
    *len = total; // return number actually sent here
    return n == -1 ? -1 : 0; // return -1 on failure, 0 on success
}

void Webserver::sendblaster(pollfd client_fd)
{
	stringstream buff;
	string buffer;
	cout << _response->http_header << endl;
	fstream file(_response->root.c_str(), fstream::in);
	size_t len;
	if (file){
		buff << file.rdbuf();
		buffer = _response->http_header + buff.str();
		len = buffer.length();
		sendall(client_fd.fd, buffer.c_str(), &len);
	}
}