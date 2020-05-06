//
// Created by Adina Neda on 05/03/2020.
// Released under license: Apache-2.0 License
//

#include "StatsAllocation.h"

/**
 * Output to console number of tasks dropped, allocated and percentage of tasks allocated.
 * @param allocated - number of tasks allocated
 * @param dropped - number of tasks dropped
 * @param power - power in W used
 */
void StatsAllocation::print_stats_allocation(int &allocated, int&dropped, float& power) {
    int total = dropped + allocated;

    cout << "Number jobs allocated, dropped, total: " << allocated << ", " << dropped << ", " << total << endl;

    cout    << "Percentage of jobs allocated, dropped: "
            << (float) allocated / (float) total * 100 << ", "
            << (float) dropped / (float) total * 100 << endl;

    cout << "Total power consumption: " << power << endl;

}

/**
  * Output to file stats about the allocation.
  * @param allocated - number of tasks allocated
  * @param dropped - number of tasks dropped
  * @param power - power in W used
  * @param out - output file
  * @param sch_policy - name of the scheduling policy
  */
void StatsAllocation::output_to_file_stats_allocation(
        int &allocated, int&dropped, float& power,
        ofstream& out, string& sch_policy
        ) {
    int total = dropped + allocated;
    out
        << sch_policy << ","
        << allocated << "," << dropped << "," << total << ","
        << (float) allocated / (float) total * 100 << "," << (float) dropped / (float) total * 100 << ","
        << power;
}
