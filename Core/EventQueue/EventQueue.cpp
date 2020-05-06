//
// Created by Adina Neda on 15/12/2019.
// Released under license: Apache-2.0 License
//

#include "EventQueue.h"

// Events on the queue are sorted by the end time, then memory
bool sort_by_custom(
        const tuple<string, string, float, float, float>& event_1,
        const tuple<string, string, float, float, float>& event_2
){
    float event_1_end_time = get<4>(event_1);
    float event_2_end_time = get<4>(event_2);

    float event_1_memory = get<2>(event_1);
    float event_2_memory = get<2>(event_2);

    if( event_1_end_time == event_2_end_time ){
        return (event_1_memory > event_2_memory);
    }
    else return (event_1_end_time > event_2_end_time);
}

/**
 * Return the size of the EventQueue
 * @return
 */
int EventQueue::sizeEventQueue() {
    return (this->pq).size();
}

/**
* Check if EventQueue is empty or not
* @return
*/
bool EventQueue::isEmpty() {
    return (this->pq).empty();
}

/**
 * Get event at the top of the EventQueue i.e the event with the earliest end time
 * @return
 */
event EventQueue::getEventTop() {
    try{
        if(!isEmpty()){return (this->pq)[sizeEventQueue() - 1];}
    }catch (exception &e){
        cout << "Trying to return Event at the top of EventQueue, but EventQueue is empty!" << endl;
    }
    return event();
}

/**
 * Get the Task ID for the event at the top of the EventQueue
 * @return
 */
string EventQueue::getTaskIDTop() {
    if(!isEmpty()){return get<0>(this->pq[sizeEventQueue() - 1]);}
    else{ return "Trying to getTaskIDTop(), but Event Queue is empty!"; }
}

/**
 * Get the Resource ID for the event at the top of the EventQueue
 * Resource ID is of the form SWx_y or CPUx_y
 * @return
 */
string EventQueue::getResourceIDTop() {
    try{
        if(!isEmpty()){return get<1>(this->pq[sizeEventQueue() - 1]);}
    }catch(exception &e){
        return "Trying to getResourceIDTop(), but Event Queue is empty!";
    }
    return "Trying to getResourceIDTop(), but Event Queue is empty!";
}

/**
 * Get the Memory for the Resource ID for the event at the top of the EventQueue
 * @return
 */
float EventQueue::getResourceMemoryTop() {
    // < resource ID where task was allocated, memory, power >
    try{
        if(!isEmpty()){return get<2>(this->pq[sizeEventQueue() - 1]);}
    }catch(exception &e){
        cout << "Trying to getResourceMemoryTop(), but Event Queue is empty!" << endl;
    }
    return -1;
}

/**
 * Get the Power for the Resource ID for the event at the top of the EventQueue
 * @return
 */
float EventQueue::getResourcePowerTop() {
    // < resource ID where task was allocated, memory, power >
    try{
        if(!isEmpty()){return get<3>(this->pq[sizeEventQueue() - 1]);}
    }catch(exception &e){
        cout << "Trying to getResourcePowerTop(), but Event Queue is empty!" << endl;
    }
    return -1;
}

/**
 * Get the end time for the event at the top of the EventQueue
 * i.e the first end time
 * @return
 */
float EventQueue::getEndTimeTop() {
    try{
        if(!isEmpty()){return get<4>(this->pq[sizeEventQueue() - 1]);}
    }catch(exception &e){
        cout << "Trying to getEndTimeTop(), but Event Queue is empty!" << endl;
    }
    return -1;
}

/**
 * Insert an event at position such that Event Queue is still sorted by end time then memory
 * @param vector_events
 * @param new_event
 */

void insert( vector<event> &vector_events, const event& new_event ) {
    auto it = lower_bound( vector_events.begin(), vector_events.end(), new_event, sort_by_custom); // find position
    vector_events.insert( it, new_event ); // insert before iterator
}

/**
 * Add an event to the EventQueue
 * @param task_id - the ID of the task to be added to the Event Queue
 * @param resource_id - the ID of the resource where task was allocated
 * @param memory - resource's memory
 * @param power - power used
 * @param end_time - end time of the task
 */
