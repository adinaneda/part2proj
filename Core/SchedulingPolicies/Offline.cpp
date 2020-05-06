//
// Created by Adina Neda on 23/02/2020.
// Released under license: Apache-2.0 License
//

#include "Offline.h"

/**
 * Read and parse all tasks in file "input_file"
 * @param input_file
 */
void Offline::read_jobs_file(const string& input_file) {
    ifstream in(tasks_dir + input_file + ".csv");
    string str;
    while(!in.eof() && getline(in, str)) {
        // Find the first index character that's not an escape sequence
        string::size_type begin = str.find_first_not_of(" \f\t\v");
        //Skip blank lines and lines starting with #
        if (begin == string::npos || string("#").find(str[begin]) != string::npos) continue;

        Task current_task = ParseTasks::parseTask(str);
        // Push task read to the list priority queue of all tasks
        this->pq_tasks.push(current_task);
    }
}

/**
 * Take scheduling decison by having complete knowledge of all the tasks
 * arriving in some time interval and optimising for memory
 * @param tasks_input_file - input file name
 * @param available_resources_file - file where all available resources are stored
 * @param resource_queues - queue of free resources of type CPU and switch
 * @param priority_resource - resource priority: SW or CPU
 * @param output_alloc_stats - file where allocation statistics are recorded
 */
void Offline::offline_scheduling(
        const string& tasks_input_file,  const string& available_resources_file,
        ResourceQueues& resource_queues,
        const string& priority_resource,
        ofstream& output_alloc_stats
        ) {
    // create event queue
    auto *event_queue = new EventQueue();

    // Open (or create) the output file for Offline allocation
    ofstream offline_output(
            offline_output_dir + "Offline" +
            tasks_input_file + "-" +
            available_resources_file + "-" + priority_resource + ".csv");

    offline_output  << "# task_id" << ","
                    << "arrival_time" << ","
                    << "scheduled_time" << ","
                    << "number_of_operations" << ","
                    << "id_machine" << ","
                    << "deadline" << ","
                    << "end_time" << ","
                    << "memory" << ","
                    << "power" << endl;

    // iterate over all tasks to allocate them
    while(!this->pq_tasks.empty()){
        // read current task
        Task current_task = this->pq_tasks.top();

        tuple<string, int, float> vect = event_queue->peek_best_event_with_requirements(
                current_task, resource_queues
        );

        string machine_id_event_queue = get<0>(vect);
        int index_position_event_queue = get<1>(vect);
        float end_time_machine_id_event_queue = get<2>(vect);

        DecideResourcePriority::try_to_allocate_on_best(
                machine_id_event_queue, end_time_machine_id_event_queue, index_position_event_queue,
                current_task,
                event_queue,resource_queues,offline_output,
                priority_resource,
                this->allocated,this->dropped,this->power
                );

        // pop task from priority queue
        this->pq_tasks.pop();
    }
    offline_output.close();
    StatsAllocation::print_stats_allocation(this->allocated, this->dropped, this->power);

    string sch_policy_name = "Offline";
    StatsAllocation::output_to_file_stats_allocation(
            this->allocated, this->dropped, this->power,
            output_alloc_stats, sch_policy_name
    );

    delete event_queue;
}