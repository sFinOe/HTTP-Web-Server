/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   common.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zkasmi <zkasmi@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/09 12:10:42 by ren-nasr          #+#    #+#             */
/*   Updated: 2022/11/14 19:07:04 by zkasmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// here add any common header files and typedef's
// header files which are used in many places (like iosream, string...);

#ifndef COMMON_HPP
#define COMMON_HPP


/***** define of parsing error *****/

#define KEY_ERR 1
#define VALUE_ERR 2
#define DUP_PATH 3
#define EMPTY_PATH 4
#define L_KEY_ERR 5
#define L_VALUE_ERR 6
#define PATH_RCS_ERR 7
#define DUP_DATA_ERR 8
#define NO_DATA 9

/***** define of server *****/

#define SERVER_TIMEOUT 15

#include <iostream>
#include <unistd.h>
#include <fstream>
#include <regex.h>
#include <vector>
#include <map>
#include <utility>
#include <stdlib.h>
#include <algorithm>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/poll.h>
#include <dirent.h>
#include <cstring>
#include <sys/time.h>
#include <csignal>
#include <sstream>


using std::multimap;
using std::map;
using std::make_pair;
using std::string;
using std::vector;
using std::cout;
using std::endl;
using std::fstream;
using std::pair;
using std::cerr;
using std::atoi;
using std::isalnum;
using std::isdigit;
using std::stringstream;



#endif