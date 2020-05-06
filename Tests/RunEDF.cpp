//
// Created by Adina Neda on 24/04/2020.
// Released under license: Apache-2.0 License
//

#include "RunEDF.h"
#include "../Core/ManageAvailableResources/ResourceQueues.h"
#include "../Core/SchedulingPolicies/EDF.h"
#include "../InputHandling/ParseAvailableResources/ParseAvailableResources.h"
#include "Paths.h"

/**
 * Run the EDF scheduling policy and output stats about the scheduling
 * @param input_file - tasks input file name
 * @param available_resources_file - available resources file name
 * @param priority_resource - priority resource type; can be SW or CPU
 * @param out_scheduling_stats - file where statistics about the allocation are recorded
 */
void RunEDF::run_edf(
        const string& input_file, const string& available_resources_file,
        const string& priority_resource,
        ofstream& out_scheduling_stats
){

    // ResourceQueues class manages the CPU and SW queues of available resources
    ResourceQueues resource_queues;
    auto *edf = new EDF();
    // Open file given as input

    ifstream in_resources_edf(available_resources_dir + available_resources_file + ".csv");
    ParseAvailableResources::read_available_resources(resource_queues, in_resources_edf);

    // EDF SCHEDULING:
    // read all tasks in a priority queue (ordered by end time); schedule the tasks on CPU or SW
    cout << "Running EDF scheduling: " << input_file << endl;
    auto start_time_edf = std::chrono::high_resolution_clock::now();

    edf->read_and_schedule_task(
            input_file, available_resources_file,
            resource_queues, priority_resource, out_scheduling_stats
            );

    auto end_time_edf = std::chrono::high_resolution_clock::now();
    auto duration_edf = std::chrono::duration_cast<std::chrono::seconds>(end_time_edf - start_time_edf).count();
    cout << "Duration (in seconds) of EDF scheduling for tasks: " << duration_edf << endl;

    out_scheduling_stats << "," << duration_edf << endl;

    delete edf;
}