//
// Created by Adina Neda on 05/03/2020.
// Released under license: Apache-2.0 License
//

#include "AllocationDecision.h"

/**
* Output "allocate" decision to file
* @param t - Task getting allocated
* @param id_machine - ID machine where task is allocatdd
* @param power_used - Power used by the resource
* @param output_file - File where allocation decision is recorded
* @param end_time - End time of the task
* @param allocated - Count total tasks allocated
*/
void AllocationDecision::allocate(
        const Task &t,
        const string &id_machine,
        const float power_used,
        ofstream &output_file,
        const float &end_time,
        int &allocated
        ) {
    // increase count allocated tasks
    allocated++;
    // output the scheduling to file
    output_file << t.task_id << ","
                << t.arrival_time << ","
                << t.scheduled_time << ","
                << t.number_of_operations << ","
                << id_machine << ","
                << t.deadline << ","
                << end_time << ","
                << t.memory_requested << ","
                << power_used << endl;
}

/**
 * Output "drop" decision to file
 * @param t - Task getting dropped
 * @param output_file - File where allocation decision is recorded
 * @param dropped - Count total tasks allocated
 * @param reason_for_dropping - Why task is dropped
 */
void AllocationDecision::drop(const Task &t, ofstream &output_file, int &dropped, const string& reason_for_dropping) {
    // increase count dropped tasks
    dropped++;
    // output the scheduling to file
    output_file << t.task_id << ","
                << t.arrival_time << ","
                << t.scheduled_time << ","
                << t.number_of_operations << ","
                << reason_for_dropping << ","
                << t.deadline << ","
                << "null" << ","
                << t.memory_requested << ","
                << "null" << endl;
}