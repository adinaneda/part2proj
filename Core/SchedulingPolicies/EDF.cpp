//
// Created by Adina Neda on 23/02/2020.
// Released under license: Apache-2.0 License
//

#include "EDF.h"



/**
 * Read tasks and attempt schedule them
 * @param tasks_input_file
 * @param available_resources_file
 * @param resource_queues
 * @param priority_resource
 * @param output_alloc_stats
 */
void EDF::read_and_schedule_task(
        const string& tasks_input_file,  const string&  available_resources_file,
        ResourceQueues& resource_queues,
        const string& priority_resource,
        ofstream& output_alloc_stats
        ) {
    string str;
    auto *event_queue = new EventQueue();

    ifstream in(tasks_dir + tasks_input_file + ".csv"); // Open taks input file
    // Open (or create) the output file for EDF allocation
    ofstream edf_output(
            edf_output_dir + "EDF" +
            tasks_input_file + "-" +
            available_resources_file + "-" + priority_resource + ".csv");

    edf_output << "# task_id" << ","
               << "arrival_time" << ","
               << "scheduled_time" << ","
               << "number_of_operations" << ","
               << "id_machine" << ","
               << "deadline" << ","
               << "end_time" << ","
               << "memory" << ","
               << "power" << endl;

    // Read line by line the file. Store tasks in order of arrival, then deadline
    while(!in.eof() && getline(in,str)) {
        // Find the first index character that's not an escape sequence
        string::size_type begin = str.find_first_not_of(" \f\t\v");
        //Skips blank lines and line starting with #
        if (begin == string::npos || string("#").find(str[begin]) != string::npos) continue;

        Task current_task = ParseTasks::parseTask(str); // parse the current task file line
        this->tasks_arrival_queue.push(current_task); // Push current task to deadline queue
    }

    edf_allocation(edf_output, event_queue, resource_queues, priority_resource);

    // Output stats for allocation
    StatsAllocation::print_stats_allocation(this->allocated, this->dropped, this->power);

    string sch_policy_name = "EDF";
    StatsAllocation::output_to_file_stats_allocation(
            this->allocated, this->dropped, this->power,
            output_alloc_stats, sch_policy_name
    );

    delete event_queue;
    edf_output.close();
}

/**
 * Allocate tasks on resource according to EDF policy
 * @param edf_output
 * @param event_queue
 * @param resource_queues
 * @param priority_resource
 */
void EDF::edf_allocation(
        ofstream &edf_output,
        EventQueue *event_queue, ResourceQueues &resource_queues,
        const string& priority_resource
) {

    while(!this->tasks_arrival_queue.empty()) {
        // Task we're trying to schedule is the one with earliest deadline
        Task current_task = this->tasks_arrival_queue.top();
        this->deadline_queue.push(current_task);
        Task task_first_deadline = this->deadline_queue.top();

        string decision =
                DecideResourcePriority::try_to_allocate(task_first_deadline, event_queue, resource_queues, edf_output,
                priority_resource,
                this->allocated, this->dropped, this->power); // prefer to allocate task asap.

        if(decision == "null") { // Task could not be allocated at arival time; check if we can "promise" it to a currently in use resource
            string machine_id =
                    event_queue->return_first_event_with_requirements_and_add_to_unallocated(
                            task_first_deadline, resource_queues);

            if(machine_id != "PastDeadline" && machine_id != "NoResourceWithRequirements") {
                DecideResourcePriority::calculate_end_time_and_allocate(
                        task_first_deadline, machine_id, event_queue, resource_queues, edf_output,
                        allocated, dropped, power);
            }else{
                AllocationDecision::drop(task_first_deadline, edf_output, dropped, machine_id);
            }
        }
        this->deadline_queue.pop();
        this->tasks_arrival_queue.pop();
    }
}