//
// Created by Adina Neda on 24/04/2020.
// Released under license: Apache-2.0 License
//

#ifndef PART2_PROJCODE_RUNFCFS_H
#define PART2_PROJCODE_RUNFCFS_H

#include <iostream>
#include <string>

using  namespace std;

class RunFCFS {
public:
    /**
     * Run the FCFS without queueing scheduling policy
     * @param input_file - tasks input file name
     * @param available_resources_file - available resources file name
     * @param priority_resource - priority resource type; can be SW or CPU
     * @param out_scheduling_stats - file where statistics about the allocation are recorded
     */
    static void run_fcfs(
            const string& input_file, const string& available_resources_file,
            const string& priority_resource,
            ofstream& out_scheduling_stats
    );

    /**
     * Run the FCFS with queueing scheduling policy
     * @param input_file - tasks input file name
     * @param available_resources_file - available resources file name
     * @param priority_resource - priority resource type; can be SW or CPU
     * @param out_scheduling_stats - file where statistics about the allocation are recorded
     */
    static void run_fcfs_queue(
            const string& input_file, const string& available_resources_file,
            const string& priority_resource,
            ofstream& out_scheduling_stats
    );
};

#endif //PART2_PROJCODE_RUNFCFS_H
