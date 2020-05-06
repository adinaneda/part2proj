//
// Created by Adina Neda on 23/04/2020.
// Released under license: Apache-2.0 License
//

#ifndef PART2_PROJCODE_PERFORMANCETESTS_H
#define PART2_PROJCODE_PERFORMANCETESTS_H

#include <iostream>

using namespace std;

class PerformanceTests {
public:
    /**
     * Generate file with percentage of tasks with and order of MB memory from 0.1 to 0.9 in 0.1 increments
     * Run scheduling with 3 resources files with ~10K resources:
     * only CPUs, 10K CPUs and switches combined and 10K CPUs + 250 SWs
     * @param priority
     */
    static void generate_file_run_percentage_tasks_mb_test(const string& priority);

    /**
     * Given an input file, run all 3 scheduling policies on it (FCFS queue, offline, EDF),
     * using 3 available resources files with ~10K resources:
     * only CPUs, 10K CPUs and switches combined and 10K CPUs + 250 SWs
     * for both the SW and CPU priorities
     * @param input_file
     */
    static void run_allocation_both_priorities(const string& input_file);

    /**
     * Generate new input file for each arrival rate between 10K-100K tasks per second (in increments of 10K)
     * Run scheduling for all 3 scheduling policies
     * Run scheduling with 3 resources files with ~10K resources:
     * only CPUs, 10K CPUs and switches combined and 10K CPUs + 250 SWs
     * @param priority
     */
    static void generate_file_run_incoming_tasks_rate_vs_power(const string& priority);
};


#endif //PART2_PROJCODE_PERFORMANCETESTS_H
