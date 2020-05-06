//
// Created by Adina Neda on 22/02/2020.
// Released under license: Apache-2.0 License
//

#include "ResourceQueues.h"


// Resources in Resource Queue are sorted by memory, then number of operations per second
bool sort_by_memory_then_operations(
        const tuple<string, priority_queue<int>, int, float, float>& resource_1,
        const tuple<string, priority_queue<int>, int, float, float>& resource_2
){
    float resource_1_memory = get<4>(resource_1);
    float resource_2_memory = get<4>(resource_2);

    float resource_1_operations_per_sec = get<2>(resource_1);
    float resource_2_operations_per_sec= get<2>(resource_2);

    if( resource_1_memory == resource_2_memory ){
        return (resource_1_operations_per_sec > resource_2_operations_per_sec);
    }
    else return (resource_1_memory < resource_2_memory);
}

/**
 * Verify that queue for resources of type Switch is empty
 * @return
 */
bool ResourceQueues::is_queue_sw_empty() {
    for(int i = 0; i < this->type_SWITCH.size(); i++){
        //found at least one pq not empty
        if (!get<1>(this->type_SWITCH[i]).empty()) { return false;}
        else { continue; }
    }
    // iterated over all the switch types and there's no resource unallocated
    return true;
}

/**
 * Verify that queue for resources of type CPU is empty
 * @return
 */
bool ResourceQueues::is_queue_cpu_empty() {
    for(int i = 0; i < this->type_CPU.size(); i++){
        //found at least one pq not empty
        if (!get<1>(this->type_CPU[i]).empty()) { return false;}
        else { continue; }
    }
    // iterated over all the CPU types and there's no resource unallocated
    return true;
}

/**
 * Returns the type of the resource
 * Type can be SW or CPU
 * @param resource_id
 * @return
 */
string ResourceQueues::get_type(const string &resource_id) {
    size_t found_SW = resource_id.find("SW");
    size_t found_CPU = resource_id.find("CPU");

    if(found_SW == 0) { return "SW"; }
    else if (found_CPU == 0) { return "CPU"; }
    else return "Unknown type";
}

/**
 * Push the resource to the appropriate queue (depending on its type)
 * @param type_id - resource type e.g. SW1
 * @param count - indicates the number of resources of type type_id
 * @param operations_per_second - number of operations per second the resource can execute
 * @param memory - available memory of the resource
 * @param power - additional power in W used by the resource to execute an additional task
 */
void ResourceQueues::push_resource(
        const string &type_id, int count,
        int operations_per_second, float memory, float power) {

    priority_queue<int> pq;
    for(int i = 0; i < count; i++){pq.push(i);}
    try {
        if(get_type(type_id) == "SW" && !pq.empty()) {
            this->type_SWITCH.push_back(make_tuple(type_id, pq, operations_per_second, memory, power));
            sort(this->type_SWITCH.begin(), this->type_SWITCH.end(), sort_by_memory_then_operations);
        }
        else if(get_type(type_id) == "CPU" && !pq.empty()) {
            this->type_CPU.push_back(make_tuple(type_id, pq, operations_per_second, memory, power));
            sort(this->type_CPU.begin(), this->type_CPU.end(), sort_by_memory_then_operations);
        }
    }catch(exception &e){
        cout << "Error in push_resource() due to unknown type" << endl;
    }
}

/**
 * Pop resource with ID "id_machine" from appropriate queue
 * @param id_machine
 */
void ResourceQueues::pop_resource(const string& id_machine) {
    try{
        vector<string> tokens = SplitString::split(id_machine, "_");
        string id_type = tokens.at(0);

        if(get_type(id_machine) == "SW"){
            for (int i = 0; i < this->type_SWITCH.size(); i++){
                string current_id_type =  get<0>(this->type_SWITCH[i]);
                // for now we're popping the top of pq; would be better to pop exactly index from pq
                if(id_type == current_id_type) { get<1>(this->type_SWITCH[i]).pop(); return; }
            }
        }
        else if (get_type(id_machine) == "CPU"){
            for (int i = 0; i < this->type_CPU.size(); i++){
                string current_id_type = get<0>(this->type_CPU[i]);
                // for now we're popping the top of pq; would be better to pop exactly index from pq
                if(id_type == current_id_type)  {get<1>(this->type_CPU[i]).pop(); return; }
            }
        }
    }catch(exception &e){
        cout << "Attempting to pop resource from empty queue" << endl;
    }
}

/**
 * Get power of resource with ID "id_machine"
 * @param id_machine
 * @return
 */
float ResourceQueues::get_power(const string &id_machine) {
    try{
        vector<string> tokens = SplitString::split(id_machine, "_");
        string id_type = tokens.at(0);

        if(get_type(id_machine) == "SW"){
            for (int i = 0; i < this->type_SWITCH.size(); i++){
                string current_id_type =  get<0>(this->type_SWITCH[i]);
                if(id_type == current_id_type) { return get<4>(this->type_SWITCH[i]); }
            }
        }
        else if (get_type(id_machine) == "CPU"){
            for (int i = 0; i < this->type_CPU.size(); i++){
                string current_id_type = get<0>(this->type_CPU[i]);
                if(id_type == current_id_type) { return get<4>(this->type_CPU[i]); }
            }
        }
    }catch(exception &e){
        cout << "Exception in get_power()" << endl;
    }
    return -1;
}

