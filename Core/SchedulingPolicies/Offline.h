//
// Created by Adina Neda on 23/02/2020.
// Released under license: Apache-2.0 License
//

#ifndef PART2_PROJCODE_OFFLINE_H
#define PART2_PROJCODE_OFFLINE_H

#include <iostream>
#include <string>
#include <filesystem>
#include <exception>
#include <fstream>
#include <vector>
#include <queue>
#include <map>

#include "../AllocationStats/StatsAllocation.h"
#include "../EventQueue/EventQueue.h"
#include "../../Tests/Paths.h"
#include "../../InputHandling/ParseTasks/ParseTasks.h"
#include "../ManageAvailableResources/ResourceQueues.h"
#include "../HelperFunctions/OrdersDefinition.h"
#include "../HelperFunctions/DecideResourcePriority.h"


using namespace std;
namespace fs = std::__fs::filesystem;

class Offline {
private:
    // Read all incoming tasks in a time interval and store them in a queue
    priority_queue<Task, vector<Task>, MinPriorityStartTimeEndTimeOrder> pq_tasks;
    // Keep track of the total number of tasks allocated and dropped and the total power used.
    int allocated = 0;
    int dropped = 0;
    float power = 0;
public:
    /**
     * Read and parse all tasks in file "input_file"
     * @param input_file
     */
    void read_jobs_file(const string& input_file);

    /**
     * Take scheduling decison by having complete knowledge of all the tasks
     * arriving in some time interval and optimising for memory
     * @param tasks_input_file - input file name
     * @param available_resources_file - file where all available resources are stored
     * @param resource_queues - queue of free resources of type CPU and switch
     * @param priority_resource - resource priority: SW or CPU
     * @param output_alloc_stats - file where allocation statistics are recorded
     */
    void offline_scheduling(
            const string& tasks_input_file,  const string& available_resources_file,
            ResourceQueues& resource_queues,
            const string& priority_resource,
            ofstream& output_alloc_stats
            );
};


#endif //PART2_PROJCODE_OFFLINE_H
