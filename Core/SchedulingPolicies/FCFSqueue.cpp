//
// Created by Adina Neda on 23/04/2020.
// Released under license: Apache-2.0 License
//

#include "FCFSqueue.h"

/**
 * Read task and schedule
 * @param tasks_input_file - input file where tasks are stored
 * @param available_resources_file - file where all available resources are stored
 * @param resource_queues - queue of free resources of type CPU and switch
 * @param priority_resource - resource priority: SW or CPU
 * @param output_alloc_stats - file where allocation statistics are recorded
 */
void FCFSqueue::read_and_schedule_task(
        const string& tasks_input_file,  const string&  available_resources_file,
        ResourceQueues& resource_queues,
        const string& priority_resource,
        ofstream& output_alloc_stats
        ) {

    string str;
    auto *event_queue = new EventQueue();

    ifstream in(tasks_dir + tasks_input_file + ".csv"); // Open taks input file
    // Open (or create) the output file for FCFS allocation
    ofstream fcfs_output(
            fcfs_output_dir + "FCFSqueue" +
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

    string sch_policy_name = "FCFSqueue";
    StatsAllocation::output_to_file_stats_allocation(
            this->allocated, this->dropped, this->power,
            output_alloc_stats, sch_policy_name
    );

    delete event_queue;

}

/**
 * Decide if task can be allocated or has to be dropped
 * according to the FCFS schedulling algorithm (with queueing)
 * @param fcfs_output - file where we store the scheduling decisions
 * @param current_task - task we need to schedule
 * @param event_queue - queue of events
 * @param resource_queues - queue of free resources of type CPU and switch
 * @param priority_resource - resource priority: SW or CPU
 */
void
FCFSqueue::fcfs_allocation(
        ofstream &fcfs_output,
        Task &new_task,
        EventQueue *event_queue, ResourceQueues &resource_queues,
        const string &priority_resource
        ) {

    // Free resources running tasks that finished
    while(!event_queue->isEmpty() && event_queue->getEndTimeTop() <= new_task.arrival_time){
        FreeResources::free_resources_from_eventqueue(event_queue, resource_queues);
    }

    this->wait_queue.push(new_task);

    Task task_first_deadline = this->wait_queue.front(); // select the task

    // allocate based on which resource has priority
    string decision = DecideResourcePriority::try_to_allocate( task_first_deadline, event_queue, resource_queues, fcfs_output,
                                                               priority_resource,
                                                               this->allocated, this->dropped, this->power);

    if(decision == "null")  { // Task could not be allocated at arrival time
        // Check if we can promise it on a now in use resource
        string machine_id =
                event_queue->return_first_event_with_requirements_and_add_to_unallocated(
                        task_first_deadline, resource_queues);

        if(machine_id != "PastDeadline" && machine_id != "NoResourceWithRequirements") {
            DecideResourcePriority::calculate_end_time_and_allocate(
                    task_first_deadline, machine_id, event_queue, resource_queues, fcfs_output,
                    allocated, dropped, power);
        }else{
            AllocationDecision::drop(task_first_deadline, fcfs_output, dropped, machine_id);
        }
    }
    this->wait_queue.pop();
}
