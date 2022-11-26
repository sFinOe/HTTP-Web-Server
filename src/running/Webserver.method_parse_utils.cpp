/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserver.method_parse_utils.cpp                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zkasmi <zkasmi@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/17 19:52:17 by zkasmi            #+#    #+#             */
/*   Updated: 2022/11/25 19:30:07 by zkasmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <common.hpp>
#include <webserver.hpp>


string Webserver::count_content_length(string root)
{
	fstream file(root.c_str(), fstream::in);
	string str;
	stringstream buffer, len;
	buffer << file.rdbuf();
	file.close();
	len << buffer.str().length();
	return "Content-Length: " + len.str() + "\r\n";
}

string Webserver::multimap_value(multimap<string, string> data, string key)
{
    for(multimap<string, string>::iterator it = data.begin(); it != data.end(); it++){
        if (it->first == key)
            return it->second;
    }
    return "";
}
