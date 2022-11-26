/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserver.errors.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zkasmi <zkasmi@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/25 19:28:19 by zkasmi            #+#    #+#             */
/*   Updated: 2022/11/25 19:28:31 by zkasmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <common.hpp>
#include <webserver.hpp>

int Webserver::error_page(string path)
{
    DIR *dir;
    string error_dir;
    dir = opendir(path.c_str());
    if (!dir)
        return 1;
    closedir(dir);
    return 0;
}