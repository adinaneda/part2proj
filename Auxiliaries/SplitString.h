//
// Created by Adina Neda on 30/03/2020.
// Released under license: Apache-2.0 License
//

#ifndef PART2_PROJCODE_SPLITSTRING_H
#define PART2_PROJCODE_SPLITSTRING_H

#include <iostream>
#include <string>
#include <vector>

using namespace std;

class SplitString {
public:
    /**
    * Split a string into a vector of tokens. Split by symbol "delim".
    * @param str - string to be split
    * @param delim - split by delim
    * @return
    */
    static vector<string> split(const string& str, const string& delim);
};


#endif //PART2_PROJCODE_SPLITSTRING_H
