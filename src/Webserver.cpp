/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserver.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zkasmi <zkasmi@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/09 14:16:41 by ren-nasr          #+#    #+#             */
/*   Updated: 2022/11/17 17:19:53 by zkasmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/common.hpp"
#include "../includes/webserver.hpp"


// Constructors 
Webserver::Webserver(config_t server_vals, locations_t locations_vals, mime_t mime_types) {
    _server_data = server_vals;
    _location_data = locations_vals;
    _mime_types = mime_types;
};

// Webserver::Webserver(vector<Webserver> servers) {
//     _servers = servers;
// };

// void Webserver::listen() {
    
//     // listen stuff here...   
// }