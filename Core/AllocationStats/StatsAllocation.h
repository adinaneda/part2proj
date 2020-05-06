//
// Created by Adina Neda on 05/03/2020.
// Released under license: Apache-2.0 License
//

#ifndef PART2_PROJCODE_STATSALLOCATION_H
#define PART2_PROJCODE_STATSALLOCATION_H

#include <iostream>
#include <string>
#include <filesystem>
#include <exception>
#include <fstream>

using namespace std;
namespace fs = std::__fs::filesystem;

class StatsAllocation {
public:
    /**
     * Output to console number of tasks dropped, allocated and percentage of tasks allocated.
     * @param allocated - number of tasks allocated
     * @param dropped - number of tasks dropped
     * @param power - power in W used
     */
    static void print_stats_allocation(int &allocated, int&dropped, float& power);

     /**
      * Output to file stats about the allocation.
      * @param allocated - number of tasks allocated
      * @param dropped - number of tasks dropped
      * @param power - power in W used
      * @param out - output file
      * @param sch_policy - name of the scheduling policy
      */
    static void output_to_file_stats_allocation(
            int &allocated, int&dropped, float& power,
            ofstream& out, string& sch_policy);
};

#endif //PART2_PROJCODE_STATSALLOCATION_H
