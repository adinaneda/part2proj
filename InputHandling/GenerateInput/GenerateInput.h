//
// Created by Adina Neda on 22/12/2019.
// Released under license: Apache-2.0 License
//

#ifndef TESTPROJ_SCRIPT_H
#define TESTPROJ_SCRIPT_H

#include <iostream>
#include <string>
#include <filesystem>
#include <fstream>
#include <cmath>
#include <random>
#include <algorithm>




using namespace std;
namespace fs = std::__fs::filesystem;

class GenerateInput {
public:
    /**
     * Generates pseudo-random input with constraints on values
     * @param output_file_name - file where tasks are written
     * @param SIZE - total number of lines in input file i.e. total number of tasks
     * @param max_arrival_time
     * @param max_number_operations
     * @param max_deadline_over_end_time
     * @param max_memory_requested
     * @param percentage_mb_to_gb - percentage of tasks which have an order of MB memory
     *                              (out of the total mnumber of tasks pseudo-randomly generated)
     */
    static void generate_input(
            const string& output_file_name,
            int SIZE,
            int max_arrival_time,
            unsigned long long int max_number_operations,
            int max_deadline_over_end_time,
            int max_memory_requested,
            float percentage_mb_to_gb
            );
};

#endif //TESTPROJ_SCRIPT_H
