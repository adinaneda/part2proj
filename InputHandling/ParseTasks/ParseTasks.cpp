//
// Created by Adina Neda on 29/03/2020.
// Released under license: Apache-2.0 License
//

#include "ParseTasks.h"
#include "../../Auxiliaries/SplitString.h"

/**
 * Parse task given a file line
 * @param current_file_line
 * @return
 */
Task ParseTasks::parseTask(const string& current_file_line) {
    // Split line into tokens
    vector<string> tokens = SplitString::split(current_file_line, ",");
    try {

        Task t =  {
                tokens.at(0),
                stof(tokens.at(1)),
                stof(tokens.at(2)),
                stof(tokens.at(3)),
                stof(tokens.at(1)),
                stof(tokens.at(4))
        };

        return t;

    } catch (exception &e) {
        cout << "Malformed input line when Parsing Task" << endl;
        return Task();
    }

}

