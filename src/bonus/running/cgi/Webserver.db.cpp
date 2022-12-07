/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserver.db.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zkasmi <zkasmi@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/02 13:31:50 by zkasmi            #+#    #+#             */
/*   Updated: 2022/12/07 13:42:32 by zkasmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <common_bonus.hpp>
#include <webserver_bonus.hpp>

void	Webserver::fill_login_db(vector<post_body> body)
{
	fstream file("/tmp/login.db", fstream::out | fstream::app);
	
	for (vector<post_body>::iterator it = body.begin(); it != body.end(); it++){
		if (it->key.find("signup") != string::npos){
			file << "signup : " << it->value << endl;
			file << "-----------------------------------------" << endl;	
		}
	}
	file.close();
}

bool	Webserver::valid_login(string login)
{
	fstream file("/tmp/login.db", fstream::in);
	string line;
	
	login = trim(login, " \r\n");
	while (getline(file, line)){
		if (line.find(login) != string::npos)
			return true;
	}
	file.close();
	return false;
}
