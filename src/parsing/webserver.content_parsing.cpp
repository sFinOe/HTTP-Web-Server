/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserver.content_parsing.cpp                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zkasmi <zkasmi@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/10 22:28:36 by zkasmi            #+#    #+#             */
/*   Updated: 2022/11/21 16:51:52 by zkasmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <common.hpp>
#include <webserver.hpp>

void Webserver::root_block(locations_t &_locations, config_t _server_data)
{
    for (config_it data_it = _server_data.begin(); data_it != _server_data.end(); data_it++) {
        if (data_it->first == "root"){
            for (locations_it locs_it = _locations.begin(); locs_it != _locations.end(); locs_it++){
                map<string, string> data = locs_it->second;
                if (data.find("root") == data.end() && data.find("return_301") == data.end()){
                    data.insert(make_pair(data_it->first, data_it->second));
                    locs_it->second = data;
                }
            }
        }
    }
}

map<string, map<string, string> > Webserver::parse_locations(string &line, locations_t _locations)
{
    regmatch_t index;
    regex_t regex;
    string sub_location, tmp;
    mime_t location;
    vector<string> data;
    // v_it it;
    // vector<string>::iterator it;
    size_t ret = 0, start = 0, end = 0, _key_len, _value_len;
    while (line.find("location", end) != string::npos) {
        // loop for location and substr from location to } ==>   location {...........} <== and i start the parsing
        start = line.find("location", end);
        end = start;
        while (end < line.length()) {
            // looking for and of brackets of location
            if (line[end] == '}') {
                end++;
                break;
            }
            end++;
        }
        tmp = line.substr(start, end - start);
        ///// regex parsing ////////// i grab what insed the brackets of location as looks like ez right??????? lets find out
        regcomp(&regex, "\\{[^}]*\\}", REG_EXTENDED);
        ret = regexec(&regex, tmp.c_str(), 1, &index, 0);
        if (ret == 0) {
            // here i parse what inside every location brackets and inset them a map;
            tmp = trim(tmp.substr(index.rm_so, index.rm_eo - index.rm_so), "{} \t\v");
            // i split on ; each data of a location exmple:
            // {root /var/www; index index.hml;} ==> data[0] = root /var/www //// data[1] = index index.html
            data = split(tmp, ";");
            for (vector<string>::iterator it = data.begin(); it != data.end(); it++) {
                // loop on every vector and insert key and value ==> key = root, value = /var/www;
                *it = trim((*it), " \t\v\f");
                _key_len = key_len(*it);
                _value_len = value_len(*it, _key_len);
                if (duplicate_location_data((*it).substr(0, _key_len), location)) {
                    config_t data;
                    location.clear();
                    exit_error(data, _locations, (*it).substr(0, _key_len), PATH_RCS_ERR);
                }
                location.insert(make_pair((*it).substr(0, _key_len), (*it).substr(_value_len, (*it).length())));
            }
            tmp = line.substr(start, end - start);
            data.clear();
            // here i where i get the path of location ===> location '/image' so i can create map of maps for each path !!!
            // regex d zob
            regcomp(&regex, "[location]+.*\\{", REG_EXTENDED);
            ret = regexec(&regex, tmp.c_str(), 1, &index, 0);
            if (ret == 0) {
                tmp.erase(0, 9);
                // here we i trim earse location so i get only path ==> /image !!!!!
                tmp = trim(tmp.substr(index.rm_so, index.rm_eo - index.rm_so - 9), " {\t\v");
                if (path_duplicate(tmp, _locations)) {
                    config_t data;
                    exit_error(data, _locations, tmp, DUP_PATH);
                }
                // here i insert the path and the map that i created earlier
                _locations.insert(make_pair(tmp, location));
                location.clear();
            }
            regfree(&regex);
        }
        // i earse every location {...} that i parse and loop for next location and so on until i have no location left
        line.erase(start, end - start);
        end = 0; // to back to first element bc i earse the location ki t5war liya index that's why
    }
    // here where i finished the location parsing and earse every location from string so i have only server data ===> listen, host .... blabla
    return _locations;
}

multimap<string, string> Webserver::parse_server_data(string &line, config_t _server_data)
{
    regex_t regex;
    regmatch_t index;
    string tmp;
    vector<string> data;
    size_t ret = 0,_key_len, _value_len;
    // here is same as location parsing i use regex pattern to grab data inside brackets of server ==>  {listen 8080; host 0.0.0.0 .....}
    regcomp(&regex, "\\{[^}]*\\}", REG_EXTENDED);
    ret = regexec(&regex, line.c_str(), 1, &index, 0);
    if (ret == 0) {
        // itrim the brackets ==> listen 8080; host 0.0.0.0 ....
        if (empty_brackets(line.substr(index.rm_so, index.rm_eo - index.rm_so)))
            cerr << "error : empty brackets in server block" << endl, exit(1);
        tmp = trim(line.substr(index.rm_so, index.rm_eo - index.rm_so), "{} \t\v");
        // split on ";" become ==> vector data[0] = listen 8080, data[1] = host 0.0.0.0
        data = split(tmp, ";");
        // i loop and vector elements and grab value and key ===> key = listen, value = 8080
        for (vector<string>::iterator it = data.begin(); it != data.end(); it++) {
            *it = trim((*it), " \t\v\f");
            _key_len = key_len(*it);
            _value_len = value_len(*it, _key_len);
            _server_data.insert(make_pair((*it).substr(0, _key_len), (*it).substr(_value_len, (*it).length())));
        }
        data.clear();
        regfree(&regex);
        // done server parsing go to next server and so on
    }
    return _server_data;
}