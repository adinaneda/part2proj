//
// Created by Adina Neda on 07/03/2020.
// Released under license: Apache-2.0 License
//

#include "RunSchedulingAndStats.h"
#include "../OutputHandling/OutputStats/JobsOutputAnalysis.h"
#include "../InputHandling/GenerateInput/GenerateInput.h"
#include "Paths.h"
#include "RunFCFS.h"
#include "RunEDF.h"
#include "RunOffline.h"
#include "../InputHandling/InputStats/JobsInputAnalysis.h"

/**
 * Generate Tasks input file given the task properties
 * @param number_lines
 * @param max_arrival_time
 * @param max_number_operations
 * @param max_deadline_over_end_time
 * @param max_memory_requested
 * @param percentage_mb_to_gb
 * @return
 */
string RunSchedulingAndStats::GenerateInputFile(
        int number_lines ,
        int max_arrival_time,
        unsigned long long int max_number_operations,
        int max_deadline_over_end_time,
        int max_memory_requested,
        float percentage_mb_to_gb
        ) {

    // Fixed seed for reproducibility - the time when the project needs to be submitted :)
    // srand(1588939200);
    srand(time(0)); // change to fixed seed for the same file name at every run
    string random_file_name = to_string(rand() % 100);

    // Run function to generate input
    GenerateInput::generate_input(
            tasks_dir + random_file_name + ".csv",
            number_lines,
            max_arrival_time, max_number_operations,
            max_deadline_over_end_time,
            max_memory_requested,
            percentage_mb_to_gb
    );

    return random_file_name;
}

/**
 * Run all scheduling policies given an input file with tasks, file with available resources and a resource priority
 * @param input_file
 * @param resources_file_name
 * @param priority_resource
 */
void RunSchedulingAndStats::scheduling_all_policies(
        const string& input_file, const string& resources_file_name,
        const string& priority_resource
        ) {

    // Path to Evaluation Files
    string path_eval_input_file_stats = eval_dir + + "InputStats" + input_file + ".csv";
    string path_eval_output_file_stats = eval_dir + + "OutputStats" + input_file + "-" + resources_file_name + "-" + priority_resource + ".csv";
    string path_eval_sch_stats = eval_dir + + "Scheduling" + input_file + "-" + resources_file_name + "-" + priority_resource + ".csv";

    ofstream out_scheduling_stats(path_eval_sch_stats); // Output Scheduling Statistics
    out_scheduling_stats << "SchPolicy,TasksAlloc,TasksDropped,TasksTotal,PercentageAlloc,PercentageDropped,TotalPower,Duration(secs)" << endl;
    // 2. Run FCFS, FCFS(queue), Offline, EDF scheduling policies
    // RunFCFS::run_fcfs(input_file, resources_file_name, priority_resource, out_scheduling_stats);
    RunFCFS::run_fcfs_queue(input_file, resources_file_name, priority_resource, out_scheduling_stats);
    RunOffline::run_offline(input_file, resources_file_name, priority_resource, out_scheduling_stats);
    RunEDF::run_edf(input_file, resources_file_name, priority_resource, out_scheduling_stats);

    // 3. Run statsistics on output files from the 3 allocation policies
    RunSchedulingAndStats::output_stats_all_policies(path_eval_output_file_stats, input_file, priority_resource, resources_file_name);
}


/**
  * Compute statistics about input file
  * @param input_file_name
  */
void RunSchedulingAndStats::run_statistics_input_file(
        const string& input_file_name
){
    const string path_tasks_input_file = tasks_dir + input_file_name + ".csv";
    ifstream in(path_tasks_input_file);
    string output_file_stats = eval_dir + "InputStats" + input_file_name + ".csv";
    ofstream out(output_file_stats);
    JobsInputAnalysis::input_statistics(in, out);
}

/**
 * Compute statistics about allocation output
 * @param path_eval_output_file_stats
 * @param input_file
 * @param priority
 * @param resources_file_name
 */
void RunSchedulingAndStats::output_stats_all_policies(
        const string& path_eval_output_file_stats,
        const string& input_file, const string& priority,
        const string& resources_file_name
        ){
    ofstream ouptut_evaluationstats_ofstream(path_eval_output_file_stats);

    ouptut_evaluationstats_ofstream << "SchPolicy,TasksDropped,TasksAlloc,TasksTotal,"
                                       "RunningTimeTask(min),RunningTimeTask(max),RunningTimeTask(avg),"
                                       "WaitTimeTask(min),WaitTimeTask(max),WaitTimeTask(avg),AllocTime(ticks),"
                                       "TotalMemory,TotalPower,AvgPower(time),AvgPower(NumTasks)" << endl;

    const string policies [3] = {"FCFSqueue", "Offline", "EDF"};
    for(const auto &policy : policies){
        ouptut_evaluationstats_ofstream << policy << ",";
        ifstream output_stream( output_handling_dir + "OutputFiles/" + policy + "SchedulingOutput/" + policy + input_file + "-" + resources_file_name + "-" + priority + ".csv");
        JobsOutputAnalysis::output_statistics(output_stream, ouptut_evaluationstats_ofstream);
    }
}