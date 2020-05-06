//
// Created by Adina Neda on 30/03/2020.
// Released under license: Apache-2.0 License
//

#include "SplitString.h"

/**
* Split a string into a vector of tokens. Split by symbol "delim".
* @param str - string to be split
* @param delim - split by delim
* @return
*/
vector<string> SplitString::split(const string& str, const string& delim){
    vector<string> tokens;
    size_t prev = 0, pos = 0;
    while(pos < str.length() && prev < str.length()){
        pos = str.find(delim, prev);
        if (pos == string::npos) pos = str.length();
        string token = str.substr(prev, pos-prev);
        if (!token.empty()) tokens.push_back(token);
        prev = pos + delim.length();
    }
    return tokens;
}