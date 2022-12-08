/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserver.free.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zkasmi <zkasmi@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/06 02:16:56 by zkasmi            #+#    #+#             */
/*   Updated: 2022/12/07 15:52:32 by zkasmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <common.hpp>
#include <webserver.hpp>

void Webserver::free_client()
{
	if (_request){
		if (_request->method == GET){
			get_parse *g_parse = (get_parse*)_request->parsed_method;
			delete g_parse;
			delete _request;
		}
		else if (_request->method == POST){
			post_parse *p_parse = (post_parse*)_request->parsed_method;
			p_parse->body.clear();
			delete p_parse;
			delete _request;
		}
		else if (_request->method == DELETE){
			delete_parse *d_parse = (delete_parse*)_request->parsed_method;
			delete d_parse;
			delete _request;
		}
	}
	if(_response)
		delete _response;
}
