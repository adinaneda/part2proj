//
// Created by Adina Neda on 22/02/2020.
// Released under license: Apache-2.0 License
//

#ifndef PART2_PROJCODE_RESOURCEQUEUES_H
#define PART2_PROJCODE_RESOURCEQUEUES_H

#include <iostream>
#include <string>
#include <filesystem>
#include <exception>
#include <fstream>
#include <vector>
#include <queue>

#include "../../Auxiliaries/SplitString.h"
#include "../../InputHandling/ParseTasks/ParseTasks.h"

using namespace std;
namespace fs = std::__fs::filesystem;

/**
 * A resource is a tuple of the form:
 * Type name: e.g. SW1, CPU2
 * Queue of IDs
 * Number of operations per second
 * Available memory
 * Power
 */
typedef tuple<string, priority_queue<int>, int, float, float> resource_type;

class ResourceQueues {
private:
    // A queue for each resource type
    vector<resource_type> type_SWITCH;
    vector<resource_type> type_CPU;
public:
    /**
     * Verify that queue for resources of type Switch is empty
     * @return
     */
    bool is_queue_sw_empty();

    /**
     * Verify that queue for resources of type CPU is empty
     * @return
     */
    bool is_queue_cpu_empty();

    /**
     * Returns the type of the resource
     * Type can be SW or CPU
     * @param resource_id
     * @return
     */
    string get_type(const string& resource_id);

    /**
     * Push the resource to the appropriate queue (depending on its type)
     * @param type_id - resource type e.g. SW1
     * @param count - indicates the number of resources of type type_id
     * @param operations_per_second - number of operations per second the resource can execute
     * @param memory - available memory of the resource
     * @param power - additional power in W used by the resource to execute an additional task
     */
    void push_resource(
            const string& type_id,
            int count,
            int operations_per_second,
            float memory,
            float power
            );

    /**
     * Pop resource with ID "id_machine" from appropriate queue
     * @param id_machine
     */
    void pop_resource(const string& id_machine);

    /**
     * Get power of resource with ID "id_machine"
     * @param id_machine
     * @return
     */
    float get_power(const string& id_machine);

    /**
     * Get memory of resource with ID "id_machine"
     * @param id_machine
     * @return
     */
    float get_memory(const string& id_machine);

    /**
     * Get number operations per second of resource with ID "id_machine"
     * @param id_machine
     * @return
     */
    float get_operations_per_second(const string& id_machine);

    /**
     * Find first machine which meets task requirements
     * Because resources are ordered by memory it will find the resource with the least memory
     * that's bigger than the requested memory by the task
     * @param resource_name
     * @param t - Task
     * @return
     */
    string get_first_id_machine_with_requirements (const string& resource_name, const Task& t);

    /**
     * Add resource back to type_SW or type_CPU depending on type
     * @param id_machine
     */
    void add_resource_to_unallocated(const string& id_machine);
};

#endif //PART2_PROJCODE_RESOURCEQUEUES_H
