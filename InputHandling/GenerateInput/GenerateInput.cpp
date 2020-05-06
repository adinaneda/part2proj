//
// Created by Adina Neda on 22/12/2019.
// Released under license: Apache-2.0 License
//

#include "GenerateInput.h"

/**
 * Generates pseudo-random input with constraints on values
 * @param output_file_name - file where tasks are written
 * @param SIZE - total number of lines in input file i.e. total number of tasks
 * @param max_arrival_time
 * @param max_number_operations
 * @param max_deadline_over_end_time
 * @param max_memory_requested
 * @param percentage_mb_to_gb - percentage of tasks which have an order of MB memory
 *                              (out of the total mnumber of tasks pseudo-randomly generated)
 */
void GenerateInput::generate_input(
        const string& output_file_name,
        int SIZE,
        int max_arrival_time, unsigned long long int max_number_operations, int max_deadline_over_end_time,
        int max_memory_requested,
        float percentage_mb_to_gb) {

    // Fixed seed for repeatability and reproducibility - the time when the project needs to be submitted :)
    srand(1588939200);

    ofstream output_file;
    output_file.open(output_file_name);

    int *start_times = new int[SIZE];
    int *memories = new int[SIZE];
    int order_of_mb = 0;
    int order_of_gb = 0;

    for(int index = 0; index < SIZE; index++){
        start_times[index] = rand() % max_arrival_time; // arrival time in the range 0 to max_arrival_rate

        if (order_of_mb < percentage_mb_to_gb * SIZE && percentage_mb_to_gb != 0){
            do{
                memories[index] = rand() % max_memory_requested; // generate a MB value
            }while(memories[index] > 1000);
            order_of_mb++;
        }else{
            do{
                memories[index] = rand() % max_memory_requested; // generate a GB value
            }while(memories[index] <= 1000);
            order_of_gb++;
        }
    }

    sort(start_times, start_times + SIZE); // sort the start times
    shuffle(&memories[0], &memories[SIZE], std::mt19937(std::random_device()()));

    for (size_t i = 0; i < SIZE; i++){
        long long operations  = (rand() % max_number_operations) + 1; // duration in the range [1, max_number_operations]

        output_file << i << "," // task ID
                    << start_times[i] << "," // start time
                    << operations << "," // operations
                    << start_times[i] + (rand() % max_deadline_over_end_time) << ","  //deadline
                    << memories[i] // memory in [0, max_memory)
                    << endl;
    }

    delete [] start_times;
    delete [] memories;
}
