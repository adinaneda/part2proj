//
// Created by Adina Neda on 26/12/2019.
// Released under license: Apache-2.0 License
//

#include <iostream>
#include <string>

#include "InputHandling/GenerateInput/GenerateInput.h"
#include "Tests/RunSchedulingAndStats.h"
#include "Tests/PerformanceTests.h"
#include "Tests/Paths.h"
#include "Tests/RunOffline.h"
#include "Tests/RunEDF.h"
#include "Tests/RunFCFS.h"
#include "OutputHandling/OutputStats/JobsOutputAnalysis.h"


using namespace std;
namespace fs = std::__fs::filesystem;

int main(int argc, char** argv) {
    // Generate new input
    if(strcmp(argv[1], "GenerateInput") == 0){
        try{
            if(argc < 8) {throw invalid_argument("Invalid number of arguments");}
            const int number_tasks = stoi(argv[2]);
            int max_arrival_time = stoi(argv[3]);
            unsigned long long int max_number_operations = stoll(argv[4]);
            int max_deadline_over_end_time = stoi(argv[5]);
            int max_memory_requested = stoi(argv[6]);
            float percentage_mb_to_gb;

            if(stof(argv[7]) > 1 || stof(argv[7]) <= 0){
                cout << "Percentage order of MB tasks (out of total number of tasks) is not valid\n";
                cout << "Retry with command of the form: "
                        "GenerateInput number_tasks (int) max_arrival_time (int) max_number_operations (long) "
                        "max_wait_time_in_queue (int) max_MB_memory requested (int) percentage_tasks_MB_memory_out_of_total (float) \n";
                return -1; }
            else{
                percentage_mb_to_gb = stof(argv[7]);
            }

            const string input_file = RunSchedulingAndStats::GenerateInputFile(
                    number_tasks,
                    max_arrival_time,
                    max_number_operations,
                    max_deadline_over_end_time,
                    max_memory_requested,
                    percentage_mb_to_gb
            );
            cout << "Generated input file in folder ./InputHandling/InputFiles/Tasks/ with name " << input_file << ".csv" << endl;
        }
        catch (exception& e) {
            cout << e.what() << endl;
            cout << "Exception when parsing the input arguments to generate an input file\n";
            cout << "Retry with command of the form: "
                    "GenerateInput number_tasks (int) max_arrival_time (int) max_number_operations (long) "
                    "max_wait_time_in_queue (int) max_MB_memory_requested (int) percentage_tasks_MB_memory_out_of_total (float) \n";
            return -1;
        }
    }
    // RUn statistics on input file
    else if(strcmp(argv[1], "RunInputStats") == 0){
        try{
            RunSchedulingAndStats::run_statistics_input_file(argv[2]);
            cout << "Ran statistics on input. Find the results in ./OutputHandling/OutputFiles/EvaluationStatistics/\n";
        } catch (exception& e) {
            cout << "Exception when attempting to run input statistics\n";
            cout << "Retry with command of the form: "
                    "RunInputStats input_file_name (string, ignore extension!)\n";
            cout << "File must be a csv file of the correct format, present in directory ./InputHandling/InputFiles/Tasks/\n";
            return -1;
        }
    }
    // Run all scheduling statistics
    else if (strcmp(argv[1], "SchedulingAll") == 0){
        try{
            if(argc < 5) {throw invalid_argument("Invalid number of arguments");}
            string input_file = argv[2];
            string available_resources_file = argv[3];
            string priority_resource;

            if (strcmp(argv[4], "SW") == 0 || strcmp(argv[4], "CPU") == 0){ priority_resource = string(argv[4]); }
            else{
                cout << "Incorrect argument for priority resource! \n";
                cout << "Retry with command of the form: "
                        "SchedulingAll input_file_name (string, ignore extension!), resources_file_name (string, ignore extension!) priority_resource (string: CPU or SW) \n";
                return -1;
            }
            RunSchedulingAndStats::scheduling_all_policies(input_file, available_resources_file, priority_resource);
        } catch (exception &e) {
            cout << "Error when attempting to schedule taks on resources \n";
            cout << "Retry with command of the form: "
                    "SchedulingAll input_file_name (string, ignore extension!), resources_file_name (string, ignore extension!) priority_resource (string: CPU or SW) \n";
            return -1;
        }
    }
    // Run scheduling given a specified policy
    else if (strcmp(argv[1], "Scheduling") == 0){
        try{
            if(argc < 6) {throw invalid_argument("Invalid number of arguments");}
            string input_file = argv[3];
            string resources_file_name = argv[4];
            string priority_resource;

            if (strcmp(argv[5], "SW") == 0 || strcmp(argv[5], "CPU") == 0){ priority_resource = string(argv[5]); }
            else{
                cout << "Incorrect argument for priority resource! \n";
                cout << "Retry with command of the form: "
                        "Scheduling policy_name input_file_name (string, ignore extension!), resources_file_name (string, ignore extension!) priority_resource (string: CPU or SW) \n";
                return -1;
            }
            string path_eval_input_file_stats = eval_dir + + "InputStats" + input_file + ".csv";
            string path_eval_output_file_stats = eval_dir + + "OutputStats" + input_file + "-" + resources_file_name + "-" + priority_resource + ".csv";
            string path_eval_sch_stats = eval_dir + + "Scheduling" + input_file + "-" + resources_file_name + "-" + priority_resource + ".csv";

            if(strcmp(argv[2], "fcfsqueue") == 0){
                ofstream out_scheduling_stats(path_eval_sch_stats); // Output Scheduling Statistics
                out_scheduling_stats << "SchPolicy,TasksAlloc,TasksDropped,TasksTotal,PercentageAlloc,PercentageDropped,TotalPower,Duration(secs)" << endl;
                RunFCFS::run_fcfs_queue(input_file, resources_file_name, priority_resource, out_scheduling_stats);

                ofstream ouptut_evaluationstats_ofstream(path_eval_output_file_stats);
                ouptut_evaluationstats_ofstream << "SchPolicy,TasksDropped,TasksAlloc,TasksTotal,"
                                                   "RunningTimeTask(min),RunningTimeTask(max),RunningTimeTask(avg),"
                                                   "WaitTimeTask(min),WaitTimeTask(max),WaitTimeTask(avg),AllocTime(ticks),"
                                                   "TotalMemory,TotalPower,AvgPower(time),AvgPower(NumTasks)" << endl;
                ouptut_evaluationstats_ofstream << "FCFSqueue" << ",";
                ifstream output_stream( output_handling_dir + "OutputFiles/FCFSqueueSchedulingOutput/FCFSqueue" + input_file + "-" + resources_file_name + "-" + priority_resource + ".csv");
                JobsOutputAnalysis::output_statistics(output_stream, ouptut_evaluationstats_ofstream);

            }
            else if(strcmp(argv[2], "offline") == 0){
                ofstream out_scheduling_stats(path_eval_sch_stats); // Output Scheduling Statistics
                out_scheduling_stats << "SchPolicy,TasksAlloc,TasksDropped,TasksTotal,PercentageAlloc,PercentageDropped,TotalPower,Duration(secs)" << endl;
                RunOffline::run_offline(input_file, resources_file_name, priority_resource, out_scheduling_stats);

                ofstream ouptut_evaluationstats_ofstream(path_eval_output_file_stats);
                ouptut_evaluationstats_ofstream << "SchPolicy,TasksDropped,TasksAlloc,TasksTotal,"
                                                   "RunningTimeTask(min),RunningTimeTask(max),RunningTimeTask(avg),"
                                                   "WaitTimeTask(min),WaitTimeTask(max),WaitTimeTask(avg),AllocTime(ticks),"
                                                   "TotalMemory,TotalPower,AvgPower(time),AvgPower(NumTasks)" << endl;
                ouptut_evaluationstats_ofstream << "Offline" << ",";
                ifstream output_stream( output_handling_dir + "OutputFiles/OfflineSchedulingOutput/Offline" + input_file + "-" + resources_file_name + "-" + priority_resource + ".csv");
                JobsOutputAnalysis::output_statistics(output_stream, ouptut_evaluationstats_ofstream);
            }
            else if(strcmp(argv[2], "edf") == 0){
                ofstream out_scheduling_stats(path_eval_sch_stats); // Output Scheduling Statistics
                out_scheduling_stats << "SchPolicy,TasksAlloc,TasksDropped,TasksTotal,PercentageAlloc,PercentageDropped,TotalPower,Duration(secs)" << endl;
                RunEDF::run_edf(input_file, resources_file_name, priority_resource, out_scheduling_stats);

                ofstream ouptut_evaluationstats_ofstream(path_eval_output_file_stats);
                ouptut_evaluationstats_ofstream << "SchPolicy,TasksDropped,TasksAlloc,TasksTotal,"
                                                   "RunningTimeTask(min),RunningTimeTask(max),RunningTimeTask(avg),"
                                                   "WaitTimeTask(min),WaitTimeTask(max),WaitTimeTask(avg),AllocTime(ticks),"
                                                   "TotalMemory,TotalPower,AvgPower(time),AvgPower(NumTasks)" << endl;
                ouptut_evaluationstats_ofstream << "EDF" << ",";
                ifstream output_stream( output_handling_dir + "OutputFiles/EDFSchedulingOutput/EDF" + input_file + "-" + resources_file_name + "-" + priority_resource + ".csv");
                JobsOutputAnalysis::output_statistics(output_stream, ouptut_evaluationstats_ofstream);
            }
            cout << "Saved scheduling and stats results can be found in ./OutputHandling/OutputFiles/" << endl;
        } catch (exception& e) {
            cout << "Error when attempting to schedule taks on resources \n";
            cout << "Retry with command of the form: "
                    "Scheduling policy_name input_file_name (string, ignore extension!), resources_file_name (string, ignore extension!) priority_resource (string: CPU or SW) \n";
            return -1;
        }
    }
    // Tasks rate vs. Power Test
    else if (strcmp(argv[1], "TasksRatePower") == 0){
        try{
            if(argc < 3) { throw invalid_argument("Invalid number of arguments"); }
            if(strcmp(argv[2], "SW") == 0 || strcmp(argv[2], "CPU") == 0){
                PerformanceTests::generate_file_run_incoming_tasks_rate_vs_power(argv[2]);
            }else{
                throw invalid_argument("Priority is not recognised. Argument can be CPU or SW");
            }

        }
        catch(exception& e){
            cout
                << "Error when attemptiong to run test TasksRatePower \n"
                << "Retry with command of the form: TasksRatePower priority \n";
            return -1;
        }
    }
    return 0;
}


