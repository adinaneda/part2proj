//
// Created by Adina Neda on 29/03/2020.
// Released under license: Apache-2.0 License
//

#ifndef PART2_PROJCODE_JOBSOUTPUTANALYSIS_H
#define PART2_PROJCODE_JOBSOUTPUTANALYSIS_H

#include <iostream>
#include <climits>
#include <filesystem>
#include <string>
#include <fstream>
#include <map>

using namespace std;

class JobsOutputAnalysis {
public:
    /**
     * Output statistics about the allocation from a file recording the allocation decisions
     * @param allocation_output
     * @param output_file_results
     */
    static void output_statistics(
            ifstream& allocation_output,
            ofstream& output_file_results
            );

    /**
     * Output to file time and current power used every time there's a change
     * i.e. resource gets allocated or freed
     * @param path_to_output_file_from_allocation
     * @param path_to_output_result
     */
    static void power_over_time(
            const string& path_to_output_file_from_allocation,
            const string& path_to_output_result
    );
};


#endif //PART2_PROJCODE_JOBSOUTPUTANALYSIS_H
