//
// Created by Adina Neda on 24/04/2020.
// Released under license: Apache-2.0 License
//

#include "RunOffline.h"
#include "../Core/ManageAvailableResources/ResourceQueues.h"
#include "../Core/SchedulingPolicies/Offline.h"
#include "Paths.h"
#include "../InputHandling/ParseAvailableResources/ParseAvailableResources.h"

/**
 * Run the offline scheduling policy
 * @param input_file - tasks input file name
 * @param available_resources_file - available resources file name
 * @param priority_resource - priority resource type; can be SW or CPU
 * @param out_scheduling_stats - file where statistics about the allocation are recorded
 */
void RunOffline::run_offline(
        const string& input_file, const string& available_resources_file,
        const string& priority_resource,
        ofstream& out_scheduling_stats
        ){

    // ResourceQueues class manages the CPU and SW queues of available resources
    ResourceQueues resource_queues;
    auto *offline = new Offline();

    // Parse resources
    ifstream in_resources_offline(available_resources_dir + available_resources_file + ".csv");
    ParseAvailableResources::read_available_resources(resource_queues, in_resources_offline);

    // OFFLINE SCHEDULING:
    // read all tasks in a priority queue (ordered by end time); schedule the tasks on CPU or SW
    cout << "Running Offline scheduling on: " << input_file << endl;

    auto start_time_offline = std::chrono::high_resolution_clock::now();
    offline->read_jobs_file(input_file);
    cout << "Finished reading jobs, starting the allocation!" << endl;

    offline->offline_scheduling(
            input_file, available_resources_file,
            resource_queues,priority_resource,out_scheduling_stats
            );

    auto end_time_offline_alloc = std::chrono::high_resolution_clock::now();
    auto duration_offline_alloc = std::chrono::duration_cast<std::chrono::seconds>(end_time_offline_alloc - start_time_offline).count();
    cout << "Duration (in seconds) of offline scheduling for tasks: " << duration_offline_alloc << endl << endl;

    out_scheduling_stats << "," << duration_offline_alloc << endl;

    delete offline;
}