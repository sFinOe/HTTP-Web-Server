/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserver.running.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zkasmi <zkasmi@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/22 09:26:25 by ren-nasr          #+#    #+#             */
/*   Updated: 2022/11/26 12:29:21 by zkasmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <common.hpp>
#include <webserver.hpp>

void Webserver::up()
{
    pair<config_t::const_iterator, config_t::const_iterator> range = _server_data.equal_range("listen");
    int sock;
    // start by assuming host is localhost
    u_int32_t host = inet_addr("127.0.0.1");
    // if it's not localhost check for validity
    if (_server_data.find("host")->second != "localhost")
    {
        host = inet_addr(_server_data.find("host")->second.c_str());
        if (host == INADDR_NONE)
        {
            std::cerr << "\033[1;31mError:\033[0m Invalid host address" << std::endl;
            exit(1);
        }
    }

    // loop all listen values (ports)
    for (; range.first != range.second; ++range.first)
    {
        // create a socket for each port
        sock = socket(AF_INET, SOCK_STREAM, 0);
        if (sock < 0)
            _error(errno);
        sockaddr_in addr;
        addr.sin_family = AF_INET;
        addr.sin_addr.s_addr = host;
        addr.sin_port = htons(atoi(range.first->second.c_str()));
        memset(&(addr.sin_zero), '\0', 8);
        // check if kernel is hanged in kernel if so reuse it
        int optval = 1;
        if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) < 0)
            _error(errno);
        if (bind(sock, (sockaddr*)&addr, sizeof(addr)) < 0)
        {
            // if bind fails because of address already in use. ignore it
            if (errno == EADDRINUSE)
                continue;
            _error(errno);
        }
        if (listen(sock, 10) < 0)
            _error(errno);
        _sockets.insert(sock);
    }
}

void Webserver::run(vector<Webserver> servers)
{
    vector<pollfd> fds;
    int num_servers = _sockets.size();
    // initializing all the servers
    set<int>::iterator it = _sockets.begin();
    for (size_t i = 0; i < _sockets.size(); ++i, ++it)
    {
        pollfd fd;
        fd.fd = *it;
        fd.events = POLLIN;
        fds.push_back(fd);
    }

    for (;;)
    {
        // poll the sockets
        int ret = poll(fds.data(), fds.size(), -1);
        if (ret < 0)
        {
            if (errno == EINTR)
                continue;
            _error(errno);
        }
        for (size_t i = 0; i < fds.size(); ++i)
        {
            // if there's a read event is server's sockets, there's an incoming connection
            if (fds[i].revents & POLLIN && (int)i < num_servers)
            {
                // accept the connection
                int client_fd = 0;
                if ((client_fd = accept(fds[i].fd, NULL, NULL)) < 0)
                    throw AcceptException();
                pollfd fd;
                fd.fd = client_fd;
                fd.events = POLLIN;
                fds.push_back(fd);
            }
            // else there's a request from a connected client
            else if (fds[i].revents & POLLIN && (int)i >= num_servers)
            {
                bool closed = 0;
                req_t* req = _parse_request(fds[i].fd, &closed);
                _handld_bad_req(fds, &i, &closed);

                /////// handle parsing of methods and response!!!!!!/////////
                if (req != NULL){
                    _request = parse_request(req);
                    _response = method_parsing(req, servers);
                    sendblaster(fds[i]);
                }

                /////////////////////////////
            }
        }
    }
}