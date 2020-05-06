//
// Created by Adina Neda on 26/12/2019.
// Released under license: Apache-2.0 License
//

#include "JobsInputAnalysis.h"
#include "../../Auxiliaries/SplitString.h"

/**
 * Run statistics about the input file
 * @param generated_input
 * @param output_file_results
 */
void JobsInputAnalysis::input_statistics(
        ifstream& generated_input,
        ofstream& output_file_results
        ) {
    long long int sum_deadlines = 0;
    int max_deadline = INT_MIN;
    int min_deadline = INT_MAX;
    float avg_deadline = 0.0;

    long long int sum_number_of_operations = 0;
    int max_number_of_operations = INT_MIN;
    int min_number_of_operations = INT_MAX;
    float avg_number_of_operations = 0.0;

    int total_number_of_tasks = 0;

    int max_arrival_time = INT_MIN;
    int min_arrival_time = INT_MAX;

    string str;
    while(!generated_input.eof() && getline(generated_input,str)) {
        // Find the first index character that's not an escape sequence
        string::size_type begin = str.find_first_not_of(" \f\t\v");
        //Skips blank lines and line starting with #
        if (begin == string::npos || string("#").find(str[begin]) != string::npos) continue;

        vector<string> tokens = SplitString::split(str, ",");

        // Get values for current task
        string task_id = tokens.at(0);
        int arrival_time = stoi(tokens.at(1));
        int number_of_operations = stoi(tokens.at(2));
        int deadline = stoi(tokens.at(3));

        /**
         * Calculate:
         * 1. max, min, average deadline
         * 2. max, min, average number of operations
         * 3. total number of tasks in the input file
         * 4. max, min arrival time
         * 5. arrival rate: total number of tasks / maximum arrival time
         */
        sum_deadlines += deadline;
        if(deadline < min_deadline) { min_deadline = deadline; }
        if(deadline > max_deadline) { max_deadline = deadline; }

        sum_number_of_operations += number_of_operations;
        if(number_of_operations < min_number_of_operations) { min_number_of_operations = number_of_operations; }
        if(number_of_operations > max_number_of_operations) { max_number_of_operations = number_of_operations; }

        total_number_of_tasks++;

        if(arrival_time < min_arrival_time) { min_arrival_time = arrival_time; }
        if(arrival_time > max_arrival_time) { max_arrival_time = arrival_time; }
    }

    /*
     * Output statistics
     */

    output_file_results
        << "Deadline(min),Deadline(max),Deadline(avg),NumOps(min),NumOps(max),NumOps(avg),ArrivalTime(min),ArrivalTime(max),ArrivalRate" << endl
        << min_deadline << "," << max_deadline << "," << (float) (sum_deadlines) / total_number_of_tasks << ","
        << min_number_of_operations << "," << max_number_of_operations << "," << (float) (sum_number_of_operations) / total_number_of_tasks << ","
        << min_arrival_time << "," << max_arrival_time << ","
        << (float) total_number_of_tasks / (max_arrival_time + 1) << endl;
}

