//
// Created by Adina Neda on 29/03/2020.
// Released under license: Apache-2.0 License
//

#include "JobsOutputAnalysis.h"
#include "../../Auxiliaries/SplitString.h"

/**
* Output statistics about the allocation from a file recording the allocation decisions
* @param allocation_output
* @param output_file_results
*/
void JobsOutputAnalysis::output_statistics(
        ifstream& allocation_output,
        ofstream& output_file_results
        ){

    int total_number_tasks_dropped = 0;
    int total_number_tasks_allocated = 0;
    int number_dropped_due_to_memory = 0;
    int number_dropped_due_to_no_resources_free = 0;
    int number_dropped_past_deadline = 0;

    float sum_running_time = 0;
    float max_running_time = INT_MIN;
    float min_running_time = INT_MAX;
    float avg_running_time = 0.0;

    float sum_waiting_time = 0;
    float max_waiting_time = INT_MIN;
    float min_waiting_time = INT_MAX;
    float avg_waiting_time = 0.0;

    float total_power_used = 0.0;
    float total_memory_requested = 0.0;

    float min_arrival_time = INT_MAX;
    float max_arrival_time = INT_MIN;
    float max_end_time = INT_MIN;

    map<string, map<string, int>> mymap; // <SW1, <SW1_1, 120>>
    map<float, float> map_energy_time;

    string str;
    while(!allocation_output.eof() && getline(allocation_output,str)) {
        // Find the first index character that's not an escape sequence
        string::size_type begin = str.find_first_not_of(" \f\t\v");
        //Skips blank lines and line starting with #
        if (begin == string::npos || string("#").find(str[begin]) != string::npos) continue;

        vector<string> tokens =  SplitString::split(str, ",");

        // Get values for current task
        string task_id = tokens.at(0);
        float arrival_time = stof(tokens.at(1));
        float scheduled_time = stof(tokens.at(2));
        float number_of_operations = stof(tokens.at(3));
        string id_machine = tokens.at(4);
        float deadline = stof(tokens.at(5));
        float end_time;
        float memory_requested = stof(tokens.at(7));
        float power_used;
        try {
            end_time = stof(tokens.at(6));
            power_used = stof(tokens.at(8));
        }catch (exception&e){
            end_time = -1;
            power_used = 0.0;
        }

//        cout << "Task ID: " <<  task_id << endl;
//        cout << "Arrival time: " <<  arrival_time << endl;
//        cout << "Scheduled time: " <<  scheduled_time << endl;
//        cout << "No. ops: " <<  number_of_operations << endl;
//        cout << "Machine ID: " <<  id_machine << endl;
//        cout << "Deadline: " <<  deadline << endl;
//        cout << "End time: " <<  end_time << endl;

        if (arrival_time < min_arrival_time) {min_arrival_time = arrival_time;}
        if (arrival_time > max_arrival_time) {max_arrival_time = arrival_time;}
        if (end_time > 0){ // if task is allocated on a machine
            total_number_tasks_allocated++; // Increase count for allocated

            float current_task_running_time = end_time - scheduled_time;
            sum_running_time += current_task_running_time;
            if(current_task_running_time < min_running_time) { min_running_time = current_task_running_time; }
            if(current_task_running_time > max_running_time) { max_running_time = current_task_running_time; }

            float current_task_waiting_time = scheduled_time - arrival_time;
            sum_waiting_time += current_task_waiting_time;
            if(current_task_waiting_time < min_waiting_time) { min_waiting_time = current_task_waiting_time; }
            if(current_task_waiting_time > max_waiting_time) { max_waiting_time = current_task_waiting_time; }

            total_power_used += power_used;
            total_memory_requested += memory_requested;

            if (end_time > max_end_time) {max_end_time = end_time;}
        }
        else{ total_number_tasks_dropped++; } // Increase count for dropped
    }

    /**
    * Calculate:
    * 1. total number of tasks dropped due to: power, no free resources, past deadline
    * 2. min, max, avg running time
    * 3. total running time
    * 4. Ratio: (tasks_arrived / max_arrival_time) * (sum_runtime / tasks_arrived) / total_number_of_resources
    * 5. min, max, avg waiting time i.e. scheduled - arrival
    * 6.
    */

    long total_number_of_tasks = total_number_tasks_allocated + total_number_tasks_dropped;
    float simulation_time = max_end_time - min_arrival_time;
    output_file_results
            << total_number_tasks_dropped << "," << total_number_tasks_allocated << ","
            << total_number_of_tasks << ","
            << min_running_time << "," << max_running_time << "," << (float) sum_running_time / total_number_tasks_allocated << ","
            << min_waiting_time << "," << max_waiting_time << "," << (float) sum_waiting_time / total_number_tasks_allocated << ","
            << simulation_time << "," // Allocation time
            << total_memory_requested << "," << total_power_used << ","
            << total_power_used / (simulation_time + 1) << "," << total_power_used / total_number_tasks_allocated
            << endl;
}


/**
 * Output to file time and current power used every time there's a change
 * i.e. resource gets allocated or freed
 * @param path_to_output_file_from_allocation
 * @param path_to_output_result
 */
void JobsOutputAnalysis::power_over_time(
        const string& path_to_output_file_from_allocation,
        const string& path_to_output_result
        ){
    ifstream input_file(path_to_output_file_from_allocation);
    ofstream output_file(path_to_output_result);

    map<float, float> map_energy_time;

    string str;
    while(!input_file.eof() && getline(input_file,str)) {
        // Find the first index character that's not an escape sequence
        string::size_type begin = str.find_first_not_of(" \f\t\v");
        //Skips blank lines and line starting with #
        if (begin == string::npos || string("#").find(str[begin]) != string::npos) continue;

        vector<string> tokens =  SplitString::split(str, ",");

        // Get values for current task
        string task_id = tokens.at(0);
        float arrival_time = stof(tokens.at(1));
        float scheduled_time = stof(tokens.at(2));
        float number_of_operations = stof(tokens.at(3));
        string id_machine = tokens.at(4);
        float deadline = stof(tokens.at(5));
        float end_time;
        float memory_requested = stof(tokens.at(7));
        float power_used;
        try {
            end_time = stof(tokens.at(6));
            power_used = stof(tokens.at(8));
        }catch (exception&e){
            end_time = -1;
            power_used = 0.0;
        }

        if (end_time > 0){ // if task is allocated on a machine
            // add power to the time of scheduling
            if(map_energy_time.find(scheduled_time) == map_energy_time.end()){
                map_energy_time.insert(pair<float, float>(scheduled_time, power_used));
            }
            else{ map_energy_time[scheduled_time] += power_used; }

            if(map_energy_time.find(end_time) == map_energy_time.end()){
                map_energy_time.insert(pair<float, float>(end_time, -power_used));
            }
            else{ map_energy_time[end_time] -= power_used; }
        }
    }

    output_file << "Time,Power" << endl;
    float prev_power = 0;
    for(map<float, float>::const_iterator it = map_energy_time.begin(); it != map_energy_time.end(); ++it){
        float current_time = it->first;
        float current_power = it->second;

        float sum = 0;
        for (map<float, float>::const_iterator it2 = map_energy_time.begin();
            it2 != map_energy_time.end() && it2->first < current_time; ++it2){
            sum += it2->second;
        }
        if(current_power + sum != prev_power){ // Add record to file only if there's a change in power at current_time
            output_file << current_time << "," << current_power + sum << endl;
            prev_power = current_power + sum;
        }
    }
}



