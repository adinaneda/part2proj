//
// Created by Adina Neda on 07/03/2020.
// Released under license: Apache-2.0 License
//

#ifndef PART2_PROJCODE_DECIDERESOURCEPRIORITY_H
#define PART2_PROJCODE_DECIDERESOURCEPRIORITY_H


#include <iostream>
#include <string>
#include <filesystem>
#include <iomanip>
#include <exception>
#include <fstream>
#include <vector>
#include <algorithm>
#include <queue>

#include "AllocationDecision.h"
#include "../../InputHandling/ParseTasks/ParseTasks.h"
#include "../ManageAvailableResources/ResourceQueues.h"
#include "../EventQueue/EventQueue.h"


using namespace std;
namespace fs = std::__fs::filesystem;

class DecideResourcePriority {
public:

    /**
     * Try to allocate the task t on the first resource that matches the task requirements and is available
     * If none is available retun "null"
     * @param t - task we're trying to allocate
     * @param event_queue - event queue
     * @param resource_queues - queue with the free available resources
     * @param output_file_name - file where allocation decision is recorded
     * @param priority_resource - which resource type has priority: switch or CPU
     * @param allocated - number of tasks allocated so far (increase if task gets allocated)
     * @param dropped - number of tasks dropped so far (increase if task gets dropped)
     * @param power - total power used so far
     * @return
     */
    static string try_to_allocate(
            Task& t, EventQueue* event_queue, ResourceQueues& resource_queues, ofstream& output_file_name,
            const string& priority_resource,
            int& allocated, int& dropped, float& power
    );

    /**
     * We try to allocate the task on the best resource (in terms of memory)
     * @param machine_still_in_use - machine that's still in use, but where we might be able to allocate Task t when it finishes its current execution
     * @param end_time_machine_still_in_use  - end time of machine still on event queue
     * @param index_in_event_queue_machine_still_in_use - index of the machine in the event queue
     * @param t - task we want to allocate
     * @param event_queue - the event queue
     * @param resource_queues - queue with the free available resources
     * @param output_file_name - file where allocation decision is recorded
     * @param priority_resource - which resource type has priority: switch or CPU
     * @param allocated - number of tasks allocated so far (increase if task gets allocated)
     * @param dropped - number of tasks dropped so far (increase if task gets dropped)
     * @param power - total power used so far
     */
    static void try_to_allocate_on_best(
            const string& machine_still_in_use, float end_time_machine_still_in_use, int index_in_event_queue_machine_still_in_use,
            Task& t,
            EventQueue* event_queue, ResourceQueues& resource_queues, ofstream& output_file_name,
            const string& priority_resource,
            int& allocated, int& dropped, float& power
    );


    /**
     * Decide which resource is better (in terms of memory) between "free_machine" and "machine_still_in_use"
     * @param current_task - task we attempt to allocate
     * @param free_machine - machine currently free
     * @param machine_still_in_use - machine that's still in use, but where we might be able to allocate Task t when it finishes its current execution
     * @param end_time_machine_still_in_use - end time of machine still on event queue
     * @param index_in_event_queue_machine_still_in_use - index of the machine in the event queue
     * @param resource_queues - queue with the free available resources
     * @param event_queue - the event queue
     * @param offline_output -  file where allocation decision is recorded
     * @param priority_resource - which resource type has priority: switch or CPU
     * @param allocated - number of tasks allocated so far (increase if task gets allocated)
     * @param dropped - number of tasks dropped so far (increase if task gets dropped)
     * @param power - total power used so far
     */
    static void allocate_on_best(
            Task current_task, const string& free_machine,
            const string& machine_still_in_use, float end_time_machine_still_in_use, int index_in_event_queue_machine_still_in_use,
            ResourceQueues& resource_queues, EventQueue *event_queue,
            ofstream& offline_output,
            const string& priority_resource,
            int& allocated, int& dropped, float& power
    );

    /**
     * Task t can be allocated on machine with ID "id_machine".
     * Record allocation decison on output file and increase the counter of tasks allocated and total power used.
     * @param t - tasks we allocate on resource
     * @param id_machine - machine where task is allocated
     * @param event_queue - the event queue
     * @param resource_queues - queue with the free available resources
     * @param output - file where allocation decision is recorded
     * @param allocated - number of tasks allocated so far (increase if task gets allocated)
     * @param dropped - number of tasks dropped so far
     * @param power - total power used so far
     */
    static void calculate_end_time_and_allocate(
            Task &t, const string& id_machine, EventQueue *event_queue, ResourceQueues &resource_queues, ofstream &output,
            int& allocated, int& dropped, float& power
            );
};

#endif //PART2_PROJCODE_DECIDERESOURCEPRIORITY_H
