//
// Created by Adina Neda on 07/03/2020.
// Released under license: Apache-2.0 License
//

#ifndef PART2_PROJCODE_RUNSCHEDULINGANDSTATS_H
#define PART2_PROJCODE_RUNSCHEDULINGANDSTATS_H

#include <iostream>
#include <string>
#include <cmath>
#include <string>
#include <fstream>
#include "../OutputHandling/OutputStats/JobsOutputAnalysis.h"
#include "Paths.h"

using namespace std;

class RunSchedulingAndStats{
public:

    /**
     * Generate Tasks input file given the task properties
     * @param number_lines
     * @param max_arrival_time
     * @param max_number_operations
     * @param max_deadline_over_end_time
     * @param max_memory_requested
     * @param percentage_mb_to_gb
     * @return
     */
    static string GenerateInputFile(
            int number_lines,
            int max_arrival_time,
            unsigned long long int max_number_operations,
            int max_deadline_over_end_time,
            int max_memory_requested,
            float percentage_mb_to_gb
            );

    /**
     * Run all scheduling policies given an input file with tasks, file with available resources and a resource priority
     * @param input_file
     * @param resources_file_name
     * @param priority_resource
     */
    static void scheduling_all_policies(
            const string& input_file, const string& resources_file_name,
            const string& priority_resource
            );

     /**
      * Compute statistics about input file
      * @param input_file_name
      */
    static void run_statistics_input_file(const string& input_file_name);

    /**
     * Compute statistics about allocation output
     * @param path_eval_output_file_stats
     * @param input_file
     * @param priority
     * @param resources_file_name
     */
    static void output_stats_all_policies(
            const string& path_eval_output_file_stats,
            const string& input_file, const string& priority,
            const string& resources_file_name
            );

};

#endif //PART2_PROJCODE_RUNSCHEDULINGANDSTATS_H
