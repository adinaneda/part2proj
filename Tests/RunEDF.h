//
// Created by Adina Neda on 24/04/2020.
// Released under license: Apache-2.0 License
//

#ifndef PART2_PROJCODE_RUNEDF_H
#define PART2_PROJCODE_RUNEDF_H

#include <string>

using  namespace std;

class RunEDF {
public:
    /**
     * Run the EDF scheduling policy and output stats about the scheduling
     * @param input_file - tasks input file name
     * @param available_resources_file - available resources file name
     * @param priority_resource - priority resource type; can be SW or CPU
     * @param out_scheduling_stats - file where statistics about the allocation are recorded
     */
    static void run_edf(
            const string& input_file, const string& available_resources_file,
            const string& priority_resource,
            ofstream& out_scheduling_stats
    );

};


#endif //PART2_PROJCODE_RUNEDF_H
