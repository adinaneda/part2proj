//
// Created by Adina Neda on 23/02/2020.
// Released under license: Apache-2.0 License
//

#ifndef PART2_PROJCODE_EDF_H
#define PART2_PROJCODE_EDF_H

#include <iostream>
#include <string>
#include <filesystem>
#include <exception>
#include <fstream>
#include <vector>
#include <queue>

#include "../ManageAvailableResources/ResourceQueues.h"
#include "../EventQueue/EventQueue.h"
#include "../../Tests/Paths.h"
#include "../../InputHandling/ParseTasks/ParseTasks.h"
#include "../AllocationStats/StatsAllocation.h"
#include "../HelperFunctions/OrdersDefinition.h"
#include "../HelperFunctions/DecideResourcePriority.h"

using namespace std;
namespace fs = std::__fs::filesystem;

class EDF {
private:
    // Read all tasks in "tasks_arrival_queue". Tasks are sorted by arrival time, then deadline
    priority_queue<Task, vector<Task>, MinPriorityStartTimeEndTimeOrder> tasks_arrival_queue;
    // store the tasks which can't be allocated at arrival time in a queue sorted by deadline
    priority_queue<Task, vector<Task>, DeadlineQueueOrder> deadline_queue;
    // Keep track of the total number of tasks allocated and dropped and the total power used.
    int dropped = 0;
    int allocated = 0;
    float power = 0;
public:
    /**
     * Read tasks and attempt schedule them
     * @param tasks_input_file
     * @param available_resources_file
     * @param resource_queues
     * @param priority_resource
     * @param output_alloc_stats
     */
    void read_and_schedule_task(
             const string& tasks_input_file,  const string& available_resources_file,
             ResourceQueues& resource_queues,
             const string& priority_resource,
             ofstream& output_alloc_stats
            );

    /**
     * Allocate tasks on resource according to EDF policy
     * @param edf_output
     * @param event_queue
     * @param resource_queues
     * @param priority_resource
     */
    void edf_allocation(
            ofstream& edf_output,
            EventQueue* event_queue, ResourceQueues& resource_queues,
            const string& priority_resource
            );
};

#endif //PART2_PROJCODE_EDF_H
