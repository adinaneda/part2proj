//
// Created by Adina Neda on 29/03/2020.
// Released under license: Apache-2.0 License
//

#ifndef PART2_PROJCODE_PARSETASKS_H
#define PART2_PROJCODE_PARSETASKS_H

#include <iostream>
#include <string>
#include <vector>


using namespace std;

struct Task{
    string task_id = "";
    float arrival_time = 0.0;
    float number_of_operations = 0.0;
    float deadline = 0.0;
    float scheduled_time = 0.0;
    float memory_requested = 0.0;
};

class ParseTasks {
public:
    /**
     * Parse task given a file line
     * @param current_file_line
     * @return
     */
    static Task parseTask (const string& current_file_line);
};

#endif //PART2_PROJCODE_PARSETASKS_H
