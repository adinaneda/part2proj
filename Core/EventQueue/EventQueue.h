//
// Created by Adina Neda on 15/12/2019.
// Released under license: Apache-2.0 License
//

#ifndef TESTPROJ_EVENTQUEUE_H
#define TESTPROJ_EVENTQUEUE_H

#include <iostream>
#include <string>
#include <filesystem>
#include <exception>
#include <fstream>
#include <vector>
#include <queue>

#include "../../InputHandling/ParseTasks/ParseTasks.h"
#include "../ManageAvailableResources/ResourceQueues.h"

using namespace std;
namespace fs = std::__fs::filesystem;

/**
 * An Event is a tuple of the form
 * Task ID e.g. 0, 1, 2, ...
 * Resource ID e.g. SW1_0, CPU2_1, ...
 * Memory of resource e.g 16 (MB)
 * Power used by resource 300 (W)
 * End Time e.g. 3, 100, 1.2, ...
 */
typedef tuple<string, string, float, float, float> event;

class EventQueue {
private:
    // Vector of Events
    // Vector is sorted by the end time!
    vector<event> pq;
public:
    /**
     * Return the size of the EventQueue
     * @return
     */
    int sizeEventQueue();

    /**
     * Check if EventQueue is empty or not
     * @return
     */
    bool isEmpty();

    /**
     * Get event at the top of the EventQueue i.e the event with the earliest end time
     * @return
     */
    event getEventTop();

    /**
     * Get the Task ID for the event at the top of the EventQueue
     * @return
     */
    string getTaskIDTop();

    /**
     * Get the Resource ID for the event at the top of the EventQueue
     * Resource ID is of the form SWx_y or CPUx_y
     * @return
     */
    string getResourceIDTop();

    /**
     * Get the Memory for the Resource ID for the event at the top of the EventQueue
     * @return
     */
    float getResourceMemoryTop();

    /**
     * Get the Power for the Resource ID for the event at the top of the EventQueue
     * @return
     */
    float getResourcePowerTop();

    /**
     * Get the end time for the event at the top of the EventQueue
     * i.e the first end time
     * @return
     */
    float getEndTimeTop();

    /**
     * Add an event to the EventQueue
     * @param task_id - the ID of the task to be added to the Event Queue
     * @param resource_id - the ID of the resource where task was allocated
     * @param memory - resource's memory
     * @param power - power used
     * @param end_time - end time of the task
     */
    void pushEvent(
            const string& task_id,
            const string& resource_id, const float& memory, const float& power,
            float end_time
            );

    /**
     * Pop an event from the EventQueue
     */
    void popEvent();

    /**
     * Get Resource Type from Resource ID
     * @param resource_id e.g. SW1_3 or CPU1_4
     * @return e.g. SW1 or CPU1
     */
    static string getTypeIdFromResourceID(const string& resource_id);

    /**
     * Get Resource Index from Resource ID
     * @param resource_id e.g. SW1_3 or CPU1_4
     * @return e.g. 3 or 4
     */
    static int getResourceIndexFromResourceID(const string& resource_id);

    /**
     * Return first resource which matches task requirements and unallocate event
     * @param t - Task
     * @param resource_queues - queue with available resources
     * @return
     */
    string return_first_event_with_requirements_and_add_to_unallocated(Task& t, ResourceQueues& resource_queues);

    /**
     * Return the best resource which matches task requirements
     * @param t - Task
     * @param resource_queues - queue with available resources
     * @return
     */
    tuple<string, int, float> peek_best_event_with_requirements(Task& t, ResourceQueues& resource_queues);

    /**
     * Delete Event at index
     * @param index
     */
    void erase_value_at_index(int index);
};


#endif //TESTPROJ_EVENTQUEUE_H
