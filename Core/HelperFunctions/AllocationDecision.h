//
// Created by Adina Neda on 05/03/2020.
// Released under license: Apache-2.0 License
//

#ifndef PART2_PROJCODE_ALLOCATIONDECISION_H
#define PART2_PROJCODE_ALLOCATIONDECISION_H

#include <iostream>
#include <string>
#include <filesystem>
#include <fstream>

#include "../../InputHandling/ParseTasks/ParseTasks.h"

using namespace std;
namespace fs = std::__fs::filesystem;

class AllocationDecision {
public:
    /**
     * Output "allocate" decision to file
     * @param t - Task getting allocated
     * @param id_machine - ID machine where task is allocatdd
     * @param power_used - Power used by the resource
     * @param output_file - File where allocation decision is recorded
     * @param end_time - End time of the task
     * @param allocated - Count total tasks allocated
     */
    static void allocate(
            const Task &t,
            const string &id_machine,
            const float power_used,
            ofstream &output_file,
            const float &end_time,
            int &allocated
            );

    /**
     * Output "drop" decision to file
     * @param t - Task getting dropped
     * @param output_file - File where allocation decision is recorded
     * @param dropped - Count total tasks allocated
     * @param reason_for_dropping - Why task is dropped
     */
    static void drop(const Task &t, ofstream &output_file, int &dropped, const string& reason_for_dropping);
};


#endif //PART2_PROJCODE_ALLOCATIONDECISION_H
