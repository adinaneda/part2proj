//
// Created by Adina Neda on 22/02/2020.
// Released under license: Apache-2.0 License
//

#include "FCFS.h"

/**
  * Read the file and schedule current task
  * @param tasks_input_file - input file where tasks are stored
  * @param available_resources_file - file where all available resources are stored
  * @param resource_queues - queue of free resources of type CPU and switch
  * @param priority_resource - resource priority: SW or CPU
  * @param output_alloc_stats - file where allocation decision is recorded
  */
void FCFS::read_and_schedule_task(
        const string& tasks_input_file,  const string&  available_resources_file,
        ResourceQueues& resource_queues,
        const string& priority_resource,
        ofstream& output_alloc_stats
        ){
    string str;
    auto *event_queue = new EventQueue();

    ifstream in(tasks_dir + tasks_input_file + ".csv"); // Open taks input file
    // Open (or create) the output file for FIFO allocation
    ofstream fcfs_output(
            fcfs_output_dir + "FCFS" +
            tasks_input_file + "-" +
            available_resources_file + "-" + priority_resource + ".csv");

    fcfs_output  << "# task_id" << ","
                    << "arrival_time" << ","
                    << "scheduled_time" << ","
                    << "number_of_operations" << ","
                    << "id_machine" << ","
                    << "deadline" << ","
                    << "end_time" << ","
                    << "memory" << ","
                    << "power" << endl;

    /**
     * Read line by line the file i.e one task at a time
     * We don't store the lines anywhere, we make decisions at each line without
     * knowing anything about the future tasks
     */
    while(!in.eof() && getline(in,str)) {
        // Find the first index character that's not an escape sequence
        string::size_type begin = str.find_first_not_of(" \f\t\v");
        // Skip blank lines and lines starting with #
        if (begin == string::npos || string("#").find(str[begin]) != string::npos) continue;
        // Parse and allocate current task
        Task current_task = ParseTasks::parseTask(str);

        fcfs_allocation(fcfs_output, current_task, event_queue, resource_queues, priority_resource);
    }

    fcfs_output.close();

    // Output allocation statistics
    StatsAllocation::print_stats_allocation(this->allocated, this->dropped, this->power);

    string sch_policy_name = "FCFS";
    StatsAllocation::output_to_file_stats_allocation(
            this->allocated, this->dropped, this->power,
            output_alloc_stats, sch_policy_name
            );

    delete event_queue;
}

/**
 * Decide if task can be allocated or has to be dropped
 * @param out - file where we store the scheduling decisions
 * @param current_task
 * @param event_queue - queue of events
 * @param resource_queues - queue of free resources of type CPU and switch
 * @param priority_resource - resource priority: SW or CPU
 */
void FCFS::fcfs_allocation(
        ofstream& fcfs_output,
        Task &current_task,
        EventQueue *event_queue, ResourceQueues& resource_queues,
        const string& priority_resource
        ) {

    // Free resources running tasks that finished
    while(!event_queue->isEmpty() && event_queue->getEndTimeTop() <= current_task.arrival_time){
        FreeResources::free_resources_from_eventqueue(event_queue, resource_queues);
    }

    // No resource is free. Hence, drop task.
    if(resource_queues.is_queue_sw_empty() && resource_queues.is_queue_cpu_empty()){
        AllocationDecision::drop(current_task, fcfs_output, this->dropped, "NoFreeResources");
    }
    // If resources are available
    else{
        // allocate based on which resource has priority
        string decision = DecideResourcePriority::try_to_allocate( current_task, event_queue, resource_queues, fcfs_output,
                                                 priority_resource,
                                                 this->allocated, this->dropped, this->power);
        if(decision == "null"){
            AllocationDecision::drop(current_task, fcfs_output, dropped, "MemoryRequirements");
        }
    }

}