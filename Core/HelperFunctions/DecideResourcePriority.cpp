//
// Created by Adina Neda on 07/03/2020.
// Released under license: Apache-2.0 License
//

#include "DecideResourcePriority.h"


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
string DecideResourcePriority::try_to_allocate(
        Task& t, EventQueue* event_queue, ResourceQueues& resource_queues, ofstream& output_file_name,
        const string& priority_resource,
        int& allocated, int& dropped, float& power
){
    string id_machine_sw = resource_queues.get_first_id_machine_with_requirements("SW", t);
    string id_machine_cpu = resource_queues.get_first_id_machine_with_requirements("CPU", t);

    string machine_id_priority_1 = priority_resource == "SW" ? id_machine_sw : id_machine_cpu;
    string machine_id_priority_2 = priority_resource == "SW" ? id_machine_cpu : id_machine_sw;

    // Can allocate the task on machine with id "machine_id_priority_1"
    if(machine_id_priority_1 != "null"){
        calculate_end_time_and_allocate(
                t, machine_id_priority_1, event_queue, resource_queues, output_file_name,
                allocated, dropped, power
        );
        return machine_id_priority_1;
    }
    // Can allocate the task on machine with id "machine_id_priority_2"
    else if(machine_id_priority_2 != "null"){
        calculate_end_time_and_allocate(
                t, machine_id_priority_2, event_queue, resource_queues, output_file_name,
                allocated, dropped, power
        );
        return machine_id_priority_2;
    }
    // No resources that match the task requirements are free
    else{
        return "null";
    }
}


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
void DecideResourcePriority::try_to_allocate_on_best(
        const string& machine_still_in_use, float end_time_machine_still_in_use, int index_in_event_queue_machine_still_in_use,
        Task& t, EventQueue* event_queue, ResourceQueues& resource_queues, ofstream& output_file_name,
        const string& priority_resource,
        int& allocated, int& dropped, float& power
){
    string id_machine_sw = resource_queues.get_first_id_machine_with_requirements("SW", t);
    string id_machine_cpu = resource_queues.get_first_id_machine_with_requirements("CPU", t);

    string machine_id_priority_1 = priority_resource == "SW" ? id_machine_sw : id_machine_cpu;
    string machine_id_priority_2 = priority_resource == "SW" ? id_machine_cpu : id_machine_sw;

    // No machines in use finish before the task deadline and there are no free resources
    // We know for sure the task will be dropped
    if(machine_still_in_use == "null" && id_machine_sw == "null" && id_machine_cpu == "null"){
        AllocationDecision::drop(t, output_file_name, dropped, "NoFreeResources");
    }
    // Can allocate task either on "machine_id_priority_1" or on "machine_still_in_use" (when it finishes its current execution)
    else if(machine_id_priority_1 != "null"){
        DecideResourcePriority::allocate_on_best(
                t, machine_id_priority_1, machine_still_in_use,
                end_time_machine_still_in_use, index_in_event_queue_machine_still_in_use,
                resource_queues, event_queue,output_file_name,
                priority_resource,allocated,dropped,power
                );
    }
    // Can allocate task either on "machine_id_priority_2" or on "machine_still_in_use" (when it finishes its current execution)
    else if(machine_id_priority_2 != "null"){
        DecideResourcePriority::allocate_on_best(
                t, machine_id_priority_2, machine_still_in_use,
                end_time_machine_still_in_use, index_in_event_queue_machine_still_in_use,
                resource_queues, event_queue,output_file_name,
                priority_resource,allocated,dropped,power
                );
    }
    // We can only allocate the task on "machine_still_in_use" when it finishes its current execution
    else{
        if( machine_still_in_use != "null" ){
            resource_queues.add_resource_to_unallocated(machine_still_in_use);
            t.scheduled_time = end_time_machine_still_in_use < t.arrival_time ? t.arrival_time : end_time_machine_still_in_use;
            event_queue->erase_value_at_index(index_in_event_queue_machine_still_in_use);

            DecideResourcePriority::calculate_end_time_and_allocate(
                    t, machine_still_in_use, event_queue, resource_queues, output_file_name,
                    allocated, dropped, power
            );
        }
    }
}

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
void DecideResourcePriority::allocate_on_best(
        Task current_task, const string& free_machine,
        const string& machine_still_in_use, float end_time_machine_still_in_use, int index_in_event_queue_machine_still_in_use,
        ResourceQueues& resource_queues, EventQueue *event_queue,
        ofstream& offline_output,
        const string& priority_resource,
        int& allocated, int& dropped, float& power
){

    if(machine_still_in_use != "null"){
        if(resource_queues.get_memory(machine_still_in_use) == resource_queues.get_memory(free_machine) &&
                machine_still_in_use.find(priority_resource) == 0){
            string resource_max_num_ops_per_sec =
                    resource_queues.get_operations_per_second(machine_still_in_use) < resource_queues.get_operations_per_second(free_machine) ?
                    free_machine : machine_still_in_use;

            if(resource_max_num_ops_per_sec == machine_still_in_use){ // if in use machine was chosen then free the machine first
                resource_queues.add_resource_to_unallocated(machine_still_in_use);
                current_task.scheduled_time = end_time_machine_still_in_use < current_task.arrival_time ? current_task.arrival_time : end_time_machine_still_in_use;
                event_queue->erase_value_at_index(index_in_event_queue_machine_still_in_use);
            }

            DecideResourcePriority::calculate_end_time_and_allocate(
                    current_task, resource_max_num_ops_per_sec, event_queue, resource_queues, offline_output,
                    allocated, dropped, power
            );
        }
        else if(
                resource_queues.get_memory(machine_still_in_use) < resource_queues.get_memory(free_machine) &&
                        machine_still_in_use.find(priority_resource) == 0
                ){
            resource_queues.add_resource_to_unallocated(machine_still_in_use);
            current_task.scheduled_time = end_time_machine_still_in_use < current_task.arrival_time ? current_task.arrival_time : end_time_machine_still_in_use;
            event_queue->erase_value_at_index(index_in_event_queue_machine_still_in_use);

            DecideResourcePriority::calculate_end_time_and_allocate(
                    current_task, machine_still_in_use, event_queue, resource_queues, offline_output,
                    allocated, dropped, power
            );
        }else{
            DecideResourcePriority::calculate_end_time_and_allocate(
                    current_task, free_machine, event_queue, resource_queues, offline_output,
                    allocated, dropped, power
            );
        }
    }
    else{
        DecideResourcePriority::calculate_end_time_and_allocate(
                current_task, free_machine, event_queue, resource_queues, offline_output,
                allocated, dropped, power
        );
    }
}

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
void DecideResourcePriority::calculate_end_time_and_allocate(
        Task &t, const string& id_machine, EventQueue *event_queue, ResourceQueues &resource_queues, ofstream &output,
        int& allocated, int& dropped, float& power
        ) {

    string task_id = t.task_id;
    float arrival_time = t.arrival_time;
    float scheduled_time = t.scheduled_time;
    float number_of_operations = t.number_of_operations;
    float deadline = t.deadline;

    // Calculate end time
    float end_time = scheduled_time + (float) number_of_operations / resource_queues.get_operations_per_second(id_machine);

    // Increase the power and memory used
    float power_used = resource_queues.get_power(id_machine);
    power += power_used;
    float memory_used = resource_queues.get_memory(id_machine);

    // can allocate event on id_machine; add it to the event queue
    event_queue->pushEvent(
            task_id,
            id_machine, memory_used, power_used,
            end_time
            );

    AllocationDecision::allocate(t, id_machine, power_used, output, end_time, allocated);

    // pop SW/CPU resource from queue of unallocated resources
    resource_queues.pop_resource(id_machine);
}