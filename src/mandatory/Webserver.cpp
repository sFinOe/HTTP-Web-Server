/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserver.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ren-nasr <ren-nasr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/09 14:16:41 by ren-nasr          #+#    #+#             */
/*   Updated: 2022/12/02 17:37:22 by ren-nasr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/common.hpp"
#include "../includes/webserver.hpp"

// defining the static variable
set<int> Webserver::_sockets;


// Constructors 
Webserver::Webserver(config_t server_vals, locations_t locations_vals, mime_t mime_types) {
    _server_data = server_vals;
    _location_data = locations_vals;
    _mime_types = mime_types; 
    try {
        // _res = new res_t;
        _request = new h_request;
        _response = new h_response;
    }
    catch (std::bad_alloc &e) {
        std::cout << "Error: " << e.what() << std::endl;
        exit(1);
    }
    
};
