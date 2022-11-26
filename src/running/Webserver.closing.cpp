/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserver.closing.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ren-nasr <ren-nasr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/22 15:19:52 by ren-nasr          #+#    #+#             */
/*   Updated: 2022/11/25 18:25:10 by ren-nasr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include <common.hpp>
#include <webserver.hpp>

void    Webserver::_close_client(int client_fd) {
    // std::cout << "Closing client" << std::endl;
    close(client_fd);
    _sockets.erase(client_fd);
}