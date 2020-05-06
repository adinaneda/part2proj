//
// Created by Adina Neda on 23/04/2020.
// Released under license: Apache-2.0 License
//

#include "PerformanceTests.h"
#include "RunSchedulingAndStats.h"

/**
 * Generate file with percentage of tasks with and order of MB memory from 0.1 to 0.9 in 0.1 increments
 * Run scheduling with 3 resources files with ~10K resources:
 * only CPUs, 10K CPUs and switches combined and 10K CPUs + 250 SWs
 * @param priority
 */
void PerformanceTests::generate_file_run_percentage_tasks_mb_test(const string& priority){
    float percentage [9] = {0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9};
    for(const auto& p : percentage){

        const int number_tasks = 100000; // number of lines i.e tasks
        int max_arrival_time = 1; // arrival rate will be in [0, max_arrival_time)
        long max_number_operations = pow(10, 11); // maximum number of operations
        int max_deadline_over_end_time = 500;
        int max_memory_requested = 32000; // 32 GB
        float percentage_mb_to_gb = p;

        const string input_file = RunSchedulingAndStats::GenerateInputFile(
                number_tasks,
                max_arrival_time,
                max_number_operations,
                max_deadline_over_end_time,
                max_memory_requested,
                percentage_mb_to_gb
        );
        cout
                << "Generated input file" << input_file << ".csv"
                << " with " << p << " % MB tasks (out of total incoming tasks) " << endl;

        cout << "Running statistics input file. Results stored in file OutputHandling/OutputFiles/EvaluationStatistics" << endl;
        RunSchedulingAndStats::run_statistics_input_file(input_file);

        string res[3] = {"resources10thousandCPU", "resources10thousandcombined", "resources10thousandCPUplus250SW"};
        for (const auto &resource_file : res) {
            cout
                    << "Running scheduling on file " << input_file
                    << " with priority " << priority << " and resource file " << resource_file << endl;
            RunSchedulingAndStats::scheduling_all_policies(input_file, resource_file, priority);
        }
    }
}

/**
 * Given an input file, run all 3 scheduling policies on it (FCFS queue, offline, EDF),
 * using 3 available resources files with ~10K resources:
 * only CPUs, 10K CPUs and switches combined and 10K CPUs + 250 SWs
 * for both the SW and CPU priorities
 * @param input_file
 */
void PerformanceTests::run_allocation_both_priorities(const string& input_file){
    cout << "File given as input is " << input_file << endl;
    cout << "Running statistics input file. Results stored in file OutputHandling/OutputFiles/EvaluationStatistics" << endl;
    RunSchedulingAndStats::run_statistics_input_file(input_file);

    string priorities [2] = {"SW", "CPU"};
    for(const auto& prio : priorities) {
        string res[3] = {"resources10thousandCPU", "resources10thousandcombined", "resources10thousandCPUplus250SW"};
        for (const auto &resource_file : res) {
            cout
                << "Running scheduling on file " << input_file
                << " with priority " << prio
                << " and resource file " << resource_file << endl;

            RunSchedulingAndStats::scheduling_all_policies(input_file, resource_file, prio);
        }
    }
}

/**
 * Generate new input file for each arrival rate between 10K-100K tasks per second (in increments of 10K)
 * Run scheduling for all 3 scheduling policies
 * Run scheduling with 3 resources files with ~10K resources:
 * only CPUs, 10K CPUs and switches combined and 10K CPUs + 250 SWs
 * @param priority
 */
void PerformanceTests::generate_file_run_incoming_tasks_rate_vs_power(const string& priority){
    int throughputs [10] = {10000, 20000, 30000, 40000, 50000, 60000, 70000, 80000, 90000, 100000};
    for (const auto& current_throughput : throughputs){
        cout << "Current incoming tasks rate: " << current_throughput << endl;

        const int number_tasks = current_throughput; // number of lines i.e tasks
        int max_arrival_time = 1; // arrival rate will be in [0, max_arrival_time)
        long max_number_operations = pow(10, 11); // maximum number of operations
        int max_deadline_over_end_time = 500;
        int max_memory_requested = 32000; // 32 GB
        float percentage_mb_to_gb = 0.5;

        const string input_file = RunSchedulingAndStats::GenerateInputFile(
                number_tasks,
                max_arrival_time,
                max_number_operations,
                max_deadline_over_end_time,
                max_memory_requested,
                percentage_mb_to_gb
        );

        cout
            << "Generated input file" << input_file << ".csv"
            << " with incoming tasks rate = " << current_throughput << endl;
        cout << "Running statistics input file. Results stored in file OutputHandling/OutputFiles/EvaluationStatistics" << endl;

        RunSchedulingAndStats::run_statistics_input_file(input_file);

        string res [3] = {"resources10thousandCPU", "resources10thousandcombined", "resources10thousandplus250SW"};
        for (const auto& resource_file : res){
            cout
                << "Running scheduling FCFS, offline, EDF on file " << input_file
                << " with file of available resources " << resource_file
                << " and resource priority " << priority << endl;
            RunSchedulingAndStats::scheduling_all_policies(input_file, resource_file, priority);
        }
    }
}