//
// Created by Adina Neda on 22/02/2020.
// Released under license: Apache-2.0 License
//

#ifndef PART2_PROJCODE_FCFS_H
#define PART2_PROJCODE_FCFS_H

#include <iostream>
#include <string>
#include <filesystem>
#include <exception>
#include <fstream>
#include <vector>
#include <algorithm>
#include <sstream>
#include <string>
#include <functional>
#include <queue>

#include "../../InputHandling/ParseTasks/ParseTasks.h"
#include "../../Tests/Paths.h"
#include "../AllocationStats/StatsAllocation.h"
#include "../ManageAvailableResources/ResourceQueues.h"
#include "../EventQueue/EventQueue.h"
#include "../HelperFunctions/FreeResources.h"
#include "../HelperFunctions/AllocationDecision.h"
#include "../HelperFunctions/DecideResourcePriority.h"

using namespace std;
namespace fs = std::__fs::filesystem;

class FCFS {
private:
    // Keep track of the total number of tasks allocated and dropped and the total power used.
    int dropped = 0;
    int allocated = 0;
    float power = 0;
public:
     /**
      * Read the file and schedule current task
      * @param tasks_input_file - input file where tasks are stored
      * @param available_resources_file - file where all available resources are stored
      * @param resource_queues - queue of free resources of type CPU and switch
      * @param priority_resource - resource priority: SW or CPU
      * @param output_alloc_stats - file where allocation statistics are recorded
      */
     void read_and_schedule_task(
            const string&  tasks_input_file, const string&  available_resources_file,
            ResourceQueues& resource_queues,
            const string& priority_resource,
            ofstream& output_alloc_stats
            );

     /**
      * Decide if task can be allocated or has to be dropped
      * @param out - file where we store the scheduling decisions
      * @param current_task - task we need to schedule
      * @param event_queue - queue of events
      * @param resource_queues - queue of free resources of type CPU and switch
      * @param priority_resource - resource priority: SW or CPU
      */
     void fcfs_allocation(
             ofstream& out, Task &current_task,
             EventQueue *event_queue, ResourceQueues& resource_queues,
             const string& priority_resource
             );

};


#endif //PART2_PROJCODE_FCFS_H