/**
 * Get memory of resource with ID "id_machine"
 * @param id_machine
 * @return
 */
float ResourceQueues::get_memory(const string &id_machine) {
    try{
        vector<string> tokens = SplitString::split(id_machine, "_");
        string id_type = tokens.at(0);

        if(get_type(id_machine) == "SW"){
            for (int i = 0; i < this->type_SWITCH.size(); i++){
                string current_id_type =  get<0>(this->type_SWITCH[i]);
                if(id_type == current_id_type) { return get<3>(this->type_SWITCH[i]); }
            }
        }
        else if (get_type(id_machine) == "CPU"){
            for (int i = 0; i < this->type_CPU.size(); i++){
                string current_id_type = get<0>(this->type_CPU[i]);
                if(id_type == current_id_type) { return get<3>(this->type_CPU[i]); }
            }
        }
    }catch(exception &e){
        cout << "Exception in get_memory()" << endl;
    }
    return -1;
}

/**
 * Get number operations per second of resource with ID "id_machine"
 * @param id_machine
 * @return
 */
float ResourceQueues::get_operations_per_second(const string &id_machine) {
    try {
        vector<string> tokens =  SplitString::split(id_machine, "_");
        string id_type = tokens.at(0);
        int index = stoi(tokens.at(1));

        if(get_type(id_type) == "SW"){
            for (int i = 0; i < (this->type_SWITCH).size(); i++){
                string current_id_type =  get<0>(this->type_SWITCH[i]);
                if(id_type == current_id_type){ return get<2>(this->type_SWITCH[i]); }
            }
        } else if(get_type(id_type) == "CPU"){
            for (int i = 0; i < (this->type_CPU).size(); i++){
                string current_id_type = get<0>(this->type_CPU[i]);
                if(id_type == current_id_type){ return get<2>(this->type_CPU[i]); }
            }
        }
    }catch(exception &e){
        cout << "Exception in get_operations_per_second()" << endl;
    }
    return -1;
}

/**
 * Find first machine which meets task requirements
 * Because resources are ordered by memory it will find the resource with the least memory
 * that's bigger than the requested memory by the task
 * @param resource_name
 * @param t - Task
 * @return
 */
string
ResourceQueues::get_first_id_machine_with_requirements(const string &resource_name, const Task& t) {
    try{
        float memory = t.memory_requested;

        if(resource_name == "SW"){
            for(int i = 0; i <  this->type_SWITCH.size(); i++){
                tuple<string, priority_queue<int>, int, float, float > current_element = this->type_SWITCH[i];
                // task with any no. ops. can be run, it would just take longer
                // only constraint is memory
                float current_element_memory = get<3>(current_element);
                // matches requirements && pq not empty
                if(
                        !get<1>(current_element).empty() &&
                        current_element_memory >= memory
                        ){
                    string type_id = get<0>(this->type_SWITCH[i]);
                    int front_index = get<1>(this->type_SWITCH[i]).top();

                    return type_id + "_" + to_string(front_index);
                }
            }
            return "null"; // No free resource found
        }
        else if(resource_name == "CPU") {
            for(int i = 0; i <  this->type_CPU.size(); i++){
                tuple<string, priority_queue<int>, int, float, float > current_element = this->type_CPU[i];
                float current_element_memory = get<3>(current_element);

                // matches all the requirements && pq not empty
                if(
                        !get<1>(current_element).empty() &&
                        current_element_memory >= memory
                        ){
                    string type_id = get<0>(this->type_CPU[i]);
                    int front_index = get<1>(this->type_CPU[i]).top();
                    return type_id + "_" + to_string(front_index);
                }
            }
            return "null"; // No free resource found
        }
        else return "Type ID unknown";
    }catch (exception &e){
        cout << "err" << endl;
        return "Exception in get_first_id_machine_with_requirements() function \n";
    }
}

/**
 * Add resource back to type_SW or type_CPU depending on type
 * @param id_machine
 */
void ResourceQueues::add_resource_to_unallocated(const string& id_machine) {

    vector<string> tokens =  SplitString::split(id_machine, "_");
    string id_type = tokens.at(0);
    int index = stoi(tokens.at(1));

    if(get_type(id_machine) == "SW"){
        for (int i = 0; i < this->type_SWITCH.size(); i++){
            // found the entry in the type SW with the ID resource_type_id
            if(id_type == get<0>(this->type_SWITCH[i])) {
                // push the index back
                get<1>(this->type_SWITCH[i]).push(index);
                return;
            }
        }
    }
    else if(get_type(id_machine) == "CPU"){
        for (int i = 0; i < this->type_CPU.size(); i++){
            // found the entry in the type SW with the ID resource_type_id
            if(id_type == get<0>(this->type_CPU[i])) {
                // push the index back
                get<1>(this->type_CPU[i]).push(index);
                return;
            }
        }
    }
    else{
        cout << "Error in add_resource_to_unallocated()" << endl;
    }
}