void EventQueue::pushEvent(
        const string& task_id,
        const string& resource_id, const float& memory, const float& power,
        float end_time) {
    tuple<string, string, float, float, float> elem_to_insert = make_tuple(task_id, resource_id, memory, power, end_time);
    insert(this->pq, elem_to_insert);
}

/**
 * Pop an event from the EventQueue
 */
void EventQueue::popEvent() {
    try{
        (this->pq).pop_back();
    }catch(exception &e){
        cout << "Trying to pop from empty queue" << endl;
    }
}

/**
 * Get Resource Type from Resource ID
 * @param resource_id e.g. SW1_3 or CPU1_4
 * @return e.g. SW1 or CPU1
 */
string EventQueue::getTypeIdFromResourceID(const string& resource_id) {
    return SplitString::split(resource_id, "_").at(0);
}

/**
 * Get Resource Index from Resource ID
 * @param resource_id e.g. SW1_3 or CPU1_4
 * @return e.g. 3 or 4
 */
int EventQueue::getResourceIndexFromResourceID(const string& resource_id) {
    return stoi(SplitString::split(resource_id, "_").at(1));
}

/**
 * Return first resource which matches task requirements and unallocate event
 * @param t - Task
 * @param resource_queues - queue with available resources
 * @return
 */
string EventQueue::return_first_event_with_requirements_and_add_to_unallocated(
        Task& t,
        ResourceQueues& resource_queues
) {

    // priority_queue <event, vector<event>, OrderEndTime> event_queue_copy;
    const float arrival_time = t.arrival_time;
    const float deadline = t.deadline;
    const float memory_requested = t.memory_requested;

    try{
        if(!this->pq.empty()){
            // for each event in the EventQueue; starting from the one with the earliest end time
            for(int i = this->pq.size()-1; i >= 0; i--){
                event current_top_event = this->pq[i];
                string top_event_resource_id = get<1>(current_top_event);
                float top_event_memory = get<2>(current_top_event);
                float top_event_end_time = get<4>(current_top_event);

                if(top_event_end_time > deadline) {
                     return "PastDeadline";
                }
                else if ( top_event_memory >= memory_requested ){
                    resource_queues.add_resource_to_unallocated(top_event_resource_id);
                    t.scheduled_time = top_event_end_time < t.arrival_time ? t.arrival_time : top_event_end_time;
                    this->pq.erase(this->pq.begin() + i);
                    return top_event_resource_id;
                }
            }
        }
    }catch(exception &e){
        cout << "Exception in pop_and_return_first_event_with_requirements()" << endl;
    }
    return "NoResourceWithRequirements";
}

/**
 * Return the best resource which matches task requirements
 * @param t - Task
 * @param resource_queues - queue with available resources
 * @return
 */
tuple<string, int, float> EventQueue::peek_best_event_with_requirements(
        Task& t,
        ResourceQueues& resource_queues
) {
    tuple<string, int, float> result;

    // priority_queue <event, vector<event>, OrderEndTime> event_queue_copy;
    const float arrival_time = t.arrival_time;
    const float deadline = t.deadline;
    const float memory_requested = t.memory_requested;

    try{
        unsigned long index_where_found = -1;
        string resource_id = "null";
        float end_time = -1;
        float memory = INT_MAX;

        if(!this->pq.empty()){ // if Event Queue not empty, iterate over events in increasing order of end time
            for(int i = this->pq.size()-1; i >= 0; i--){
                event current_top_event = this->pq[i];
                string top_event_resource_id = get<1>(current_top_event);
                float top_event_memory = get<2>(current_top_event);
                float top_event_end_time = get<4>(current_top_event);

                if(top_event_end_time > deadline) { break; }
                else if (
                        top_event_memory >= memory_requested  // finishes before task's deadline && has enough memory
                        && memory > top_event_memory){ // it's the smallest value so far
                    index_where_found = i;
                    resource_id = top_event_resource_id;
                    end_time = top_event_end_time;
                    memory = top_event_memory;
                }
            }
        }

        result = make_tuple(resource_id, index_where_found, end_time);
    }catch(exception &e){
        cout << "Exception in pop_and_return_first_event_with_requirements()" << endl;
    }
    return result;
}

/**
 * Delete Event at index
 * @param index
 */
void EventQueue::erase_value_at_index(int index) {
    this->pq.erase(this->pq.begin() + index);
}
