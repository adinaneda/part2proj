//
// Created by Adina Neda on 24/04/2020.
// Released under license: Apache-2.0 License
//

#include "RunFCFS.h"
#include "../InputHandling/ParseAvailableResources/ParseAvailableResources.h"
#include "../Core/ManageAvailableResources/ResourceQueues.h"
#include "../Core/SchedulingPolicies/FCFS.h"
#include "Paths.h"
#include "../Core/SchedulingPolicies/FCFSqueue.h"


/**
 * Run the FCFS without queueing scheduling policy
 * @param input_file - tasks input file name
 * @param available_resources_file - available resources file name
 * @param priority_resource - priority resource type; can be SW or CPU
 * @param out_scheduling_stats - file where statistics about the allocation are recorded
 */
void RunFCFS::run_fcfs(
        const string& input_file, const string& available_resources_file,
        const string& priority_resource,
        ofstream& out_scheduling_stats
        ){
    // ResourceQueues class manages the CPU and SW queues of available resources
    ResourceQueues resource_queues;
    auto *fcfs = new FCFS();

    //Parse resources (input config file)
    ifstream in_resources_fcfs(available_resources_dir + available_resources_file + ".csv");
    ParseAvailableResources::read_available_resources(resource_queues, in_resources_fcfs);

    // FCFS SCHEDULING (without batching):
    cout << "Running FCFS scheduling: " << input_file << endl;
    auto start_time_fcfs = std::chrono::high_resolution_clock::now();

    fcfs->read_and_schedule_task(
            input_file, available_resources_file,
            resource_queues, priority_resource,out_scheduling_stats
            );

    auto end_time_fcfs = std::chrono::high_resolution_clock::now();
    auto duration_fcfs = std::chrono::duration_cast<std::chrono::seconds>( end_time_fcfs - start_time_fcfs ).count();
    cout << "Duration (in seconds) of FCFS scheduling for tasks: " << duration_fcfs << endl << endl;

    out_scheduling_stats << "," << duration_fcfs << endl;

    delete fcfs;
}

/**
 * Run the FCFS with queueing scheduling policy
 * @param input_file - tasks input file name
 * @param available_resources_file - available resources file name
 * @param priority_resource - priority resource type; can be SW or CPU
 * @param out_scheduling_stats - file where statistics about the allocation are recorded
 */
void RunFCFS::run_fcfs_queue(
        const string& input_file, const string& available_resources_file,
        const string& priority_resource,
        ofstream& out_scheduling_stats
){
    // ResourceQueues class manages the CPU and SW queues of available resources
    ResourceQueues resource_queues;
    auto *fcfsQueue = new FCFSqueue();

    //Parse resources (input config file)
    ifstream in_resources_fcfs(available_resources_dir + available_resources_file + ".csv");
    ParseAvailableResources::read_available_resources(resource_queues, in_resources_fcfs);

    // FCFS SCHEDULING (without batching):
    cout << "Running FCFS (with queueing) scheduling: " << input_file << endl;
    auto start_time_fcfs = std::chrono::high_resolution_clock::now();

    fcfsQueue->read_and_schedule_task(
            input_file, available_resources_file,
            resource_queues, priority_resource,out_scheduling_stats
    );

    auto end_time_fcfs = std::chrono::high_resolution_clock::now();
    auto duration_fcfs = std::chrono::duration_cast<std::chrono::seconds>( end_time_fcfs - start_time_fcfs ).count();
    cout << "Duration (in seconds) of FCFS queue scheduling for tasks: " << duration_fcfs << endl << endl;

    out_scheduling_stats << "," << duration_fcfs << endl;

    delete fcfsQueue;
}